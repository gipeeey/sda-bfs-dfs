# Penjelasan Code-by-Code: Smart Learning Path Recommender
### Pembedahan seluruh source code project IF204, file demi file, blok demi blok

**Tujuan dokumen:** Menjelaskan **setiap file** dan **setiap blok kode** di repository `sda-bfs-dfs/learning-path-planner`, lengkap dengan alasan *kenapa* kode ditulis seperti itu — bukan sekadar *apa* yang dilakukannya.

**Prasyarat:** Dokumen ini mengasumsikan kalian sudah membaca dokumen "Materi SDA IF204" (konsep graph, BFS, DFS, Big O, searching, sorting). Di sini fokusnya implementasi, bukan teori.

**Status verifikasi:** Seluruh kode di dokumen ini dikutip langsung dari source di repository, sudah di-compile ulang dengan `g++ -std=c++17 -Wall -Wextra` dan hasilnya **bersih (0 error, 0 warning)**. Perilaku program juga sudah diuji dengan profil Dimas & Sari, dan outputnya sesuai.

---

## Daftar Isi

**Bagian A — Fondasi C++ yang Muncul di Semua File**
- A.1 Struktur `.h` vs `.cpp`
- A.2 Include guard
- A.3 `struct`
- A.4 Reference (`&`) dan `const`
- A.5 `static` (fungsi privat file)
- A.6 Struktur data STL: vector, set, map, queue

**Bagian B — File demi File (urut alur program)**
1. `main.cpp` — titik masuk
2. `module.h` / `module.cpp` — data model modul
3. `student_profile.h` / `student_profile.cpp` — data profil mahasiswa
4. `graph.h` / `graph.cpp` — struktur graph + operasi
5. `dfs_path_enum.h` / `dfs_path_enum.cpp` — DFS enumerasi kandidat
6. `bfs_baseline.h` / `bfs_baseline.cpp` — BFS baseline
7. `ai_scoring.h` / `ai_scoring.cpp` — rule-based scoring
8. `merge_sort.h` / `merge_sort.cpp` — merge sort manual
9. `search_module.h` / `search_module.cpp` — linear & binary search
10. `menu.h` / `menu.cpp` — orkestrasi & UI

**Bagian C — Diagram Alur Data Antar-File**

---

# BAGIAN A: Fondasi C++ yang Muncul di Semua File

Sebelum masuk ke tiap file, empat konsep ini muncul berulang di seluruh kode. Pahami sekali di sini, nanti tidak perlu diulang.

## A.1 Struktur `.h` (Header) vs `.cpp` (Source)

Setiap komponen project punya sepasang file:
- File `.h` (header) = **deklarasi/kontrak**: daftar fungsi yang tersedia beserta tipe parameter & return-nya, tapi *tanpa isi*.
- File `.cpp` (source) = **definisi/implementasi**: isi sebenarnya dari fungsi-fungsi itu.

**Kenapa dipisah?** Supaya file lain yang mau memakai suatu fungsi cukup `#include` file `.h`-nya (kontraknya), tanpa perlu tahu detail implementasinya. Ini membuat kode modular: mengubah isi implementasi di `.cpp` tidak memaksa perubahan di file lain, selama kontrak di `.h` tetap sama. Hanya file `.cpp` yang di-compile; file `.h` "ditempel" ke `.cpp` yang membutuhkannya lewat `#include`.

## A.2 Include Guard

Di awal tiap file `.h` selalu ada pola:
```cpp
#ifndef GRAPH_H
#define GRAPH_H
// ...isi header...
#endif
```
Ini **include guard**. Fungsinya mencegah isi header ter-copy dua kali kalau dua file `.cpp` berbeda sama-sama meng-`#include` header yang sama (yang bisa menyebabkan error "redefinition"). Ini pola standar, ada di semua header project kalian.

## A.3 `struct`

`struct` adalah cara membuat tipe data gabungan — beberapa variabel dibungkus jadi satu unit. Contoh:
```cpp
struct StudentProfile {
    string name;
    set<string> masteredModules;
    string targetModule;
    double timeBudgetHours;
};
```
Ini mendefinisikan tipe baru bernama `StudentProfile`. Satu variabel bertipe ini menyimpan empat data sekaligus (nama, modul dikuasai, target, budget). Diakses dengan titik: `p.name`, `p.targetModule`, dst.

## A.4 Reference (`&`) dan `const`

Tanda `&` setelah tipe parameter membuat **reference** — parameter menunjuk ke variabel asli, bukan menyalinnya.

```cpp
void printGraphStructure(const ModuleGraph& graph, const vector<Module>& catalog);
```

- Tanpa `&`: setiap pemanggilan fungsi menyalin (copy) seluruh isi `graph` dan `catalog` — boros memori & waktu kalau datanya besar.
- Dengan `&`: fungsi hanya "meminjam" data asli, tanpa menyalin.
- `const ... &`: meminjam **dan** berjanji tidak mengubahnya (read-only).

Pola `const X&` dipakai di hampir semua fungsi project kalian yang menerima `catalog` atau `graph` — ini best practice: kalau fungsi cuma perlu membaca data, jangan salin, jangan izinkan diubah. Sebaliknya, reference **tanpa** `const` (contoh `set<string>& fulfilled`) dipakai secara sengaja ketika fungsi memang perlu mengubah variabel asli si pemanggil.

## A.5 `static` (Fungsi Privat File)

Ketika `static` dipakai pada fungsi bebas (bukan di dalam class), artinya fungsi itu hanya bisa dipanggil dari dalam file `.cpp` yang sama. File lain tidak bisa melihatnya. Ini dipakai untuk *helper internal* yang tidak perlu diekspos. Ciri khasnya: fungsi `static` **tidak** dideklarasikan di file `.h` pasangannya. Contoh di project kalian: `setKey()`, `buildCandidate()`, `minTotalHours()`, `filterByBudget()`.

## A.6 Struktur Data STL yang Dipakai

STL = Standard Template Library, kumpulan struktur data bawaan C++. Requirement tugas mengizinkan container STL (`vector`, `map`, `set`, `queue`) tapi melarang fungsi sort/search bawaan.

| Struktur | Sifat | Operasi kunci di kode kalian |
|---|---|---|
| `vector<T>` | Array dinamis (ukuran bisa berubah) | `.push_back(x)` tambah, `.size()` jumlah, `.pop_back()` hapus akhir, `[i]` akses index |
| `set<T>` | Kumpulan unik & otomatis terurut | `.count(x)` cek ada/tidak, `.insert(x)`, `.erase(x)` |
| `map<K,V>` | Pasangan key-value (seperti dictionary) | `map[key]` akses/set, `.find(key)` cari aman |
| `queue<T>` | Antrian FIFO (first in first out) | `.push(x)`, `.front()` lihat depan, `.pop()` buang depan, `.empty()` |

**`vector<vector<string>>`** (dobel vector) muncul di prasyarat modul — ini "daftar dari daftar", dipakai untuk model OR-of-AND: daftar grup, tiap grup adalah daftar kode modul.

---

# BAGIAN B: File demi File

Urutan pembahasan mengikuti **alur eksekusi program**, bukan abjad — supaya kalian melihat bagaimana data mengalir dari satu bagian ke bagian lain.

---

## 1. `main.cpp` — Titik Masuk Program

```cpp
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
```

**Peran file:** titik awal program. `int main()` adalah fungsi wajib di setiap program C++ — dari sinilah eksekusi dimulai.

