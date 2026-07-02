#ifndef MODULE_H
#define MODULE_H

#include <string>
#include <vector>

using namespace std;

// Representasi satu modul belajar di katalog.
// prereqGroups: OR-of-AND groups. Modul "ready" jika mahasiswa sudah
// menguasai SEMUA modul di SALAH SATU grup (lihat CLAUDE.md bagian 4.2).
struct Module {
    string code;
    string name;
    vector<vector<string>> prereqGroups;
    double estimatedHours;
    int difficulty;
};

// Mengembalikan katalog 7 modul sample (hardcoded, sesuai CLAUDE.md bagian 5.1).
// Katalog dikembalikan dalam urutan terurut by code (M1..M7) supaya siap dipakai binary search.
vector<Module> getModuleCatalog();

// Cari index modul di katalog berdasarkan code, -1 kalau tidak ada.
// Dipakai internal oleh modul lain (graph, scoring, dll), bukan untuk searching feature di menu.
int findModuleIndexByCode(const vector<Module>& catalog, const string& code);

#endif
