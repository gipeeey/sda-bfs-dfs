#include "module.h"

// Kompleksitas: O(n) — membangun n modul sample sekali saat dipanggil.
vector<Module> getModuleCatalog() {
    vector<Module> catalog;

    catalog.push_back({"M1", "Dasar Pemrograman & Logika", {}, 4.0, 2});
    catalog.push_back({"M2", "Array & Struktur Data Linear", {{"M1"}}, 5.0, 3});
    catalog.push_back({"M3", "Rekursi", {{"M1"}}, 6.0, 4});
    catalog.push_back({"M4", "Linked List, Stack & Queue", {{"M2"}, {"M3"}}, 6.0, 4});
    catalog.push_back({"M5", "Searching & Sorting Dasar", {{"M4"}}, 7.0, 5});
    catalog.push_back({"M6", "Pohon (Tree) & Struktur Hierarkis", {{"M4"}, {"M2", "M3"}}, 8.0, 6});
    catalog.push_back({"M7", "Graph & Algoritma Traversal", {{"M5", "M6"}}, 10.0, 8});

    return catalog;
}

// Kompleksitas: O(n) — linear scan, dipakai sebagai helper internal (bukan fitur search di menu).
int findModuleIndexByCode(const vector<Module>& catalog, const string& code) {
    for (size_t i = 0; i < catalog.size(); i++) {
        if (catalog[i].code == code) {
            return static_cast<int>(i);
        }
    }
    return -1;
}
