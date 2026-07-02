#include "search_module.h"

// Kompleksitas: O(n) — scan linear, cocokkan query terhadap code atau name (case-sensitive).
int linearSearchModule(const vector<Module>& catalog, const string& query) {
    for (size_t i = 0; i < catalog.size(); i++) {
        if (catalog[i].code == query || catalog[i].name == query) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

// Kompleksitas: O(log n) — binary search manual, asumsi catalog terurut by code.
int binarySearchModule(const vector<Module>& catalog, const string& code) {
    int low = 0;
    int high = static_cast<int>(catalog.size()) - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (catalog[mid].code == code) {
            return mid;
        } else if (catalog[mid].code < code) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}
