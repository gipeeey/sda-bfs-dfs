# SDA BFS/DFS — Smart Learning Path Recommender

Tugas IF204 Struktur Data & Algoritma. Program console C++ rekomendasi jalur belajar modul pakai Graph/DAG, BFS, DFS, merge sort, search, dan rule-based expert system.

## Struktur Repo

```
.
├── learning-path-planner/       # source code utama (C++)
│   ├── src/
│   │   ├── main.cpp             # entry point
│   │   ├── module.h/.cpp        # data & katalog modul
│   │   ├── graph.h/.cpp         # ModuleGraph (DAG), cycle detection
│   │   ├── bfs_baseline.h/.cpp  # baseline path via BFS/Kahn's algorithm
│   │   ├── dfs_path_enum.h/.cpp # enumerasi jalur alternatif via DFS
│   │   ├── merge_sort.h/.cpp    # sorting modul
│   │   ├── search_module.h/.cpp # linear & binary search modul
│   │   ├── ai_scoring.h/.cpp    # rule-based scoring rekomendasi
│   │   ├── student_profile.h/.cpp # profil mahasiswa (skill, budget waktu, dll)
│   │   └── menu.h/.cpp          # menu console interaktif
│   └── README.md                # instruksi compile & run
├── Materi_SDA_IF204.md          # materi/rangkuman konsep SDA
└── Penjelasan_Code_by_Code_IF204.md  # penjelasan code baris-per-baris
```

## Fitur Utama

1. Tampilkan katalog modul (tabel).
2. Cari modul (linear/binary search).
3. Pilih/atur profil mahasiswa (sample atau input manual).
4. Rekomendasi jalur belajar lengkap: BFS baseline path + DFS alternative paths + scoring, dengan penanganan edge case (target sudah dikuasai, target invalid, budget waktu tidak cukup).
5. Tampilkan struktur graph (DAG modul & prasyarat) dalam teks.
6. Deteksi cycle otomatis saat startup (defensive check terhadap data prasyarat modul).

## Compile & Jalankan

**Windows (MSYS2 + MinGW-w64, native g++):**

```
g++ -std=c++17 -Wall -Wextra -o learning_path_planner.exe learning-path-planner\src\*.cpp
learning_path_planner.exe
```

**Linux/WSL:**

```
g++ -std=c++17 -Wall -Wextra -o learning_path_planner learning-path-planner/src/*.cpp
./learning_path_planner
```

**macOS (clang via Xcode Command Line Tools atau g++ dari Homebrew):**

```
clang++ -std=c++17 -Wall -Wextra -o learning_path_planner learning-path-planner/src/*.cpp
./learning_path_planner
```

## Dokumentasi Tambahan

- `Materi_SDA_IF204.md` — rangkuman materi konsep (graph, BFS, DFS, sorting, searching).
- `Penjelasan_Code_by_Code_IF204.md` — penjelasan implementasi code secara detail.
