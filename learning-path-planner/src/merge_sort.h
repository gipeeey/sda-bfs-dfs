#ifndef MERGE_SORT_H
#define MERGE_SORT_H

#include <vector>
#include "ai_scoring.h"

using namespace std;

// Kompleksitas: O(k log k), k = jumlah kandidat.
// Manual merge sort (BUKAN std::sort), urutkan descending by finalScore.
void mergeSortCandidates(vector<ScoredCandidate>& candidates);

#endif