**Blok demi blok:**

| Baris | Apa yang terjadi | Kenapa |
|---|---|---|
| `getModuleCatalog()` | Membuat daftar 7 modul (data hardcoded) | Data sumber sistem; dibahas di file `module.cpp` |
| `buildGraph(catalog)` | Mengubah katalog jadi struktur graph (adjacency list) | Supaya prasyarat bisa ditelusuri efisien |
| `hasCycle(...)` | Cek apakah data prasyarat mengandung cycle | *Defensive check* — dibahas di bawah |
| `runMainMenu(...)` | Masuk ke loop menu interaktif | Titik kendali diserahkan ke user |
| `return 0;` | Program selesai dengan status "sukses" | Konvensi C++: 0 = tidak ada error |

**Poin desain penting — defensive check:** Fungsi `hasCycle()` dipanggil saat startup meski data prasyarat kalian hardcoded (jadi "seharusnya" tidak mungkin ada cycle). Ini praktik yang baik: **jangan berasumsi data selalu benar, validasi dulu.** Kalau suatu saat ada yang salah mengedit katalog dan tak sengaja membuat cycle (misal M6 butuh M7 dan M7 butuh M6), program memberi peringatan jelas alih-alih berperilaku aneh diam-diam. Untuk makalah, ini contoh konkret penerapan DFS (cycle detection) di luar fitur utama.

---

## 2. `module.h` / `module.cpp` — Data Model Modul

### 2.1 `module.h` — Definisi Tipe & Kontrak

```cpp
struct Module {
    string code;                          // "M1", "M2", ...
    string name;                          // nama modul
    vector<vector<string>> prereqGroups;  // OR-of-AND prasyarat
    double estimatedHours;                // estimasi jam belajar
    int difficulty;                       // tingkat kesulitan 1-10
};

vector<Module> getModuleCatalog();
int findModuleIndexByCode(const vector<Module>& catalog, const string& code);
```

**Field `prereqGroups` bertipe `vector<vector<string>>`** — ini implementasi konkret model OR-of-AND:
- Vector terluar = daftar **grup** (antar-grup hubungannya OR)
- Vector dalam = daftar **modul dalam satu grup** (di dalam grup hubungannya AND)

Contoh nilai `{{"M4"}, {"M2", "M3"}}` berarti: grup 1 = `[M4]`, grup 2 = `[M2, M3]`. Modul ready kalau M4 dikuasai (grup 1 lengkap) ATAU M2 dan M3 dikuasai (grup 2 lengkap).

### 2.2 `module.cpp` — Data & Implementasi

```cpp
vector<Module> getModuleCatalog() {
    vector<Module> catalog;

    catalog.push_back({"M1", "Dasar Pemrograman & Logika", {}, 4.0, 2});
    catalog.push_back({"M2", "Array & Struktur Data Linear", {{"M1"}}, 5.0, 3});
    catalog.push_back({"M3", "Rekursi", {{"M1"}}, 6.0, 4});
    catalog.push_back({"M4", "Linked List, Stack & Queue", {{"M2"}, {"M3"}}, 6.0, 4});
    catalog.push_back({"M5", "Searching & Sorting Dasar", {{"M4"}}, 7.0, 5});
    catalog.push_back({"M6", "Pohon (Tree) & Struktur Hierarkis", {{"M4"}, {"M2", "M3"}}, 8.0, 6});
    catalog.push_back({"M7", "Graph & Algoritma Traversal", {{"M5", "M6"}}, 10.0, 8});

    return catalog;
}
```

**Membaca tiap baris (ini penting untuk memahami seluruh perilaku program):**

| Modul | prereqGroups | Arti |
|---|---|---|
| M1 | `{}` | Tidak ada prasyarat — bisa langsung diambil |
| M2 | `{{"M1"}}` | Butuh M1 |
| M3 | `{{"M1"}}` | Butuh M1 |
| M4 | `{{"M2"}, {"M3"}}` | Butuh M2 **atau** M3 (dua jalur berbeda) |
| M5 | `{{"M4"}}` | Butuh M4 |
| M6 | `{{"M4"}, {"M2","M3"}}` | Butuh M4 **atau** (M2 **dan** M3) |
| M7 | `{{"M5","M6"}}` | Butuh M5 **dan** M6 (wajib keduanya) |

**Kenapa struktur data ini menghasilkan banyak jalur?** Perhatikan M4 dan M6 punya lebih dari satu opsi grup. Inilah sumber keragaman jalur: menuju M7, kalian bisa lewat M2→M4, atau M3→M4, atau kombinasi lain. Tanpa opsi ganda ini, hanya ada satu jalur dan sistem rekomendasi kehilangan maknanya.

**`.push_back({...})`** menambahkan satu `Module` ke akhir vector. Isi kurung kurawal `{...}` diisi berurutan sesuai definisi struct: code, name, prereqGroups, estimatedHours, difficulty.

```cpp
int findModuleIndexByCode(const vector<Module>& catalog, const string& code) {
    for (size_t i = 0; i < catalog.size(); i++) {
        if (catalog[i].code == code) {
            return static_cast<int>(i);
        }
    }
    return -1;
}
```

**Fungsi ini** mencari posisi (index) sebuah modul di dalam vector berdasarkan kode. Loop dari awal, kalau ketemu return index-nya; kalau habis tak ketemu return `-1`.

- `size_t` adalah tipe bilangan bulat tak-negatif yang dipakai `.size()`. `static_cast<int>(i)` mengubahnya ke `int` biasa agar cocok dengan tipe return dan menghindari warning perbandingan tipe.
- Konvensi return `-1` untuk "tidak ditemukan" adalah pola umum — memungkinkan pemanggil mengecek `if (idx == -1)` alih-alih program crash.
- **Catatan:** ini adalah linear search, tapi dipakai sebagai *helper internal* (dipanggil banyak fungsi lain), **bukan** fitur "Cari Modul" yang dilihat user. Fitur search untuk user ada di file terpisah `search_module.cpp` (poin 9). Pemisahan ini disengaja — helper internal tidak perlu fleksibilitas fitur user.

---

## 3. `student_profile.h` / `student_profile.cpp` — Data Profil Mahasiswa

### 3.1 `student_profile.h`

```cpp
struct StudentProfile {
    string name;
    set<string> masteredModules;   // modul yang SUDAH dikuasai
    string targetModule;           // modul yang INGIN dicapai
    double timeBudgetHours;        // budget waktu belajar
};

StudentProfile getSampleProfileDimas();
StudentProfile getSampleProfileSari();
```

**`masteredModules` bertipe `set<string>`, bukan `vector`.** Kenapa `set`? Karena: (1) tidak masuk akal satu modul dikuasai "dua kali" — set otomatis menjamin unik; (2) pengecekan "apakah modul X sudah dikuasai?" lewat `.count()` di set lebih efisien dan ekspresif daripada mencari manual di vector.

### 3.2 `student_profile.cpp`

```cpp
StudentProfile getSampleProfileDimas() {
    StudentProfile p;
    p.name = "Dimas";
    p.masteredModules = {"M1"};
    p.targetModule = "M7";
    p.timeBudgetHours = 45.0;
    return p;
}

StudentProfile getSampleProfileSari() {
    StudentProfile p;
    p.name = "Sari";
    p.masteredModules = {"M1", "M2", "M3"};
    p.targetModule = "M7";
    p.timeBudgetHours = 20.0;
    return p;
}
```

