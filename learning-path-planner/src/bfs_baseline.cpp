#include "bfs_baseline.h"
#include <queue>

// Kompleksitas: O(V + E) — Kahn's algorithm asli memakai queue: modul dengan
// in-degree efektif 0 (semua OR-group prasyaratnya terpenuhi) dimasukkan ke queue,
// setiap modul di-pop & diproses tepat satu kali, lalu memicu pengecekan ulang
// modul lain yang belum masuk queue. Total pekerjaan pengecekan "ready" terikat
// oleh jumlah modul dan jumlah entry prasyarat (edge), bukan V^2 full-scan berulang.
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
