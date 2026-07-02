#ifndef SEARCH_MODULE_H
#define SEARCH_MODULE_H

#include <string>
#include <vector>
#include "module.h"

using namespace std;

// Kompleksitas: O(n) — scan linear katalog by kode atau nama.
// Return index modul kalau ditemukan, -1 kalau "tidak ditemukan" (bukan crash).
int linearSearchModule(const vector<Module>& catalog, const string& query);

// Kompleksitas: O(log n) — binary search by kode. Asumsi katalog sudah terurut by code.
// Manual (bukan std::binary_search). Return index modul kalau ditemukan, -1 kalau tidak.
int binarySearchModule(const vector<Module>& catalog, const string& code);

#endif
