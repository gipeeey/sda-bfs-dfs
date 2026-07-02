#ifndef DFS_PATH_ENUM_H
#define DFS_PATH_ENUM_H

#include <string>
#include <vector>
#include <set>
#include "graph.h"
#include "module.h"

using namespace std;

// Satu kandidat jalur belajar hasil enumerasi DFS.
struct PathCandidate {
    vector<string> moduleSequence;
    double totalHours;
    double avgDifficulty;
    int moduleCount;
};

// Kompleksitas: O(2^V) worst-case — DFS rekursif + backtracking: di tiap langkah,
// coba semua modul di requiredModules yang "ready", rekursi, lalu backtrack.
// V = jumlah modul di required set (kecil karena OR-branch terbatas di sample data).
// targetModule dibutuhkan supaya DFS tahu kapan sebuah kandidat jalur "selesai":
// begitu targetModule menjadi ready dan dipilih, jalur itu final (tidak perlu
// menghabiskan seluruh requiredModules) -- inilah yang membuat rute lewat M2 saja,
// M3 saja, atau keduanya (OR-branch M4/M6) muncul sebagai kandidat terpisah.
// Edge case #6: OR-group yang semua opsinya sudah terpenuhi tidak boleh menghasilkan
// kandidat duplikat untuk hal yang sama (lihat CLAUDE.md bagian 9). Dedup dilakukan
// berdasarkan SET modul pada jalur (bukan urutan persis), karena urutan berbeda dari
// himpunan modul yang sama menghasilkan totalHours/avgDifficulty/moduleCount identik.
vector<PathCandidate> enumerateAllPaths(const string& targetModule, const set<string>& requiredModules,
                                        const set<string>& mastered, const ModuleGraph& graph,
                                        const vector<Module>& catalog);

#endif