**Dua profil ini adalah skenario uji yang dirancang untuk menunjukkan dua perilaku berbeda:**

| Profil | Sudah dikuasai | Target | Budget | Skenario yang diuji |
|---|---|---|---|---|
| **Dimas** | M1 | M7 | 45 jam | Budget cukup → menghasilkan banyak kandidat yang bisa dibandingkan (kasus utama) |
| **Sari** | M1, M2, M3 | M7 | 20 jam | Budget tidak cukup → memicu edge case "target tak terjangkau" + saran alternatif |

Ini bukan data acak. Dimas dirancang supaya sistem menghasilkan beberapa kandidat (menunjukkan proses skoring bekerja), sementara Sari dirancang supaya budgetnya sengaja kurang (menunjukkan sistem menangani kasus gagal dengan anggun, bukan crash). Untuk makalah, dua profil ini adalah bukti bahwa sistem diuji terhadap kasus normal *dan* kasus batas.

---

## 4. `graph.h` / `graph.cpp` — Struktur Graph & Operasinya

Ini file paling padat konsep. Kita bahas per fungsi.

### 4.1 Struktur Graph & `buildGraph()`

```cpp
struct ModuleGraph {
    map<string, vector<vector<string>>> prereqAdjacency;
};

ModuleGraph buildGraph(const vector<Module>& catalog) {
    ModuleGraph graph;
    for (size_t i = 0; i < catalog.size(); i++) {
        graph.prereqAdjacency[catalog[i].code] = catalog[i].prereqGroups;
    }
    return graph;
}
```

**`prereqAdjacency` adalah adjacency list**, diimplementasikan sebagai `map`: key = kode modul (string), value = prereqGroups modul itu. Setelah `buildGraph`, memanggil `graph.prereqAdjacency["M7"]` langsung mengembalikan `{{"M5","M6"}}`.

**Kenapa `map`, bukan langsung pakai vector katalog?** Karena `map` memberi akses cepat berdasarkan **kode** (`graph[code]`), tanpa perlu loop mencari modul. Banyak fungsi di bawah butuh "ambil prasyarat modul X" berulang kali — `map` membuat operasi ini efisien dan kodenya bersih.

**Kompleksitas:** O(V + E). Loop menyentuh tiap modul (V) sekali, dan menyalin prereqGroups (total E edge) sekali.

### 4.2 Deteksi Cycle — DFS #1 (`hasCycleDFS` + `hasCycle`)

```cpp
static bool hasCycleDFS(const string& code, const ModuleGraph& graph,
                         set<string>& visited, set<string>& inStack) {
    visited.insert(code);
    inStack.insert(code);

    auto it = graph.prereqAdjacency.find(code);
    if (it != graph.prereqAdjacency.end()) {
        for (const vector<string>& group : it->second) {
            for (const string& prereq : group) {
                if (inStack.count(prereq)) {
                    return true;   // menuju node di jalur aktif = CYCLE
                }
                if (!visited.count(prereq)) {
                    if (hasCycleDFS(prereq, graph, visited, inStack)) {
                        return true;
                    }
                }
            }
        }
    }

    inStack.erase(code);   // selesai eksplorasi node ini
    return false;
}
```

**Ini implementasi DFS untuk deteksi cycle.** Kuncinya adalah **dua set berbeda**:

- **`visited`** = "node ini pernah saya kunjungi, kapan pun." Bersifat permanen.
- **`inStack`** = "node ini sedang ada di jalur rekursi saya SEKARANG." Bersifat sementara.

**Logika deteksi cycle:** kalau saat menelusuri kita menemukan node yang masih ada di `inStack` (jalur aktif saat ini), berarti kita memutar balik ke leluhur sendiri → itu cycle. Kalau node cuma ada di `visited` (pernah dikunjungi lewat jalur lain, tapi bukan jalur aktif sekarang), itu **bukan** cycle — cuma dua jalur menuju node yang sama, yang sah di DAG.

**Kenapa butuh dua set, tidak cukup satu `visited`?** Kalau cuma pakai `visited`, program tidak bisa membedakan "sudah pernah ke sini via jalur lain" (aman) dari "sedang memutar balik ke diri sendiri" (cycle). `inStack` yang di-`insert` saat masuk dan di-`erase` saat keluar (baris `inStack.erase(code)`) itulah yang melacak "jalur aktif saat ini".

**Base case & recursive case (ciri rekursi):**
- Base case: kalau `prereq` sudah `visited`, tidak rekursi (berhenti menelusuri cabang itu).
- Recursive case: kalau belum, `hasCycleDFS()` memanggil dirinya untuk menelusuri prereq lebih dalam.

```cpp
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
```

**Fungsi pembungkus** ini memanggil `hasCycleDFS` untuk setiap modul yang belum dikunjungi. Loop luar ini penting karena graph bisa "terputus" (tidak semua modul terhubung ke satu titik awal) — memulai DFS dari tiap node memastikan seluruh graph tercakup. `hasCycleDFS` bersifat `static` (privat file) karena hanya `hasCycle` yang memanggilnya; `hasCycle` sendiri publik (ada di `.h`) karena dipanggil `main.cpp`.

**Kompleksitas:** O(V + E) — tiap node & edge disentuh maksimal sekali berkat penanda `visited`.

### 4.3 Cek Modul Ready — Implementasi Logika OR-of-AND (`isModuleReady`)

```cpp
bool isModuleReady(const string& moduleCode, const set<string>& mastered, const ModuleGraph& graph) {
    auto it = graph.prereqAdjacency.find(moduleCode);
    if (it == graph.prereqAdjacency.end() || it->second.empty()) {
        return true;   // tidak ada prasyarat = selalu ready
    }

    for (const vector<string>& group : it->second) {
        bool allMastered = true;
        for (const string& prereq : group) {
            if (!mastered.count(prereq)) {
                allMastered = false;
                break;
            }
        }
        if (allMastered) return true;   // satu grup lengkap = cukup (OR)
    }
    return false;
}
```

**Ini fungsi paling sering dipanggil di seluruh project** — inti logika "modul mana yang boleh diambil sekarang". Struktur loop-nya persis mencerminkan definisi OR-of-AND:

- **Loop luar** (`for group : ...`) menelusuri tiap grup. Hubungan antar-grup adalah **OR** → cukup satu grup terpenuhi.
- **Loop dalam** (`for prereq : group`) menelusuri tiap modul dalam grup. Hubungan dalam grup adalah **AND** → semua harus terpenuhi.

**Alur logika:**
1. Kalau modul tidak punya prasyarat (`empty()`), langsung `return true`.
2. Untuk tiap grup, asumsikan `allMastered = true`, lalu cek tiap anggotanya. Kalau ada satu yang belum dikuasai, set `allMastered = false` dan `break` (tak perlu cek sisanya di grup ini).
3. Kalau ada satu grup dengan `allMastered == true`, langsung `return true` (short-circuit — begitu satu jalur terpenuhi, tak perlu cek grup lain).
4. Kalau semua grup gagal, `return false`.

**`break` dan return dini** di sini bukan sekadar gaya — keduanya menghindari pekerjaan sia-sia. Begitu jawaban sudah pasti, berhenti.

### 4.4 Kumpulkan Semua Prasyarat — DFS #2 (`collectRequiredDFS` + `collectRequiredModules`)

```cpp
static void collectRequiredDFS(const string& moduleCode, const set<string>& mastered,
                                const ModuleGraph& graph, set<string>& required) {
    auto it = graph.prereqAdjacency.find(moduleCode);
    if (it == graph.prereqAdjacency.end()) return;

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
```

