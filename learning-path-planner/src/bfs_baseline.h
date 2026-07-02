#ifndef BFS_BASELINE_H
#define BFS_BASELINE_H

#include <string>
#include <vector>
#include <set>
#include "graph.h"

using namespace std;

// Kompleksitas: O(V + E) — BFS/Kahn's-style, tiap modul di requiredSet diproses
// tepat sekali saat semua OR-group prasyaratnya terpenuhi (in-degree efektif 0).
// Mengembalikan SATU urutan valid (baseline), dipakai sebagai pembanding cepat
// terhadap hasil DFS enumeration (bukan hasil akhir rekomendasi).
vector<string> findBaselinePath(const set<string>& requiredModules, const set<string>& mastered,
                                 const ModuleGraph& graph);

#endif
