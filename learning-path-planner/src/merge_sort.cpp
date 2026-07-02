#include "merge_sort.h"

static void merge(vector<ScoredCandidate>& candidates, int left, int mid, int right) {
    vector<ScoredCandidate> leftPart(candidates.begin() + left, candidates.begin() + mid + 1);
    vector<ScoredCandidate> rightPart(candidates.begin() + mid + 1, candidates.begin() + right + 1);

    size_t i = 0, j = 0;
    int k = left;
    while (i < leftPart.size() && j < rightPart.size()) {
        if (leftPart[i].finalScore >= rightPart[j].finalScore) {
            candidates[k++] = leftPart[i++];
        } else {
            candidates[k++] = rightPart[j++];
        }
    }
    while (i < leftPart.size()) candidates[k++] = leftPart[i++];
    while (j < rightPart.size()) candidates[k++] = rightPart[j++];
}

static void mergeSortRange(vector<ScoredCandidate>& candidates, int left, int right) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;
    mergeSortRange(candidates, left, mid);
    mergeSortRange(candidates, mid + 1, right);
    merge(candidates, left, mid, right);
}

// Kompleksitas: O(k log k), k = jumlah kandidat. Manual merge sort (bukan std::sort).
void mergeSortCandidates(vector<ScoredCandidate>& candidates) {
    if (candidates.empty()) return;
    mergeSortRange(candidates, 0, static_cast<int>(candidates.size()) - 1);
}