**DFS ketiga, tapi tujuannya beda dari cycle detection.** Fungsi ini menelusuri mundur dari target dan mengumpulkan **semua modul yang mungkin dibutuhkan**, dari **semua cabang OR**.

**Kenapa mengumpulkan semua cabang OR, bukan cuma satu?** Karena di titik ini program belum memutuskan jalur mana yang akan dipilih — keputusan itu baru dibuat nanti oleh DFS enumerasi (poin 5). Fungsi ini cuma menjawab: "modul apa saja yang *berpotensi* relevan?" Jawabannya harus mencakup semua opsi. Contoh untuk M6: baik M4 maupun M2+M3 dikumpulkan, karena keduanya jalur valid yang perlu dipertimbangkan.

**Peran `required` sebagai penanda visited:** kondisi `&& !required.count(prereq)` mencegah modul yang sudah dikumpulkan diproses ulang — ini yang mencegah rekursi tak berujung dan menjaga kompleksitas tetap O(V + E).

**`set<string>& required` (tanpa const)** adalah *output parameter* — alih-alih fungsi me-return set baru tiap rekursi (boros), ia langsung menambahkan hasil ke set yang "dipinjamkan" pemanggil. Ini pola efisien untuk fungsi rekursif yang mengakumulasi hasil.

### 4.5 Cetak Struktur Graph (`printGraphStructure`)

```cpp
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
```

**Fungsi ini murni presentasi** — mencetak adjacency list dalam format yang bisa dibaca manusia (dipakai Menu 5). Yang perlu diperhatikan: pola `if (g > 0) cout << " ATAU "` dan `if (j > 0) cout << ", "` adalah teknik umum untuk mencetak pemisah **di antara** elemen tanpa pemisah menggantung di awal/akhir. Grup dipisah dengan "ATAU" (mencerminkan OR), anggota grup dipisah dengan koma (mencerminkan AND dalam satu kurung siku). Output ini secara langsung memvisualkan model OR-of-AND kalian ke user.

---

## 5. `dfs_path_enum.h` / `dfs_path_enum.cpp` — DFS Enumerasi Kandidat (Inti Sistem)

Ini komponen paling penting untuk requirement "studi kasus AI". Di sinilah program mengeksplorasi berbagai kemungkinan jalur.

### 5.1 Struktur `PathCandidate` & Helper

```cpp
struct PathCandidate {
    vector<string> moduleSequence;  // urutan modul yang diambil
    double totalHours;              // total jam seluruh modul di jalur
    double avgDifficulty;           // rata-rata kesulitan
    int moduleCount;                // jumlah modul
};
```
Satu `PathCandidate` merepresentasikan satu jalur belajar lengkap beserta statistiknya. Statistik ini (totalHours, avgDifficulty, moduleCount) nanti dipakai untuk skoring.

```cpp
static PathCandidate buildCandidate(const vector<string>& sequence, const vector<Module>& catalog) {
    PathCandidate candidate;
    candidate.moduleSequence = sequence;
    candidate.moduleCount = static_cast<int>(sequence.size());

    double totalHours = 0.0;
    int totalDifficulty = 0;
    for (const string& code : sequence) {
        int idx = findModuleIndexByCode(catalog, code);
        if (idx != -1) {
            totalHours += catalog[idx].estimatedHours;
            totalDifficulty += catalog[idx].difficulty;
        }
    }
    candidate.totalHours = totalHours;
    candidate.avgDifficulty = sequence.empty() ? 0.0 : static_cast<double>(totalDifficulty) / sequence.size();

    return candidate;
}
```
**Helper ini menghitung statistik** sebuah urutan: menjumlahkan jam & kesulitan tiap modul (mencarinya di katalog via `findModuleIndexByCode`), lalu menghitung rata-rata kesulitan. Perhatikan `sequence.empty() ? 0.0 : ...` — ini *ternary operator*, pengaman agar tidak terjadi pembagian dengan nol kalau sequence kosong.

```cpp
static string setKey(const vector<string>& sequence) {
    set<string> asSet(sequence.begin(), sequence.end());
    string key;
    for (const string& code : asSet) {
        key += code;
        key += ",";
    }
    return key;
}
```
**Helper dedup.** Mengubah `sequence` (vector, urutan penting) menjadi `set` (otomatis terurut, urutan hilang), lalu menggabungnya jadi satu string "sidik jari". Tujuannya: dua urutan berbeda dari **himpunan modul yang sama** menghasilkan sidik jari yang sama — sehingga tidak dihitung dua kali sebagai kandidat berbeda. Alasannya: himpunan modul yang sama pasti punya totalHours/avgDifficulty/moduleCount identik, jadi secara skoring memang kandidat yang sama.

### 5.2 Fungsi DFS Backtracking (`dfsEnumerate`)

```cpp
static void dfsEnumerate(const string& targetModule, const set<string>& requiredModules,
                          set<string>& fulfilled, vector<string>& sequence,
                          const ModuleGraph& graph, const vector<Module>& catalog,
                          vector<PathCandidate>& candidates, set<string>& seenSets) {
    for (const string& code : requiredModules) {
        if (fulfilled.count(code)) continue;
        if (!isModuleReady(code, fulfilled, graph)) continue;

        sequence.push_back(code);       // (1) PILIH modul ini
        fulfilled.insert(code);

        if (code == targetModule) {     // (2a) sampai target -> simpan kandidat
            string key = setKey(sequence);
            if (!seenSets.count(key)) {
                seenSets.insert(key);
                candidates.push_back(buildCandidate(sequence, catalog));
            }
        } else {                        // (2b) belum sampai -> telusuri lebih dalam
            dfsEnumerate(targetModule, requiredModules, fulfilled, sequence, graph, catalog,
                         candidates, seenSets);
        }

        fulfilled.erase(code);          // (3) BATALKAN pilihan (backtrack)
        sequence.pop_back();
    }
}
```

**Ini pola backtracking klasik.** Tiga fase yang menandai backtracking:

1. **PILIH** — `sequence.push_back(code)` + `fulfilled.insert(code)`: masukkan modul ini ke jalur yang sedang dibangun, tandai sudah terpenuhi.
2. **EKSPLORASI** — kalau modul ini adalah target, simpan jalur sebagai kandidat (setelah cek duplikat via `seenSets`). Kalau belum, rekursi untuk mencoba modul berikutnya.
3. **BATALKAN (backtrack)** — `fulfilled.erase(code)` + `sequence.pop_back()`: kembalikan kondisi seperti sebelum modul ini dipilih, supaya iterasi loop berikutnya bisa mencoba modul **lain** dari titik yang sama.

**Kenapa fase 3 wajib?** Tanpa `erase` dan `pop_back`, `fulfilled` dan `sequence` akan terus menumpuk semua modul yang pernah dicoba — bukan cuma yang ada di jalur saat ini. Hasilnya akan salah total. Analogi: seperti mencoba baju untuk difoto satu per satu — kalian harus **melepas** baju sebelumnya sebelum mencoba yang berikutnya, bukan menumpuk semua.

**Dua filter di awal loop:**
- `if (fulfilled.count(code)) continue;` — lewati modul yang sudah ada di jalur (jangan ambil dua kali).
- `if (!isModuleReady(code, fulfilled, graph)) continue;` — lewati modul yang prasyaratnya belum terpenuhi (belum boleh diambil sekarang).

