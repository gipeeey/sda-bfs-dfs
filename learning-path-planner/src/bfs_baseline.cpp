#include "bfs_baseline.h"
#include <queue>

// Kompleksitas: O(V^2) worst-case -- setiap pop dari queue memicu rescan penuh
// terhadap `remaining` untuk cek modul mana yang baru jadi ready (V pop x O(V)
// rescan). Ini BUKAN Kahn's algorithm O(V+E) versi tekstual (yang butuh
// reverse-adjacency + in-degree counter per edge), tapi tetap functionally
// correct sebagai BFS level-order lewat queue. Untuk ukuran graph project ini
// (V <= 7), overhead O(V^2) vs O(V+E) diabaikan karena selisihnya konstan kecil.
vector<string> findBaselinePath(const set<string>& requiredModules, const set<string>& mastered,
                                 const ModuleGraph& graph) {
    vector<string> path;
    set<string> fulfilled = mastered;
    set<string> remaining = requiredModules;
    queue<string> ready;

    // Inisialisasi: modul yang sudah ready dari awal (in-degree efektif 0) masuk queue.
    for (auto it = remaining.begin(); it != remaining.end(); ) {
        if (isModuleReady(*it, fulfilled, graph)) {
            ready.push(*it);
            it = remaining.erase(it);
        } else {
            ++it;
        }
    }

    while (!ready.empty()) {
        string current = ready.front();
        ready.pop();
        path.push_back(current);
        fulfilled.insert(current);

        // Modul lain yang tadinya belum ready mungkin sekarang terpenuhi.
        for (auto it = remaining.begin(); it != remaining.end(); ) {
            if (isModuleReady(*it, fulfilled, graph)) {
                ready.push(*it);
                it = remaining.erase(it);
            } else {
                ++it;
            }
        }
    }

    return path;
}
