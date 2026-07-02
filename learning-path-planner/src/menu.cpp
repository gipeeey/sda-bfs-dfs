#include "menu.h"
#include "search_module.h"
#include "bfs_baseline.h"
#include "dfs_path_enum.h"
#include "ai_scoring.h"
#include "merge_sort.h"
#include <iostream>
#include <limits>
#include <sstream>

void showModuleCatalog(const vector<Module>& catalog) {
    cout << "\n=== Katalog Modul ===" << endl;
    for (size_t i = 0; i < catalog.size(); i++) {
        const Module& m = catalog[i];
        cout << m.code << " - " << m.name << endl;
        cout << "    Prasyarat  : ";
        if (m.prereqGroups.empty()) {
            cout << "(tidak ada)";
        } else {
            for (size_t g = 0; g < m.prereqGroups.size(); g++) {
                if (g > 0) cout << " ATAU ";
                cout << "[";
                for (size_t j = 0; j < m.prereqGroups[g].size(); j++) {
                    if (j > 0) cout << ", ";
                    cout << m.prereqGroups[g][j];
                }
                cout << "]";
            }
        }
        cout << endl;
        cout << "    Jam        : " << m.estimatedHours << endl;
        cout << "    Kesulitan  : " << m.difficulty << "/10" << endl;
    }
}

static void printSearchResult(const vector<Module>& catalog, int index) {
    if (index == -1) {
        cout << "Modul tidak ditemukan." << endl;
        return;
    }
    const Module& m = catalog[index];
    cout << "Ditemukan: " << m.code << " - " << m.name
         << " (jam: " << m.estimatedHours << ", kesulitan: " << m.difficulty << "/10)" << endl;
}