**Bagaimana ini menghasilkan banyak kandidat:** karena loop mencoba **setiap** modul yang ready (bukan cuma satu) lalu backtrack, kombinasi jalur yang berbeda-beda terbentuk. Untuk Dimas, ini menghasilkan 3 kandidat berbeda menuju M7. Inilah bedanya dengan topological sort biasa yang cuma memberi satu urutan.

### 5.3 Fungsi Publik (`enumerateAllPaths`)

```cpp
vector<PathCandidate> enumerateAllPaths(const string& targetModule, const set<string>& requiredModules,
                                        const set<string>& mastered, const ModuleGraph& graph,
                                        const vector<Module>& catalog) {
    vector<PathCandidate> candidates;
    set<string> fulfilled = mastered;   // mulai dari yang sudah dikuasai
    vector<string> sequence;
    set<string> seenSets;

    dfsEnumerate(targetModule, requiredModules, fulfilled, sequence, graph, catalog, candidates, seenSets);
    return candidates;
}
```
**Fungsi pembungkus** yang menyiapkan state awal lalu memanggil rekursi. Perhatikan `fulfilled = mastered` — pencarian dimulai dari kondisi "modul yang sudah dikuasai mahasiswa", bukan dari nol. Jadi modul yang sudah dikuasai tidak muncul lagi di jalur rekomendasi.

**Kompleksitas O(2ⁿ) worst-case:** karena backtracking secara teori bisa mengeksplorasi semua kombinasi subset dari modul. Untuk 7 modul itu maksimal 128 kemungkinan — sangat murah. **Trade-off untuk makalah:** pendekatan exhaustive ini mahal secara teori (eksponensial) tapi acceptable pada skala kecil. Kalau katalog tumbuh jadi puluhan/ratusan modul, pendekatan ini akan lambat dan butuh strategi lain (pruning lebih agresif, atau algoritma berbeda). Menyebut trade-off ini menunjukkan pemahaman, bukan sekadar implementasi.

---

## 6. `bfs_baseline.h` / `bfs_baseline.cpp` — BFS Baseline

```cpp
vector<string> findBaselinePath(const set<string>& requiredModules, const set<string>& mastered,
                                 const ModuleGraph& graph) {
    vector<string> path;
    set<string> fulfilled = mastered;
    set<string> remaining = requiredModules;
    queue<string> ready;

    // Fase 1: masukkan modul yang sudah ready dari awal ke queue
    for (auto it = remaining.begin(); it != remaining.end(); ) {
        if (isModuleReady(*it, fulfilled, graph)) {
            ready.push(*it);
            it = remaining.erase(it);
        } else {
            ++it;
        }
    }

    // Fase 2: proses queue level demi level
    while (!ready.empty()) {
        string current = ready.front();
        ready.pop();
        path.push_back(current);
        fulfilled.insert(current);

        // modul yang tadinya belum ready mungkin sekarang jadi ready
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
```

**Ini implementasi BFS**, ditandai penggunaan `queue<string> ready`. Alurnya:

1. **Fase 1** — cari semua modul yang sudah ready dari awal (prasyaratnya sudah terpenuhi tanpa perlu modul lain di required set), masukkan ke queue.
2. **Fase 2** — ambil modul dari depan queue (`.front()` lalu `.pop()`), masukkan ke path, tandai fulfilled. Lalu scan ulang `remaining`: modul yang tadinya belum ready, apakah sekarang jadi ready karena prasyaratnya baru terpenuhi? Kalau ya, masuk queue.
3. Ulangi sampai queue kosong.

**Hasilnya SATU urutan valid**, dipakai sebagai *baseline* (pembanding cepat), **bukan** rekomendasi akhir. Dengan menampilkan baseline BFS di samping hasil DFS+skoring, sistem bisa menunjukkan bahwa rekomendasi akhirnya memang lebih dipertimbangkan daripada jalur naif.

**Detail teknis — iterator saat menghapus:** pola `it = remaining.erase(it)` vs `++it` penting. Saat menghapus elemen dari `set` di tengah loop, `erase` mengembalikan iterator ke elemen berikutnya yang valid — jadi kita tidak menaikkan `it` secara manual di cabang itu. Kalau tidak, iterator bisa jadi invalid dan program crash. Ini detail C++ yang sering jadi bug bagi pemula, dan di kode kalian sudah ditangani benar.

**Catatan jujur soal kompleksitas (dari komentar kode kalian sendiri):** ini **bukan** Kahn's algorithm murni. Kahn's algorithm standar memakai penghitung *in-degree* per node dan reverse-adjacency untuk mencapai O(V+E). Versi kalian melakukan *rescan penuh* `remaining` setiap kali satu modul selesai — jadi kompleksitasnya realistis **O(V²)**. Untuk 7 modul, perbedaan ini tidak terasa. **Untuk makalah**, ini justru bahan diskusi bagus: kalian bisa jelaskan *kenapa* memilih versi sederhana (lebih mudah dipahami & diimplementasikan dengan benar) dan *apa* trade-off-nya (kurang optimal secara teori, tapi diabaikan karena skala kecil).

---

## 7. `ai_scoring.h` / `ai_scoring.cpp` — Rule-Based Scoring & Decision Trace

**Klarifikasi penting di depan:** ini **bukan** machine learning. Tidak ada training data, tidak ada model yang "belajar". Ini **rule-based expert system** (symbolic AI) — aturan skoring yang dirancang manusia, dihitung dengan rumus tetap. Tetap sah sebagai studi kasus AI, tapi penting dijelaskan bedanya di makalah agar tidak menyesatkan.

### 7.1 Struktur & Formula Skoring

```cpp
struct ScoredCandidate {
    PathCandidate candidate;
    double efficiencyScore;
    double difficultyScore;
    double concisenessScore;
    double finalScore;
};

static double clampScore(double value) {
    if (value < 0.0) return 0.0;
    if (value > 10.0) return 10.0;
    return value;
}

ScoredCandidate calculateScore(const PathCandidate& candidate, double timeBudget,
                                int minModuleCountAmongCandidates) {
    ScoredCandidate scored;
    scored.candidate = candidate;

    scored.efficiencyScore = clampScore(10.0 - (candidate.totalHours / timeBudget) * 10.0);
    scored.difficultyScore = clampScore(10.0 - candidate.avgDifficulty);
    scored.concisenessScore = clampScore(10.0 - (candidate.moduleCount - minModuleCountAmongCandidates) * 2.0);

    scored.finalScore = (scored.efficiencyScore * 0.45) + (scored.difficultyScore * 0.35)
                       + (scored.concisenessScore * 0.20);
    return scored;
}
```

**Tiga komponen skor**, semua di skala 0–10 (dijaga oleh `clampScore` yang memotong nilai di bawah 0 jadi 0 dan di atas 10 jadi 10):

| Komponen | Formula | Arti | Makin tinggi kalau... |
|---|---|---|---|
| `efficiencyScore` | `10 - (totalHours/budget)*10` | Efisiensi waktu | Makin sedikit jam dipakai relatif budget |
| `difficultyScore` | `10 - avgDifficulty` | Kemudahan | Makin rendah rata-rata kesulitan |
| `concisenessScore` | `10 - (moduleCount - minCount)*2` | Keringkasan | Makin sedikit modul (relatif kandidat lain) |

