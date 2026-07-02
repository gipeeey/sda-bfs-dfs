#include "ai_scoring.h"
#include <iostream>
#include <algorithm>

static double clampScore(double value) {
    if (value < 0.0) return 0.0;
    if (value > 10.0) return 10.0;
    return value;
}

ScoredCandidate calculateScore(const PathCandidate& candidate, double timeBudget,
                                int minModuleCountAmongCandidates) {
    ScoredCandidate scored;
    scored.candidate = candidate;

    scored.efficiencyScore = clampScore(10.0 - (candidate.totalHours / timeBudget) * 10.0);
    scored.difficultyScore = clampScore(10.0 - candidate.avgDifficulty);
    scored.concisenessScore = clampScore(10.0 - (candidate.moduleCount - minModuleCountAmongCandidates) * 2.0);

    scored.finalScore = (scored.efficiencyScore * 0.45) + (scored.difficultyScore * 0.35)
                       + (scored.concisenessScore * 0.20);

    return scored;
}

vector<ScoredCandidate> calculateScoreForAll(const vector<PathCandidate>& candidates, double timeBudget) {
    vector<ScoredCandidate> scoredList;
    if (candidates.empty()) return scoredList;

    int minModuleCount = candidates[0].moduleCount;
    for (size_t i = 1; i < candidates.size(); i++) {
        if (candidates[i].moduleCount < minModuleCount) {
            minModuleCount = candidates[i].moduleCount;
        }
    }

    for (size_t i = 0; i < candidates.size(); i++) {
        scoredList.push_back(calculateScore(candidates[i], timeBudget, minModuleCount));
    }

    return scoredList;
}

static string sequenceToString(const vector<string>& sequence) {
    string result;
    for (size_t i = 0; i < sequence.size(); i++) {
        if (i > 0) result += " -> ";
        result += sequence[i];
    }
    return result;
}

void printDecisionTrace(const vector<ScoredCandidate>& sortedCandidates) {
    cout << "\n=== Decision Trace (Rule-Based Scoring) ===" << endl;
    if (sortedCandidates.empty()) {
        cout << "Tidak ada kandidat untuk dibandingkan." << endl;
        return;
    }

    for (size_t i = 0; i < sortedCandidates.size(); i++) {
        const ScoredCandidate& sc = sortedCandidates[i];
        cout << "\nKandidat " << (i + 1) << ": " << sequenceToString(sc.candidate.moduleSequence) << endl;
        cout << "  Total jam        : " << sc.candidate.totalHours << endl;
        cout << "  Rata-rata sulit  : " << sc.candidate.avgDifficulty << endl;
        cout << "  Jumlah modul     : " << sc.candidate.moduleCount << endl;
        cout << "  efficiencyScore  : " << sc.efficiencyScore << " (bobot 0.45)" << endl;
        cout << "  difficultyScore  : " << sc.difficultyScore << " (bobot 0.35)" << endl;
        cout << "  concisenessScore : " << sc.concisenessScore << " (bobot 0.20)" << endl;
        cout << "  finalScore       : " << sc.finalScore << endl;
    }

    const ScoredCandidate& best = sortedCandidates[0];
    cout << "\n>> Kandidat terpilih: " << sequenceToString(best.candidate.moduleSequence)
         << " (finalScore " << best.finalScore << ")" << endl;
    if (sortedCandidates.size() > 1) {
        cout << ">> Alasan: skor efisiensi waktu (bobot terbesar 0.45) dan tingkat kesulitan "
             << "(bobot 0.35) kandidat ini lebih unggul dibanding kandidat lain, "
             << "sehingga finalScore-nya tertinggi." << endl;
    }
}

void printFinalRecommendation(const ScoredCandidate& best) {
    cout << "\n=== Rekomendasi Jalur Belajar Final ===" << endl;
    cout << "Urutan modul yang harus diambil: " << sequenceToString(best.candidate.moduleSequence) << endl;
    cout << "Total waktu belajar: " << best.candidate.totalHours << " jam" << endl;
    cout << "Rata-rata kesulitan: " << best.candidate.avgDifficulty << endl;
    cout << "Skor akhir (rule-based): " << best.finalScore << endl;
}
