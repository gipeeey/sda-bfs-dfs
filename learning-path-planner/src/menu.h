#ifndef MENU_H
#define MENU_H

#include <vector>
#include "module.h"
#include "graph.h"
#include "student_profile.h"

using namespace std;

// Menjalankan loop menu utama console (CLAUDE.md bagian 8).
void runMainMenu(const vector<Module>& catalog, const ModuleGraph& graph);

// Menu 1: tampilkan tabel katalog modul.
void showModuleCatalog(const vector<Module>& catalog);

// Menu 2: cari modul (linear/binary search), tampilkan hasil atau "tidak ditemukan".
void searchModuleMenu(const vector<Module>& catalog);

// Menu 3: pilih profil mahasiswa sample (Dimas/Sari) atau input manual.
StudentProfile selectStudentProfileMenu();

// Menu 4: jalankan pipeline rekomendasi lengkap (CLAUDE.md bagian 6), tampilkan
// decision trace lalu rekomendasi final. Menangani edge case target sudah dikuasai,
// target invalid, dan budget tidak cukup (cari alternatif target feasible).
void recommendLearningPathMenu(const StudentProfile& profile, const vector<Module>& catalog,
                                const ModuleGraph& graph);

// Menu 5: cetak struktur graph (DAG modul & prasyarat) dalam bentuk teks.
void showGraphStructureMenu(const ModuleGraph& graph, const vector<Module>& catalog);

#endif