**`finalScore` adalah weighted sum** (jumlah berbobot): efisiensi paling berpengaruh (0.45), lalu kesulitan (0.35), lalu keringkasan (0.20). Total bobot = 1.0. Bobot-bobot ini adalah **keputusan desain kalian** — dan karena rule-based, kalian bisa menjelaskan alasan tiap angka di makalah (misalnya: kenapa efisiensi waktu dianggap prioritas tertinggi untuk konteks mahasiswa dengan budget terbatas).

**Detail penting — `concisenessScore` bersifat relatif.** Ia dihitung terhadap `minModuleCountAmongCandidates` (jumlah modul paling sedikit di antara kandidat yang sedang dibandingkan), bukan angka tetap. Jadi kandidat dengan modul paling sedikit dapat skor keringkasan penuh (10), dan tiap modul tambahan mengurangi 2 poin. Ini membuat skor kontekstual per skenario.

### 7.2 Skoring Massal (`calculateScoreForAll`)

```cpp
vector<ScoredCandidate> calculateScoreForAll(const vector<PathCandidate>& candidates, double timeBudget) {
    vector<ScoredCandidate> scoredList;
    if (candidates.empty()) return scoredList;

    int minModuleCount = candidates[0].moduleCount;
    for (size_t i = 1; i < candidates.size(); i++) {
        if (candidates[i].moduleCount < minModuleCount) {
            minModuleCount = candidates[i].moduleCount;
        }
    }

    for (size_t i = 0; i < candidates.size(); i++) {
        scoredList.push_back(calculateScore(candidates[i], timeBudget, minModuleCount));
    }
    return scoredList;
}
```
**Fungsi ini menghitung skor semua kandidat sekaligus.** Perhatikan ada **dua pass**: pass pertama mencari `minModuleCount` (jumlah modul tersedikit di antara semua kandidat), pass kedua menghitung skor tiap kandidat memakai nilai minimum itu. Dua pass diperlukan karena `concisenessScore` butuh tahu nilai minimum dulu sebelum bisa menghitung skor tiap kandidat — nilai minimum tidak bisa ditentukan sampai semua kandidat dilihat.

### 7.3 Decision Trace (`printDecisionTrace`)

```cpp
void printDecisionTrace(const vector<ScoredCandidate>& sortedCandidates) {
    cout << "\n=== Decision Trace (Rule-Based Scoring) ===" << endl;
    if (sortedCandidates.empty()) {
        cout << "Tidak ada kandidat untuk dibandingkan." << endl;
        return;
    }

    for (size_t i = 0; i < sortedCandidates.size(); i++) {
        const ScoredCandidate& sc = sortedCandidates[i];
        cout << "\nKandidat " << (i + 1) << ": " << sequenceToString(sc.candidate.moduleSequence) << endl;
        cout << "  Total jam        : " << sc.candidate.totalHours << endl;
        cout << "  Rata-rata sulit  : " << sc.candidate.avgDifficulty << endl;
        cout << "  Jumlah modul     : " << sc.candidate.moduleCount << endl;
        cout << "  efficiencyScore  : " << sc.efficiencyScore << " (bobot 0.45)" << endl;
        cout << "  difficultyScore  : " << sc.difficultyScore << " (bobot 0.35)" << endl;
        cout << "  concisenessScore : " << sc.concisenessScore << " (bobot 0.20)" << endl;
        cout << "  finalScore       : " << sc.finalScore << endl;
    }

    const ScoredCandidate& best = sortedCandidates[0];
    cout << "\n>> Kandidat terpilih: " << sequenceToString(best.candidate.moduleSequence)
         << " (finalScore " << best.finalScore << ")" << endl;
    if (sortedCandidates.size() > 1) {
        cout << ">> Alasan: skor efisiensi waktu (bobot terbesar 0.45) dan tingkat kesulitan "
             << "(bobot 0.35) kandidat ini lebih unggul dibanding kandidat lain, "
             << "sehingga finalScore-nya tertinggi." << endl;
    }
}
```

**Fungsi ini memenuhi requirement "AI reasoning harus terlihat".** Ia mencetak breakdown skor lengkap **untuk setiap kandidat** (bukan cuma pemenang) — total jam, rata-rata kesulitan, jumlah modul, dan ketiga komponen skor beserta bobotnya. Ini adalah bentuk *explainability*: sistem tidak cuma memberi jawaban, tapi menunjukkan **bagaimana** ia sampai ke jawaban itu.

**Satu catatan jujur untuk makalah:** kalimat di baris `">> Alasan: ..."` adalah **teks statis** — selalu menyebut "efisiensi waktu dan tingkat kesulitan lebih unggul" apa pun penyebab sebenarnya kandidat menang. Untuk kasus Dimas, kebetulan ini benar. Tapi kalau suatu saat `concisenessScore` yang jadi penentu, teksnya tetap sama padahal secara teknis kurang tepat. Ini bukan bug fatal (angka breakdown di atasnya tetap akurat dan jujur), tapi kalau dosen bertanya "apakah alasan ini dihitung atau ditulis manual?", jawaban jujurnya: ditulis manual, bukan diturunkan dari data. Ini bukan prioritas untuk diperbaiki, tapi baik untuk kalian ketahui.

### 7.4 Rekomendasi Final (`printFinalRecommendation`)

```cpp
void printFinalRecommendation(const ScoredCandidate& best) {
    cout << "\n=== Rekomendasi Jalur Belajar Final ===" << endl;
    cout << "Urutan modul yang harus diambil: " << sequenceToString(best.candidate.moduleSequence) << endl;
    cout << "Total waktu belajar: " << best.candidate.totalHours << " jam" << endl;
    cout << "Rata-rata kesulitan: " << best.candidate.avgDifficulty << endl;
    cout << "Skor akhir (rule-based): " << best.finalScore << endl;
}
```
Presentasi hasil akhir — mencetak kandidat terbaik (yang sudah diurutkan ke posisi 0 oleh merge sort). Straightforward, tidak ada logika baru.

---

## 8. `merge_sort.h` / `merge_sort.cpp` — Merge Sort Manual

```cpp
static void merge(vector<ScoredCandidate>& candidates, int left, int mid, int right) {
    vector<ScoredCandidate> leftPart(candidates.begin() + left, candidates.begin() + mid + 1);
    vector<ScoredCandidate> rightPart(candidates.begin() + mid + 1, candidates.begin() + right + 1);

    size_t i = 0, j = 0;
    int k = left;
    while (i < leftPart.size() && j < rightPart.size()) {
        if (leftPart[i].finalScore >= rightPart[j].finalScore) {
            candidates[k++] = leftPart[i++];
        } else {
            candidates[k++] = rightPart[j++];
        }
    }
    while (i < leftPart.size()) candidates[k++] = leftPart[i++];
    while (j < rightPart.size()) candidates[k++] = rightPart[j++];
}

static void mergeSortRange(vector<ScoredCandidate>& candidates, int left, int right) {
    if (left >= right) return;                    // BASE CASE
    int mid = left + (right - left) / 2;
    mergeSortRange(candidates, left, mid);         // urutkan separuh kiri
    mergeSortRange(candidates, mid + 1, right);     // urutkan separuh kanan
    merge(candidates, left, mid, right);            // gabungkan dua bagian terurut
}

void mergeSortCandidates(vector<ScoredCandidate>& candidates) {
    if (candidates.empty()) return;
    mergeSortRange(candidates, 0, static_cast<int>(candidates.size()) - 1);
}
```

**Ini merge sort manual** (sesuai requirement: bukan `std::sort`), mengurutkan kandidat dari `finalScore` tertinggi ke terendah.