void searchModuleMenu(const vector<Module>& catalog) {
    cout << "\n=== Cari Modul ===" << endl;
    cout << "1. Linear Search (by kode/nama)" << endl;
    cout << "2. Binary Search (by kode)" << endl;
    cout << "Pilihan: ";

    int mode;
    if (!(cin >> mode)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Input tidak valid." << endl;
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (mode == 1) {
        cout << "Masukkan kode atau nama modul: ";
        string query;
        getline(cin, query);
        printSearchResult(catalog, linearSearchModule(catalog, query));
    } else if (mode == 2) {
        cout << "Masukkan kode modul: ";
        string code;
        getline(cin, code);
        printSearchResult(catalog, binarySearchModule(catalog, code));
    } else {
        cout << "Mode pencarian tidak dikenal." << endl;
    }
}

StudentProfile selectStudentProfileMenu() {
    cout << "\n=== Pilih Profil Mahasiswa ===" << endl;
    cout << "1. Dimas (sample)" << endl;
    cout << "2. Sari (sample)" << endl;
    cout << "3. Input Manual" << endl;
    cout << "Pilihan: ";

    int choice;
    if (!(cin >> choice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Input tidak valid, memakai profil Dimas." << endl;
        return getSampleProfileDimas();
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice == 2) {
        return getSampleProfileSari();
    }
    if (choice == 3) {
        StudentProfile p;
        p.name = "Manual";

        cout << "Nama mahasiswa: ";
        getline(cin, p.name);

        cout << "Modul yang sudah dikuasai (pisahkan dengan koma, contoh: M1,M2): ";
        string masteredLine;
        getline(cin, masteredLine);
        stringstream ss(masteredLine);
        string code;
        while (getline(ss, code, ',')) {
            if (!code.empty()) p.masteredModules.insert(code);
        }

        cout << "Target modul: ";
        getline(cin, p.targetModule);

        cout << "Budget waktu (jam): ";
        cin >> p.timeBudgetHours;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        return p;
    }

    return getSampleProfileDimas();
}

// Kompleksitas: O(k), k = jumlah kandidat -- linear scan buang kandidat yang tidak muat budget.
static vector<PathCandidate> filterByBudget(const vector<PathCandidate>& candidates, double timeBudget) {
    vector<PathCandidate> filtered;
    for (size_t i = 0; i < candidates.size(); i++) {
        if (candidates[i].totalHours <= timeBudget) {
            filtered.push_back(candidates[i]);
        }
    }
    return filtered;
}

static double minTotalHours(const vector<PathCandidate>& candidates) {
    double minHours = candidates[0].totalHours;
    for (size_t i = 1; i < candidates.size(); i++) {
        if (candidates[i].totalHours < minHours) minHours = candidates[i].totalHours;
    }
    return minHours;
}

// Edge case #2: cari target alternatif yang feasible. Dibatasi ke prasyarat LANGSUNG
// dari target semula (parent terdekat di DAG) -- lihat CLAUDE.md bagian 5.3: Sari
// disarankan target M6 (bukan M4 yang lebih murah tapi 2 langkah lebih jauh dari M7)
// karena M6 adalah prasyarat langsung M7 yang paling murah & sudah "ready".
static bool findFeasibleAlternativeTarget(const string& originalTarget, const StudentProfile& profile,
                                           const vector<Module>& catalog, const ModuleGraph& graph,
                                           ScoredCandidate& bestAlternative) {
    int idx = findModuleIndexByCode(catalog, originalTarget);
    if (idx == -1) return false;

    set<string> directPrereqs;
    for (size_t g = 0; g < catalog[idx].prereqGroups.size(); g++) {
        for (size_t j = 0; j < catalog[idx].prereqGroups[g].size(); j++) {
            directPrereqs.insert(catalog[idx].prereqGroups[g][j]);
        }
    }

    vector<ScoredCandidate> bestPerTarget;
    for (const string& altTarget : directPrereqs) {
        if (profile.masteredModules.count(altTarget)) continue;

        set<string> altRequired = collectRequiredModules(altTarget, profile.masteredModules, graph);
        vector<PathCandidate> altCandidates = enumerateAllPaths(altTarget, altRequired,
                                                                 profile.masteredModules, graph, catalog);
        vector<PathCandidate> altFeasible = filterByBudget(altCandidates, profile.timeBudgetHours);
        if (altFeasible.empty()) continue;

        // Pilih kandidat paling thorough (totalHours terbesar yang masih muat budget),
        // bukan yang paling murah -- supaya waktu belajar tersisa dipakai maksimal.
        size_t bestIdx = 0;
        for (size_t i = 1; i < altFeasible.size(); i++) {
            if (altFeasible[i].totalHours > altFeasible[bestIdx].totalHours) bestIdx = i;
        }
        vector<ScoredCandidate> scored = calculateScoreForAll(altFeasible, profile.timeBudgetHours);
        bestPerTarget.push_back(scored[bestIdx]);
    }

    if (bestPerTarget.empty()) return false;

    size_t bestTargetIdx = 0;
    for (size_t i = 1; i < bestPerTarget.size(); i++) {
        if (bestPerTarget[i].candidate.totalHours > bestPerTarget[bestTargetIdx].candidate.totalHours) {
            bestTargetIdx = i;
        }
    }
    bestAlternative = bestPerTarget[bestTargetIdx];
    return true;
}

void recommendLearningPathMenu(const StudentProfile& profile, const vector<Module>& catalog,
                                const ModuleGraph& graph) {
    cout << "\n=== Rekomendasi Jalur Belajar: " << profile.name << " ===" << endl;
    cout << "Target: " << profile.targetModule << " | Budget: " << profile.timeBudgetHours << " jam" << endl;

    // Edge case #3: kode modul target tidak ada di katalog.
    if (findModuleIndexByCode(catalog, profile.targetModule) == -1) {
        cout << "Target modul '" << profile.targetModule << "' tidak ditemukan di katalog." << endl;
        return;
    }

    // Edge case #1: target sudah dikuasai.
    if (profile.masteredModules.count(profile.targetModule)) {
        cout << "Target modul " << profile.targetModule << " sudah dikuasai. Tidak perlu rekomendasi." << endl;
        return;
    }

    set<string> required = collectRequiredModules(profile.targetModule, profile.masteredModules, graph);

    vector<string> baseline = findBaselinePath(required, profile.masteredModules, graph);
    cout << "\nJalur baseline (BFS): ";
    for (size_t i = 0; i < baseline.size(); i++) {
        if (i > 0) cout << " -> ";
        cout << baseline[i];
    }
    cout << endl;

    vector<PathCandidate> allCandidates = enumerateAllPaths(profile.targetModule, required,
                                                              profile.masteredModules, graph, catalog);
    vector<PathCandidate> feasible = filterByBudget(allCandidates, profile.timeBudgetHours);

    if (feasible.empty()) {
        double minHours = minTotalHours(allCandidates);
        cout << "\nTarget " << profile.targetModule << " butuh minimal " << minHours
             << " jam, budget Anda " << profile.timeBudgetHours << " jam, kurang "
             << (minHours - profile.timeBudgetHours) << " jam." << endl;

        ScoredCandidate alternative;
        if (findFeasibleAlternativeTarget(profile.targetModule, profile, catalog, graph, alternative)) {
            cout << "\nAlternatif yang disarankan: target "
                 << alternative.candidate.moduleSequence.back()
                 << " (muat dalam budget)." << endl;
            printFinalRecommendation(alternative);
        } else {
            cout << "Tidak ditemukan target alternatif yang feasible dalam budget." << endl;
        }

        double shortfall = minHours - profile.timeBudgetHours;
        if (shortfall <= 5.0) {
            cout << "\nCatatan: Anda hanya kurang " << shortfall << " jam dari target asli "
                 << profile.targetModule << ". Pertimbangkan menambah waktu belajar sedikit "
                 << "untuk langsung mencapai target awal." << endl;
        }
        return;
    }

    vector<ScoredCandidate> scored = calculateScoreForAll(feasible, profile.timeBudgetHours);
    mergeSortCandidates(scored);
    printDecisionTrace(scored);
    printFinalRecommendation(scored[0]);
}

void showGraphStructureMenu(const ModuleGraph& graph, const vector<Module>& catalog) {
    printGraphStructure(graph, catalog);
}

void runMainMenu(const vector<Module>& catalog, const ModuleGraph& graph) {
    bool running = true;
    StudentProfile currentProfile = getSampleProfileDimas();

    while (running) {
        cout << "\n=== Smart Learning Path Recommender ===" << endl;
        cout << "1. Lihat Katalog Modul" << endl;
        cout << "2. Cari Modul (Linear / Binary Search)" << endl;
        cout << "3. Pilih Profil Mahasiswa (Dimas / Sari / Input Manual)" << endl;
        cout << "4. Rekomendasikan Jalur Belajar" << endl;
        cout << "5. Lihat Struktur Graph (DAG modul & prasyarat)" << endl;
        cout << "6. Keluar" << endl;
        cout << "Pilihan: ";

        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Input tidak valid." << endl;
            continue;
        }

        switch (choice) {
            case 1:
                showModuleCatalog(catalog);
                break;
            case 2:
                searchModuleMenu(catalog);
                break;
            case 3:
                currentProfile = selectStudentProfileMenu();
                break;
            case 4:
                recommendLearningPathMenu(currentProfile, catalog, graph);
                break;
            case 5:
                showGraphStructureMenu(graph, catalog);
                break;
            case 6:
                running = false;
                break;
            default:
                cout << "Pilihan tidak dikenal." << endl;
                break;
        }
    }
}
