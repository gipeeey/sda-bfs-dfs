#ifndef AI_SCORING_H
#define AI_SCORING_H

#include <vector>
#include "dfs_path_enum.h"

using namespace std;

// CATATAN: Modul ini mengimplementasikan rule-based expert system (symbolic AI),
// BUKAN machine learning. Skor dihitung dari formula weighted-sum yang aturannya
// didefinisikan manusia (hardcoded), bukan dipelajari dari data training.

// Kandidat jalur beserta breakdown skor rule-based-nya (untuk decision trace).
struct ScoredCandidate {
    PathCandidate candidate;
    double efficiencyScore;
    double difficultyScore;
    double concisenessScore;
    double finalScore;
};

// Kompleksitas: O(m), m = jumlah modul per kandidat.
// Formula (CLAUDE.md bagian 7):
//   efficiencyScore  = clamp(10 - (totalHours / timeBudget) * 10, 0, 10)
//   difficultyScore  = clamp(10 - avgDifficulty, 0, 10)
//   concisenessScore = clamp(10 - (moduleCount - minModuleCountAmongCandidates) * 2, 0, 10)
//   finalScore = efficiencyScore*0.45 + difficultyScore*0.35 + concisenessScore*0.20
ScoredCandidate calculateScore(const PathCandidate& candidate, double timeBudget,
                                int minModuleCountAmongCandidates);

// Menghitung skor seluruh kandidat sekaligus (minModuleCountAmongCandidates dihitung
// dinamis dari kumpulan kandidat yang sedang dibandingkan, bukan konstanta).
vector<ScoredCandidate> calculateScoreForAll(const vector<PathCandidate>& candidates, double timeBudget);

// Tampilkan decision trace lengkap: skor tiap kandidat + breakdown per komponen +
// alasan kenapa satu kandidat terpilih dibanding yang lain (transparansi rule-based).
void printDecisionTrace(const vector<ScoredCandidate>& sortedCandidates);

// Tampilkan rekomendasi final (kandidat terbaik) + urutan langkah belajarnya.
void printFinalRecommendation(const ScoredCandidate& best);

#endif
