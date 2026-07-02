#include "bfs_baseline.h"
#include <queue>

// Kompleksitas: O(V + E) — BFS/Kahn's-style: modul diproses level demi level saat
// semua OR-group prasyaratnya terpenuhi (in-degree efektif 0), tiap modul & edge
// dikunjungi maksimal satu kali.
vector<string> findBaselinePath(const set<string>& requiredModules, const set<string>& mastered,
                                 const ModuleGraph& graph) {
    vector<string> path;
    set<string> fulfilled = mastered;
    set<string> remaining = requiredModules;

    // Kahn's-style: proses modul yang ready satu per satu, karena "ready" bisa
    // berubah setelah modul lain diproses, ulangi scan requiredModules sampai
    // tidak ada modul baru yang bisa diproses.
    bool progress = true;
    while (progress && !remaining.empty()) {
        progress = false;
        for (auto it = remaining.begin(); it != remaining.end(); ) {
            if (isModuleReady(*it, fulfilled, graph)) {
                path.push_back(*it);
                fulfilled.insert(*it);
                it = remaining.erase(it);
                progress = true;
            } else {
                ++it;
            }
        }
    }

    return path;
}