**`mergeSortRange` — bagian "divide":**
- **Base case:** `if (left >= right) return;` — kalau rentang tinggal 1 elemen (atau kosong), sudah otomatis terurut, berhenti rekursi.
- **Recursive case:** hitung titik tengah, urutkan separuh kiri (rekursi), urutkan separuh kanan (rekursi), lalu gabung.
- `mid = left + (right - left) / 2` — cara menghitung tengah yang aman dari integer overflow (lebih baik daripada `(left + right) / 2` untuk angka sangat besar; di sini datanya kecil, tapi ini kebiasaan yang benar).

**`merge` — bagian "conquer/gabung":**
- Salin dua bagian ke `leftPart` dan `rightPart`.
- Bandingkan elemen terdepan keduanya, ambil yang `finalScore`-nya lebih besar dulu (karena descending, pakai `>=`), tulis kembali ke `candidates`.
- Dua `while` terakhir menyalin sisa elemen yang belum terambil dari salah satu bagian.
- `k++`, `i++`, `j++` adalah *post-increment*: pakai nilai sekarang, lalu naikkan. Jadi `candidates[k++] = leftPart[i++]` berarti "salin `leftPart[i]` ke `candidates[k]`, lalu naikkan `k` dan `i`."

**Penggunaan `>=` (bukan `>`) menjaga stabilitas** — kalau dua kandidat punya skor sama, urutan relatif aslinya dipertahankan. Ini yang membuat merge sort *stable*.

**Kompleksitas O(k log k), k = jumlah kandidat** — penting dibedakan: `k` di sini adalah jumlah kandidat jalur, **bukan** jumlah modul (V). Biasanya k jauh lebih kecil dari V. **Kenapa merge sort, bukan quick sort?** Karena worst-case merge sort konsisten O(k log k), sementara quick sort bisa jatuh ke O(k²) di kasus buruk. Untuk sistem yang butuh hasil predictable, konsistensi ini lebih berharga daripada potensi kecepatan quick sort. Alasan teknis ini bagus untuk makalah.

---

## 9. `search_module.h` / `search_module.cpp` — Linear & Binary Search

```cpp
int linearSearchModule(const vector<Module>& catalog, const string& query) {
    for (size_t i = 0; i < catalog.size(); i++) {
        if (catalog[i].code == query || catalog[i].name == query) {
            return static_cast<int>(i);
        }
    }
    return -1;
}
```
**Linear search** — cek satu per satu dari awal. Bisa mencari berdasarkan kode **atau** nama (`||` = OR). Kompleksitas **O(n)**. Fleksibel (tidak butuh data terurut, bisa cari by nama), cocok untuk pencarian bebas oleh user.

```cpp
int binarySearchModule(const vector<Module>& catalog, const string& code) {
    int low = 0;
    int high = static_cast<int>(catalog.size()) - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (catalog[mid].code == code) {
            return mid;
        } else if (catalog[mid].code < code) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}
```
**Binary search** — mulai dari tengah, buang separuh yang tidak mungkin tiap langkah. Kompleksitas **O(log n)**.

**Cara kerja loop:**
- `low` dan `high` menandai batas rentang pencarian saat ini.
- `mid` adalah tengahnya. Kalau `catalog[mid].code == code`, ketemu.
- Kalau kode di tengah lebih kecil dari target (`< code`), target pasti di kanan → geser `low = mid + 1`.
- Kalau lebih besar, target di kiri → geser `high = mid - 1`.
- Loop berhenti kalau `low > high` (rentang habis, tak ketemu).

**Syarat mutlak binary search: data harus terurut.** Ini valid di project kalian karena `getModuleCatalog()` mendefinisikan modul berurutan M1..M7, dan perbandingan string (`<`) mengurutkan "M1" < "M2" < ... secara leksikografis. Kalau katalog tidak terurut, binary search akan memberi hasil salah — ini asumsi penting yang perlu disebut di makalah.

**Perbandingan untuk makalah:** linear O(n) fleksibel tapi lambat untuk data besar; binary O(log n) cepat tapi butuh data terurut. Untuk 7 modul, keduanya praktis instan — tapi keduanya diimplementasikan untuk mendemonstrasikan requirement searching, dan kalian bisa menjelaskan kapan masing-masing tepat dipakai di dunia nyata.

---

## 10. `menu.h` / `menu.cpp` — Orkestrasi & Antarmuka

File terbesar (304 baris) tapi **tidak mengandung algoritma baru** — perannya *orkestrasi*: memanggil fungsi-fungsi dari file lain dalam urutan yang benar, plus menangani input/output ke user. Kita bahas bagian-bagian pentingnya.

### 10.1 Menu 1 — Tampilkan Katalog (`showModuleCatalog`)

Loop sederhana yang mencetak tiap modul beserta prasyaratnya. Sama seperti `printGraphStructure`, memakai pola `if (g > 0) cout << " ATAU "` untuk memisahkan grup. Tidak ada logika kompleks.

### 10.2 Menu 2 — Cari Modul (`searchModuleMenu`)

```cpp
void searchModuleMenu(const vector<Module>& catalog) {
    // ... pilih mode 1 (linear) atau 2 (binary) ...
    if (mode == 1) {
        cout << "Masukkan kode atau nama modul: ";
        string query;
        getline(cin, query);
        printSearchResult(catalog, linearSearchModule(catalog, query));
    } else if (mode == 2) {
        cout << "Masukkan kode modul: ";
        string code;
        getline(cin, code);
        printSearchResult(catalog, binarySearchModule(catalog, code));
    }
    // ...
}
```
Menghubungkan pilihan user ke fungsi search yang sesuai. `getline(cin, query)` membaca satu baris input penuh (termasuk spasi, berbeda dari `cin >>` yang berhenti di spasi) — penting kalau nama modul mengandung spasi.

### 10.3 Menu 3 — Pilih Profil (`selectStudentProfileMenu`)

Mengembalikan profil Dimas, Sari, atau input manual. Bagian input manual memakai teknik parsing menarik:
```cpp
stringstream ss(masteredLine);
string code;
while (getline(ss, code, ',')) {
    if (!code.empty()) p.masteredModules.insert(code);
}
```
`stringstream` dengan `getline(..., ',')` memecah input seperti "M1,M2,M3" menjadi elemen terpisah berdasarkan koma. Ini cara idiomatis memecah string di C++ tanpa library eksternal.

### 10.4 Menu 4 — Pipeline Rekomendasi (`recommendLearningPathMenu`) — INTI ORKESTRASI

Ini fungsi terpenting di file ini. Alurnya:

```
1. Validasi: target ada di katalog?          → tidak: pesan error, stop
2. Validasi: target sudah dikuasai?           → ya: pesan, stop (tak perlu rekomendasi)
3. collectRequiredModules()                   → kumpulkan kandidat prasyarat (graph.cpp)
4. findBaselinePath() + cetak                 → BFS baseline sebagai info (bfs_baseline.cpp)
5. enumerateAllPaths()                         → SEMUA kandidat via DFS (dfs_path_enum.cpp)
6. filterByBudget()                            → buang kandidat yang > budget

   JIKA feasible KOSONG (budget tak cukup):
     a. minTotalHours() dari semua kandidat    → hitung kebutuhan minimum
     b. findFeasibleAlternativeTarget()         → cari target alternatif yang muat budget
     c. cetak selisih & saran, stop

   JIKA feasible ADA:
7. calculateScoreForAll()                      → skor tiap kandidat (ai_scoring.cpp)
8. mergeSortCandidates()                       → urutkan by skor (merge_sort.cpp)
9. printDecisionTrace()                         → tampilkan breakdown semua kandidat
10. printFinalRecommendation()                  → tampilkan pemenang
```

