#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <set>
#include <map>
#include "module.h"

using namespace std;

// Adjacency list DAG modul: key = code modul, value = daftar prereqGroups (OR-of-AND).
// Dibangun sekali dari katalog saat startup.
struct ModuleGraph {
    map<string, vector<vector<string>>> prereqAdjacency;
};

// Kompleksitas: O(V + E) — setiap vertex dan edge dikunjungi maksimal satu kali.
// Dijalankan sekali saat startup.
ModuleGraph buildGraph(const vector<Module>& catalog);

// Kompleksitas: O(V + E) — DFS pada seluruh vertex dengan visited + inStack array.
// Defensive check: mendeteksi cycle di data prasyarat modul (seharusnya tidak terjadi
// kalau data valid, tapi wajib ditangani sebagai validasi startup, bukan crash diam-diam).
bool hasCycle(const ModuleGraph& graph, const vector<Module>& catalog);

// Kompleksitas: O(V + E) — DFS backward dari target, kumpulkan seluruh prasyarat
// transitif (semua cabang OR-group) yang belum dikuasai mahasiswa.
// Rekursif penuh: menangani prasyarat berlapis (edge case #4 di CLAUDE.md bagian 9).
set<string> collectRequiredModules(const string& targetModule, const set<string>& mastered,
                                    const ModuleGraph& graph);

// Cek apakah sebuah modul "ready" diambil: SEMUA modul di SALAH SATU prereqGroups
// sudah ada di "mastered". Modul tanpa prereqGroups selalu ready.
bool isModuleReady(const string& moduleCode, const set<string>& mastered, const ModuleGraph& graph);

// Cetak adjacency list DAG dalam bentuk teks (dipakai Menu 5).
void printGraphStructure(const ModuleGraph& graph, const vector<Module>& catalog);

#endif
