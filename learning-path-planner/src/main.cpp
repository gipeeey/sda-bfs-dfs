#include <iostream>
#include "module.h"
#include "graph.h"
#include "menu.h"

int main() {
    vector<Module> catalog = getModuleCatalog();
    ModuleGraph graph = buildGraph(catalog);

    // Edge case #5: deteksi cycle di data modul saat startup (defensive check).
    if (hasCycle(graph, catalog)) {
        cout << "PERINGATAN: Ditemukan cycle pada data prasyarat modul. "
             << "Periksa kembali definisi katalog modul." << endl;
    }

    runMainMenu(catalog, graph);

    return 0;
}
