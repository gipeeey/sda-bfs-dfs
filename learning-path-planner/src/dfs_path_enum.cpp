#include "dfs_path_enum.h"

static PathCandidate buildCandidate(const vector<string>& sequence, const vector<Module>& catalog) {
    PathCandidate candidate;
    candidate.moduleSequence = sequence;
    candidate.moduleCount = static_cast<int>(sequence.size());

    double totalHours = 0.0;
    int totalDifficulty = 0;
    for (const string& code : sequence) {
        int idx = findModuleIndexByCode(catalog, code);
        if (idx != -1) {
            totalHours += catalog[idx].estimatedHours;
            totalDifficulty += catalog[idx].difficulty;
        }
    }
    candidate.totalHours = totalHours;
    candidate.avgDifficulty = sequence.empty() ? 0.0 : static_cast<double>(totalDifficulty) / sequence.size();

    return candidate;
}

// Kunci dedup berdasarkan SET modul (bukan urutan) -- set<string> otomatis terurut
// sehingga dua urutan berbeda dari himpunan modul yang sama menghasilkan kunci sama.
static string setKey(const vector<string>& sequence) {
    set<string> asSet(sequence.begin(), sequence.end());
    string key;
    for (const string& code : asSet) {
        key += code;
        key += ",";
    }
    return key;
}

static void dfsEnumerate(const string& targetModule, const set<string>& requiredModules,
                          set<string>& fulfilled, vector<string>& sequence,
                          const ModuleGraph& graph, const vector<Module>& catalog,
                          vector<PathCandidate>& candidates, set<string>& seenSets) {
    for (const string& code : requiredModules) {
        if (fulfilled.count(code)) continue;
        if (!isModuleReady(code, fulfilled, graph)) continue;

        sequence.push_back(code);
        fulfilled.insert(code);

        if (code == targetModule) {
            string key = setKey(sequence);
            if (!seenSets.count(key)) {
                seenSets.insert(key);
                candidates.push_back(buildCandidate(sequence, catalog));
            }
        } else {
            // Backtracking: lanjut coba tambah modul lain (termasuk modul dari
            // OR-branch yang sebetulnya tidak wajib) sebelum akhirnya menutup
            // jalur lewat targetModule -- ini yang menghasilkan kandidat "boros"
            // seperti Kandidat C (lewat M2 & M3 sekaligus) di CLAUDE.md bagian 5.2.
            dfsEnumerate(targetModule, requiredModules, fulfilled, sequence, graph, catalog,
                         candidates, seenSets);
        }

        fulfilled.erase(code);
        sequence.pop_back();
    }
}

// Kompleksitas: O(2^V) worst-case — lihat header untuk penjelasan lengkap.
vector<PathCandidate> enumerateAllPaths(const string& targetModule, const set<string>& requiredModules,
                                        const set<string>& mastered, const ModuleGraph& graph,
                                        const vector<Module>& catalog) {
    vector<PathCandidate> candidates;
    set<string> fulfilled = mastered;
    vector<string> sequence;
    set<string> seenSets;

    dfsEnumerate(targetModule, requiredModules, fulfilled, sequence, graph, catalog, candidates, seenSets);

    return candidates;
}
