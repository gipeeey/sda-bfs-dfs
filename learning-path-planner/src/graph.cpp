#include "graph.h"
#include <iostream>

// Kompleksitas: O(V + E) — setiap vertex dan edge dikunjungi maksimal satu kali.
ModuleGraph buildGraph(const vector<Module>& catalog) {
    ModuleGraph graph;

    for (size_t i = 0; i < catalog.size(); i++) {
        graph.prereqAdjacency[catalog[i].code] = catalog[i].prereqGroups;
    }

    return graph;
}

// Kompleksitas: O(V + E) — DFS dengan visited + inStack array.
static bool hasCycleDFS(const string& code, const ModuleGraph& graph,
                         set<string>& visited, set<string>& inStack) {
    visited.insert(code);
    inStack.insert(code);

    auto it = graph.prereqAdjacency.find(code);
    if (it != graph.prereqAdjacency.end()) {
        for (const vector<string>& group : it->second) {
            for (const string& prereq : group) {
                if (inStack.count(prereq)) {
                    return true;
                }
                if (!visited.count(prereq)) {
                    if (hasCycleDFS(prereq, graph, visited, inStack)) {
                        return true;
                    }
                }
            }
        }
    }

    inStack.erase(code);
    return false;
}

bool hasCycle(const ModuleGraph& graph, const vector<Module>& catalog) {
    set<string> visited;
    set<string> inStack;

    for (size_t i = 0; i < catalog.size(); i++) {
        if (!visited.count(catalog[i].code)) {
            if (hasCycleDFS(catalog[i].code, graph, visited, inStack)) {
                return true;
            }
        }
    }
    return false;
}

bool isModuleReady(const string& moduleCode, const set<string>& mastered, const ModuleGraph& graph) {
    auto it = graph.prereqAdjacency.find(moduleCode);
    if (it == graph.prereqAdjacency.end() || it->second.empty()) {
        return true;
    }

    for (const vector<string>& group : it->second) {
        bool allMastered = true;
        for (const string& prereq : group) {
            if (!mastered.count(prereq)) {
                allMastered = false;
                break;
            }
        }
        if (allMastered) {
            return true;
        }
    }
    return false;
}

// Kompleksitas: O(V + E) — DFS backward dari target, setiap modul & prasyaratnya
// dikunjungi maksimal satu kali (ditandai lewat set "required" sebagai visited).
static void collectRequiredDFS(const string& moduleCode, const set<string>& mastered,
                                const ModuleGraph& graph, set<string>& required) {
    auto it = graph.prereqAdjacency.find(moduleCode);
    if (it == graph.prereqAdjacency.end()) {
        return;
    }

    for (const vector<string>& group : it->second) {
        for (const string& prereq : group) {
            if (!mastered.count(prereq) && !required.count(prereq)) {
                required.insert(prereq);
                collectRequiredDFS(prereq, mastered, graph, required);
            }
        }
    }
}

set<string> collectRequiredModules(const string& targetModule, const set<string>& mastered,
                                    const ModuleGraph& graph) {
    set<string> required;

    if (!mastered.count(targetModule)) {
        required.insert(targetModule);
    }
    collectRequiredDFS(targetModule, mastered, graph, required);

    return required;
}

void printGraphStructure(const ModuleGraph& graph, const vector<Module>& catalog) {
    cout << "\n=== Struktur Graph (DAG Modul & Prasyarat) ===" << endl;
    for (size_t i = 0; i < catalog.size(); i++) {
        const string& code = catalog[i].code;
        cout << code << " (" << catalog[i].name << ") -> ";

        auto it = graph.prereqAdjacency.find(code);
        if (it == graph.prereqAdjacency.end() || it->second.empty()) {
            cout << "(tidak ada prasyarat)";
        } else {
            for (size_t g = 0; g < it->second.size(); g++) {
                if (g > 0) cout << " ATAU ";
                cout << "[";
                for (size_t j = 0; j < it->second[g].size(); j++) {
                    if (j > 0) cout << ", ";
                    cout << it->second[g][j];
                }
                cout << "]";
            }
        }
        cout << endl;
    }
}