**Ini adalah "cara kerja sistem secara keseluruhan"** — kalau dosen bertanya bagaimana sistem bekerja dari input sampai output, 10 langkah ini jawabannya. Setiap requirement tugas (graph, BFS, DFS, sorting, searching, Big O) muncul di pipeline ini.

**Dua helper penting yang dipanggil pipeline:**

```cpp
static vector<PathCandidate> filterByBudget(const vector<PathCandidate>& candidates, double timeBudget) {
    vector<PathCandidate> filtered;
    for (size_t i = 0; i < candidates.size(); i++) {
        if (candidates[i].totalHours <= timeBudget) {
            filtered.push_back(candidates[i]);
        }
    }
    return filtered;
}
```
**`filterByBudget`** — membuang kandidat yang total jamnya melebihi budget. O(k) linear scan. Inilah yang memicu edge case Sari: kalau semua kandidat melebihi budget, hasilnya kosong dan program masuk cabang "cari alternatif".

```cpp
static bool findFeasibleAlternativeTarget(const string& originalTarget, const StudentProfile& profile,
                                           const vector<Module>& catalog, const ModuleGraph& graph,
                                           ScoredCandidate& bestAlternative) {
    int idx = findModuleIndexByCode(catalog, originalTarget);
    if (idx == -1) return false;

    set<string> directPrereqs;
    for (size_t g = 0; g < catalog[idx].prereqGroups.size(); g++) {
        for (size_t j = 0; j < catalog[idx].prereqGroups[g].size(); j++) {
            directPrereqs.insert(catalog[idx].prereqGroups[g][j]);
        }
    }
    // ... untuk tiap prasyarat langsung, cari kandidat feasible, pilih yang terbaik ...
}
```
**`findFeasibleAlternativeTarget`** — logika saran alternatif saat target asli tak terjangkau. Ia mengambil **prasyarat langsung** dari target asli (bukan target hardcoded), lalu untuk tiap prasyarat mencari kandidat yang muat budget. Ini **algoritma umum**, bukan skenario Sari yang di-hardcode — kebetulan hasilnya M6 untuk Sari, tapi logikanya generalisasi ke target apa pun. Untuk makalah, ini menunjukkan sistem menangani kegagalan secara cerdas (memberi alternatif konstruktif), bukan sekadar "gagal".

### 10.5 Menu Loop Utama (`runMainMenu`) & Validasi Input

```cpp
int choice;
if (!(cin >> choice)) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Input tidak valid." << endl;
    continue;
}
```
Pola ini muncul berulang untuk **validasi input**. Kalau user mengetik huruf padahal diminta angka, `cin >> choice` gagal:
- `cin.clear()` mereset status error stream.
- `cin.ignore(...)` membuang sisa input yang salah dari buffer.
- Tanpa dua baris ini, program bisa masuk infinite loop atau salah membaca input berikutnya.

Ini *defensive programming* — mengantisipasi user (atau dosen penguji) yang salah ketik, memastikan program tetap jalan dengan anggun alih-alih crash. Detail kecil, tapi tanda kode yang matang.

---

# BAGIAN C: Diagram Alur Data Antar-File

Bagaimana file-file saling terhubung saat program menjalankan rekomendasi (Menu 4):

```
main.cpp
  │
  ├─> module.cpp        : getModuleCatalog() ──> data 7 modul
  │
  ├─> graph.cpp         : buildGraph() ──> adjacency list
  │                       hasCycle() ──> validasi (DFS)
  │
  └─> menu.cpp (runMainMenu → recommendLearningPathMenu)
        │
        ├─> graph.cpp        : collectRequiredModules() [DFS kumpulkan prasyarat]
        │
        ├─> bfs_baseline.cpp : findBaselinePath()       [BFS baseline]
        │
        ├─> dfs_path_enum.cpp: enumerateAllPaths()      [DFS backtracking → kandidat]
        │     └─> graph.cpp  : isModuleReady()          [cek OR-of-AND, dipanggil berulang]
        │
        ├─> menu.cpp         : filterByBudget()         [buang kandidat > budget]
        │
        ├─> ai_scoring.cpp   : calculateScoreForAll()   [skor rule-based]
        │
        ├─> merge_sort.cpp   : mergeSortCandidates()    [urutkan by skor]
        │
        └─> ai_scoring.cpp   : printDecisionTrace()     [tampilkan reasoning]
                               printFinalRecommendation()[tampilkan pemenang]
```

**Fungsi yang jadi "lem" penghubung:** `isModuleReady()` di `graph.cpp` dipanggil oleh hampir semua algoritma (BFS, DFS enumerasi, cek kelayakan) — ini fungsi paling sering dieksekusi, jantung logika OR-of-AND. Kalau kalian ingin menekankan satu fungsi di makalah sebagai "penentu perilaku sistem", ini kandidatnya.

---

## Ringkasan Peta Kode ke Requirement Tugas

| Requirement IF204 | File | Fungsi Kunci | Kompleksitas |
|---|---|---|---|
| Konsep graph | `graph.cpp` | `buildGraph`, `ModuleGraph` (adjacency list) | O(V+E) build |
| BFS | `bfs_baseline.cpp` | `findBaselinePath` | O(V²) versi ini |
| DFS | `graph.cpp`, `dfs_path_enum.cpp` | `hasCycleDFS`, `collectRequiredDFS`, `dfsEnumerate` | O(V+E) & O(2ⁿ) |
| Big O notasi | Semua file | Komentar `// Kompleksitas:` | — |
| Searching | `search_module.cpp` | `linearSearchModule` O(n), `binarySearchModule` O(log n) | O(n), O(log n) |
| Sorting | `merge_sort.cpp` | `mergeSortCandidates` | O(k log k) |
| Studi kasus AI | `ai_scoring.cpp` | `calculateScore`, `printDecisionTrace` | O(k) |

---

## Catatan Verifikasi & Kejujuran Teknis

Tiga hal yang sebaiknya kalian ketahui dan siap jelaskan kalau ditanya dosen (semua sudah dibahas di atas, dirangkum di sini):

1. **BFS baseline bukan Kahn's algorithm murni** — kompleksitasnya O(V²), bukan O(V+E). Ini pilihan sadar (kesederhanaan vs optimalitas), diabaikan karena skala kecil.
2. **DFS enumerasi O(2ⁿ)** — exhaustive backtracking, murah untuk 7 modul tapi tidak akan scale ke ratusan modul.
3. **Teks "Alasan" di Decision Trace bersifat statis** — angka breakdown akurat, tapi kalimat penjelasannya ditulis manual, bukan diturunkan dinamis dari data.

Ketiganya bukan bug — melainkan trade-off dan keputusan desain yang wajar untuk tugas skala ini. Justru kemampuan menjelaskan *kenapa* ketiganya begitu adalah yang membedakan pemahaman mendalam dari sekadar menjalankan kode.

---

*Dokumen ini membedah source code apa adanya, diverifikasi terhadap repository dan hasil compile. Kode program dikerjakan terpisah sesuai rencana kalian. Kalau ada fungsi atau blok tertentu yang masih ingin digali lebih dalam, sebutkan nama file dan fungsinya.*
