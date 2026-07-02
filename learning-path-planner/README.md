# Smart Learning Path Recommender

Program console C++ yang merekomendasikan jalur belajar modul (Graph/DAG + BFS + DFS +
rule-based expert system) untuk tugas IF204 Struktur Data & Algoritma.

## Compile & Jalankan

**Windows (MSYS2 + MinGW-w64, native g++):**

```
g++ -std=c++17 -Wall -Wextra -o learning_path_planner.exe src\*.cpp
learning_path_planner.exe
```

**Alternatif (WSL):**

```
g++ -std=c++17 -Wall -Wextra -o learning_path_planner src/*.cpp
./learning_path_planner
```
