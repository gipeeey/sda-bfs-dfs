# Materi Struktur Data & Algoritma (IF204)
### Bahan Belajar untuk Smart Learning Path Recommender

**Ditujukan untuk:** Mahasiswa yang baru mengenal pemrograman dan Struktur Data & Algoritma. Dokumen ini menjelaskan konsep dari nol, dengan bahasa sederhana dan banyak analogi, lalu mengaitkannya ke kode project kelompok Anda.

**Cara membaca dokumen ini:** Baca berurutan dari atas. Setiap topik dibangun di atas topik sebelumnya. Jangan lompat-lompat, karena konsep di bab akhir (misalnya DFS) butuh pemahaman konsep di bab awal (graph). Di akhir tiap topik ada kotak **"Di Kode Kalian"** yang menghubungkan teori ke implementasi nyata di project.

---

## Daftar Isi

1. Apa Itu Struktur Data dan Algoritma?
2. Graph (Graf): Struktur Data Utama Project Ini
3. DAG: Graph Khusus untuk Prasyarat
4. BFS (Breadth-First Search): Menelusuri Selebar-lebarnya
5. DFS (Depth-First Search): Menelusuri Sedalam-dalamnya
6. Big O Notation: Cara Mengukur "Kecepatan" Algoritma
7. Searching: Linear Search & Binary Search
8. Sorting: Merge Sort
9. Rangkuman & Peta ke Requirement Tugas
10. Daftar Sumber

---

## 1. Apa Itu Struktur Data dan Algoritma?

Sebelum masuk ke materi teknis, pahami dulu dua kata di judul mata kuliah ini.

**Struktur Data** adalah *cara kita menyimpan dan menyusun data di dalam komputer.* Analoginya seperti wadah penyimpanan di dapur: ada toples (untuk benda kecil sejenis), ada rak bertingkat (untuk menyusun dari atas ke bawah), ada laci bersekat (untuk memisahkan kategori). Masing-masing wadah punya kelebihan untuk situasi tertentu. Dalam pemrograman, "wadah" ini contohnya: array, list, stack, queue, map, dan **graph** (yang jadi bintang utama project kalian).

**Algoritma** adalah *langkah-langkah untuk menyelesaikan suatu masalah.* Analoginya seperti resep masakan: urutan instruksi yang jelas, dari bahan mentah sampai jadi. Contoh algoritma: cara mengurutkan angka dari kecil ke besar, cara mencari nama di buku telepon, cara menemukan jalur tercepat di peta.

**Hubungan keduanya:** struktur data adalah *tempat menyimpan*, algoritma adalah *cara mengolah*. Algoritma yang baik dipasangkan dengan struktur data yang tepat akan menghasilkan program yang cepat dan hemat memori. Sebaliknya, struktur data yang salah bisa membuat algoritma yang bagus jadi lambat. Inilah inti mata kuliah IF204: belajar memilih pasangan yang tepat.

> **Di Project Kalian:** Sistem "Smart Learning Path Recommender" menyimpan data modul kuliah dalam struktur data **graph**, lalu menggunakan **algoritma** (BFS, DFS, sorting, searching) untuk mengolahnya menjadi rekomendasi jalur belajar. Semua topik di dokumen ini akan muncul di sana.

---

## 2. Graph (Graf): Struktur Data Utama Project Ini

### 2.1 Apa Itu Graph?

Graph adalah **struktur data non-linear** yang terdiri dari dua hal: **titik** dan **garis penghubung**.

- **Vertex** (jamak: vertices), disebut juga **node/simpul** = titik-titiknya
- **Edge** (sisi) = garis yang menghubungkan dua titik

Secara formal, graph ditulis **G(V, E)**, di mana V adalah kumpulan vertex dan E adalah kumpulan edge. Menurut GeeksforGeeks, sebuah graph adalah struktur data non-linear yang terdiri dari vertex (juga disebut node) dan edge, di mana edge adalah garis atau busur yang menghubungkan dua node.

**Analogi paling mudah: peta pertemanan di media sosial.**
- Setiap **orang** = satu vertex (titik)
- Setiap **hubungan pertemanan** = satu edge (garis penghubung)

Kalau Budi berteman dengan Ani, ada garis antara titik "Budi" dan titik "Ani". Kalau Budi juga berteman dengan Citra, ada garis lagi antara "Budi" dan "Citra". Kumpulan semua titik orang + semua garis pertemanan itulah sebuah graph.

**Kenapa disebut "non-linear"?** Bandingkan dengan array/list yang "linear" — data disusun berurutan seperti gerbong kereta (satu demi satu, ada awal dan akhir yang jelas). Graph tidak begitu: satu titik bisa terhubung ke banyak titik lain sekaligus, tanpa urutan baku. Seperti jaring laba-laba, bukan garis lurus.

### 2.2 Jenis-Jenis Graph

Ada beberapa jenis graph yang perlu kalian tahu, karena project kalian memakai jenis tertentu.

| Jenis | Penjelasan | Analogi |
|---|---|---|
| **Undirected** (tak berarah) | Edge berlaku dua arah | Pertemanan: kalau A teman B, maka B juga teman A |
| **Directed** (berarah) | Edge punya arah, satu arah saja | Jalan satu arah; atau "A mengikuti B" di Instagram (belum tentu B mengikuti A) |
| **Unweighted** (tanpa bobot) | Semua edge dianggap setara | Peta jalur yang cuma menunjukkan "terhubung atau tidak" |
| **Weighted** (berbobot) | Tiap edge punya nilai/angka | Peta dengan jarak km di tiap jalan, atau biaya di tiap rute |

> **Di Project Kalian:** Graph modul kalian adalah **directed** (berarah) dan pada dasarnya **unweighted** (walaupun tiap modul punya atribut "jam belajar" dan "kesulitan", itu disimpan di data modul, bukan sebagai bobot edge). Arahnya penting: "M2 adalah prasyarat M4" itu satu arah — kalian harus belajar M2 *sebelum* M4, tidak bisa dibalik. Inilah kenapa graph berarah dipakai.

### 2.3 Cara Menyimpan Graph di Komputer: Adjacency List vs Adjacency Matrix

Setelah paham *apa itu* graph, pertanyaan berikutnya: *bagaimana* menyimpannya di dalam program? Ada dua cara utama.

**Cara 1: Adjacency Matrix (Matriks Ketetanggaan)**

Bayangkan tabel kotak-kotak (seperti tabel perkalian). Baris dan kolomnya adalah daftar semua vertex. Isi tiap kotak: angka 1 kalau ada edge antara dua vertex itu, angka 0 kalau tidak ada.

Contoh, graph dengan 3 titik A, B, C, di mana A terhubung ke B dan A terhubung ke C:

```
     A   B   C
A  [ 0   1   1 ]
B  [ 1   0   0 ]
C  [ 1   0   0 ]
```

Kotak (A,B) = 1 artinya ada edge A–B. Kotak (B,C) = 0 artinya tidak ada edge B–C.

- **Kelebihan:** cek "apakah A dan B terhubung?" sangat cepat — tinggal lihat satu kotak, waktunya konstan O(1).
- **Kekurangan:** boros memori. Untuk n vertex, butuh tabel n×n. Kalau ada 1000 vertex tapi cuma sedikit yang terhubung, kebanyakan kotak berisi 0 (memori terbuang). Space-nya **O(V²)** (kuadrat jumlah vertex).

**Cara 2: Adjacency List (Daftar Ketetanggaan)**

Untuk setiap vertex, simpan **daftar** siapa saja tetangganya. Menurut GeeksforGeeks, adjacency list adalah struktur data untuk merepresentasikan graph di mana setiap node menyimpan daftar vertex tetangganya.

Contoh graph yang sama:

```
A -> [B, C]
B -> [A]
C -> [A]
```

Dibaca: "A bertetangga dengan B dan C. B bertetangga dengan A. C bertetangga dengan A."

- **Kelebihan:** hemat memori. Kita hanya menyimpan edge yang benar-benar ada, tidak menyimpan "ketiadaan edge". Menurut Wikipedia, space adjacency list sebanding dengan jumlah edge dan vertex, sementara adjacency matrix sebanding dengan kuadrat jumlah vertex. Space-nya **O(V + E)**.
- **Kekurangan:** cek "apakah A dan B terhubung?" sedikit lebih lambat — harus menelusuri daftar tetangga A dulu. Menurut Wikipedia, operasi ini butuh waktu sebanding dengan derajat (jumlah tetangga) vertex tersebut.

**Kapan pakai yang mana?** Aturan praktisnya: kalau graph-nya **sparse** (jarang — tiap vertex cuma terhubung ke sedikit vertex lain), pakai **adjacency list**. Kalau graph-nya **dense** (padat — hampir semua vertex saling terhubung), adjacency matrix bisa jadi pilihan. Karena mayoritas graph di dunia nyata itu sparse, adjacency list adalah pilihan paling umum. Menurut GeeksforGeeks (guide oleh Ajit Singh), adjacency list adalah representasi yang disukai untuk sebagian besar masalah graph karena graph dunia nyata biasanya sparse.

> **Di Project Kalian:** Kalian memakai **adjacency list**, disimpan dalam struktur bernama `map<string, vector<vector<string>>> prereqAdjacency`. Terjemahan sederhananya: "untuk setiap kode modul (misal 'M7'), simpan daftar prasyaratnya." Pilihan ini tepat karena graph modul kalian sparse — cuma 7 modul dengan sedikit koneksi antar-modul. Kalau pakai adjacency matrix 7×7, banyak kotak akan berisi 0 (boros). Ini poin bagus untuk dijelaskan di makalah: *kenapa memilih adjacency list, bukan matrix.*

---

## 3. DAG: Graph Khusus untuk Prasyarat

Ini konsep inti yang membuat project kalian masuk akal. Pahami baik-baik.

### 3.1 Apa Itu DAG?

**DAG** = **Directed Acyclic Graph** = Graph Berarah Tanpa Siklus. Mari pecah istilahnya:

- **Directed** (berarah) — sudah dibahas: edge punya arah satu arah.
- **Acyclic** (tanpa siklus/cycle) — tidak ada jalur yang muter balik ke titik awal.

**Apa itu "cycle" (siklus)?** Cycle adalah situasi di mana kalian bisa mulai dari satu titik, mengikuti arah edge, dan akhirnya kembali ke titik awal. Contoh cycle: A → B → C → A. Menurut GeeksforGeeks, kalau graph mengandung cycle seperti u→v→w→u, maka kalian butuh u sebelum v, v sebelum w, dan w sebelum u — mustahil dipenuhi.

**Kenapa prasyarat modul TIDAK BOLEH punya cycle?** Bayangkan aturan konyol ini:
- "Untuk belajar M7, kamu harus sudah belajar M6"
- "Untuk belajar M6, kamu harus sudah belajar M7"

Ini mustahil! Kalian tidak bisa memulai keduanya, karena masing-masing menunggu yang lain. Inilah kenapa data prasyarat **harus** berbentuk DAG (tanpa cycle) — supaya selalu ada titik awal yang bisa dikerjakan.

**Analogi DAG: memakai pakaian formal.** Menurut Baeldung, urutan berpakaian bisa dimodelkan sebagai DAG: kaus kaki harus dipakai sebelum sepatu (ada arah), tapi kaus kaki dan celana bisa dipakai dengan urutan bebas. Yang penting: tidak ada aturan melingkar seperti "pakai sepatu sebelum kaus kaki, dan kaus kaki sebelum sepatu."

### 3.2 Topological Sort: Mengurutkan Berdasarkan Ketergantungan

Kalau kalian punya DAG, kalian bisa melakukan **topological sort** (pengurutan topologis). Menurut GeeksforGeeks, topological sorting untuk DAG adalah pengurutan linear vertex sedemikian rupa sehingga untuk setiap edge berarah u→v, vertex u muncul sebelum v dalam urutan.

Sederhananya: topological sort menyusun semua vertex dalam **satu barisan**, dengan aturan "yang jadi prasyarat harus ada di depan yang membutuhkannya."

**Contoh:** kalau M2 prasyarat M4, dan M4 prasyarat M5, maka salah satu topological sort valid adalah: M2 → M4 → M5. M2 harus di depan M4, M4 harus di depan M5.

**Poin penting yang jadi dasar desain project kalian:** Menurut GeeksforGeeks, satu graph bisa punya **beberapa** topological ordering yang valid. Menurut Wikipedia, kalau topological sort tidak membentuk satu jalur Hamiltonian, maka DAG akan punya dua atau lebih pengurutan valid. Artinya: sering kali ada **lebih dari satu urutan belajar yang sah** menuju target yang sama.

> **Di Project Kalian:** Justru fakta "ada beberapa urutan valid" inilah yang membuat sistem rekomendasi kalian bermakna. Kalau cuma ada satu urutan mungkin, tidak ada yang perlu direkomendasikan — jawabannya sudah pasti. Karena ada banyak jalur valid (berkat model prasyarat OR-of-AND, dijelaskan di bawah), sistem kalian bisa **membandingkan** jalur-jalur itu dan memilih yang terbaik. Itulah inti "AI" di project ini.
>
> Catatan penting untuk makalah: project kalian **tidak** memakai topological sort standar sebagai output akhir. Kalian pakai DFS untuk *menghasilkan banyak kandidat jalur*, lalu skoring untuk *memilih yang terbaik*. Topological sort biasa cuma menghasilkan satu urutan; kalian butuh banyak kandidat supaya ada yang bisa dibandingkan.

### 3.3 Model Prasyarat "OR-of-AND"

Ini konsep khusus project kalian yang membuat jalur belajarnya beragam. Konsepnya sederhana kalau dijelaskan pelan.

Sebuah modul bisa punya beberapa **grup prasyarat**. Aturannya:
- Antar-grup: hubungan **ATAU (OR)** — cukup **satu** grup terpenuhi
- Di dalam satu grup: hubungan **DAN (AND)** — **semua** anggota grup harus terpenuhi

**Contoh dari data kalian, modul M6:** prasyaratnya adalah `[M4]` ATAU `[M2 dan M3]`.

Dibaca: "Kamu boleh mengambil M6 kalau sudah menguasai **M4 saja**, ATAU kalau sudah menguasai **M2 dan M3 sekaligus**."

Ini seperti syarat masuk kompetisi: "Peserta harus punya (sertifikat juara nasional) ATAU (sertifikat juara provinsi DAN surat rekomendasi sekolah)." Ada dua jalur berbeda untuk memenuhi syarat yang sama.

> **Di Project Kalian:** Model OR-of-AND inilah yang menciptakan **banyak jalur berbeda** menuju modul yang sama. Kalau prasyarat cuma model sederhana (satu modul butuh tepat satu modul lain), semua jalur akan collapse jadi satu urutan tunggal — dan tidak ada yang bisa dibandingkan. Dengan OR-of-AND, DFS bisa menemukan jalur lewat M4, atau lewat M2+M3, sebagai kandidat terpisah. Ini keputusan desain penting yang layak ditonjolkan di makalah.

---

## 4. BFS (Breadth-First Search): Menelusuri Selebar-lebarnya

### 4.1 Konsep Dasar

BFS adalah salah satu dari dua cara utama "menjelajahi" graph (yang satunya DFS, dibahas berikutnya). BFS menjelajah **selebar-lebarnya dulu** — kunjungi semua tetangga langsung, baru lanjut ke tetangga-dari-tetangga.

**Analogi: menyebar undangan di gedung bertingkat.** Bayangkan kalian di lantai dasar dan mau membagikan undangan ke semua orang di gedung. Cara BFS: bagikan dulu ke **semua orang di lantai 1** (level terdekat). Setelah lantai 1 habis, baru naik ke **semua orang di lantai 2**. Lalu lantai 3, dan seterusnya. Kalian menyelesaikan satu "lapisan" penuh sebelum pindah ke lapisan berikutnya.

Menurut ScholarHat, BFS menjelajah semua vertex dalam urutan melebar — mulai dari satu vertex dan mengunjungi semua vertex di level yang sama sebelum pindah ke level berikutnya.

### 4.2 Struktur Data yang Dipakai: Queue (Antrian)

BFS memakai struktur data **queue** (antrian). Queue bekerja dengan prinsip **FIFO** = *First In, First Out* = "yang masuk duluan, keluar duluan." Persis seperti antrian di kasir: orang yang datang pertama, dilayani pertama.

Kenapa queue cocok untuk BFS? Karena kita mau memproses vertex sesuai urutan ditemukannya. Vertex yang ditemukan lebih dulu (lebih dekat ke titik awal) diproses lebih dulu — dan itulah yang membuat BFS menjelajah level demi level.

### 4.3 Langkah-Langkah BFS

Menurut BTechSmartClass, langkah BFS adalah:

1. Siapkan sebuah **queue**.
2. Pilih vertex awal, kunjungi, masukkan ke queue.
3. Ambil vertex di **depan** queue. Kunjungi semua tetangganya yang belum dikunjungi, masukkan mereka ke belakang queue.
4. Kalau tidak ada tetangga baru dari vertex depan, keluarkan vertex itu dari queue.
5. Ulangi langkah 3–4 sampai queue kosong.

### 4.4 Kompleksitas dan Kegunaan

- **Kompleksitas:** O(V + E) dengan adjacency list — tiap vertex dan edge disentuh sekali.
- **Kegunaan utama:** mencari **jalur terpendek** (shortest path) di graph tanpa bobot. Menurut freeCodeCamp, BFS sangat berguna untuk mencari jalur terpendek di unweighted graph karena memeriksa semua node pada jarak saat ini sebelum lanjut ke jarak berikutnya.

> **Di Project Kalian:** BFS dipakai di fungsi `findBaselinePath()`. Fungsi ini menghasilkan **satu** urutan belajar valid, dipakai sebagai *pembanding* ("baseline") — bukan sebagai rekomendasi akhir. Analoginya: BFS memberi "jawaban cepat dan sederhana", lalu DFS + skoring memberi "jawaban yang lebih dipertimbangkan." Dengan menampilkan keduanya, sistem kalian bisa menunjukkan bahwa rekomendasi akhirnya memang lebih baik daripada sekadar jalur baseline.
>
> Catatan jujur untuk makalah: implementasi BFS baseline kalian bukan versi "Kahn's algorithm" murni (algoritma BFS standar untuk topological sort yang memakai penghitung in-degree). Versi kalian lebih sederhana. Untuk 7 modul, perbedaan ini tidak berpengaruh ke hasil, tapi bagus untuk dijelaskan sebagai trade-off yang kalian pilih secara sadar: kesederhanaan implementasi vs efisiensi teoretis.

---

## 5. DFS (Depth-First Search): Menelusuri Sedalam-dalamnya

### 5.1 Konsep Dasar

DFS adalah cara kedua menjelajahi graph. Kebalikan dari BFS: DFS menjelajah **sedalam-dalamnya dulu** — ikuti satu jalur sampai mentok, baru mundur dan coba jalur lain.

**Analogi: menjelajahi labirin.** Bayangkan kalian masuk labirin. Cara DFS: pilih satu lorong, jalan terus sedalam mungkin sampai buntu. Begitu buntu, **mundur** (backtrack) ke persimpangan terakhir, lalu coba lorong lain yang belum dijelajahi. Ulangi sampai seluruh labirin terjelajahi.

Menurut freeCodeCamp, DFS bekerja dengan mengikuti satu jalur sejauh mungkin sebelum backtracking — seperti menyelam dalam ke satu jejak, lalu kembali untuk menjelajahi yang lain.

**Perbedaan visual BFS vs DFS:**
- BFS = menyebar seperti gelombang air (melebar merata dari pusat)
- DFS = menyelam seperti akar pohon (dalam dulu di satu cabang, baru pindah cabang)

### 5.2 Struktur Data yang Dipakai: Stack (atau Rekursi)

DFS memakai **stack** (tumpukan). Stack bekerja dengan prinsip **LIFO** = *Last In, First Out* = "yang masuk terakhir, keluar duluan." Persis seperti tumpukan piring: piring yang terakhir ditaruh di atas, itu yang pertama diambil.

Kenapa stack cocok untuk DFS? Karena kita mau selalu melanjutkan dari titik **terbaru** yang kita kunjungi (menyelam lebih dalam). Kalau buntu, stack otomatis membawa kita mundur ke titik sebelumnya.

**Rekursi sebagai alternatif stack:** DFS sering ditulis secara **rekursif** (fungsi yang memanggil dirinya sendiri). Ini karena komputer secara otomatis memakai "call stack" saat menjalankan fungsi rekursif — jadi efeknya sama seperti pakai stack manual, tapi kodenya lebih ringkas. Menurut Medium (Atul Singh), DFS bisa diimplementasikan dengan stack (iteratif) atau secara rekursif.

**Apa itu rekursi?** Rekursi adalah fungsi yang memanggil dirinya sendiri untuk menyelesaikan versi lebih kecil dari masalah yang sama. Setiap fungsi rekursif wajib punya:
- **Base case** = kondisi berhenti (kalau tidak ada, program crash karena memanggil dirinya tanpa henti)
- **Recursive case** = bagian yang memanggil diri sendiri dengan input yang makin mendekati base case

Analogi: seperti boneka matryoshka (boneka Rusia bersusun). Buka boneka besar, ada boneka lebih kecil di dalamnya, buka lagi, ada yang lebih kecil lagi — sampai boneka terkecil yang tidak bisa dibuka (itu base case-nya).

### 5.3 Langkah-Langkah DFS

Menurut BTechSmartClass, langkah DFS adalah:

1. Siapkan sebuah **stack**.
2. Pilih vertex awal, kunjungi, push (masukkan) ke stack.
3. Kunjungi salah satu tetangga yang belum dikunjungi dari vertex di **puncak** stack, push ke stack.
4. Ulangi langkah 3 sampai tidak ada tetangga baru dari vertex puncak.
5. Kalau buntu, **backtrack**: pop (keluarkan) satu vertex dari stack.
6. Ulangi langkah 3–5 sampai stack kosong.

### 5.4 Kompleksitas dan Kegunaan

- **Kompleksitas:** O(V + E) dengan adjacency list — sama seperti BFS.
- **Kegunaan utama:** deteksi cycle, topological sort, dan **menjelajahi semua kemungkinan jalur** (lewat teknik backtracking). Menurut Medium (Atul Singh), DFS populer untuk masalah yang butuh backtracking, seperti memecahkan labirin.

### 5.5 Perbandingan BFS vs DFS

| Aspek | BFS | DFS |
|---|---|---|
| Struktur data | Queue (FIFO) | Stack / rekursi (LIFO) |
| Strategi | Selebar dulu (level demi level) | Sedalam dulu (satu cabang sampai mentok) |
| Analogi | Gelombang air | Akar pohon / labirin |
| Kegunaan utama | Jalur terpendek (unweighted) | Deteksi cycle, backtracking, enumerasi jalur |
| Memori | Cenderung lebih besar (simpan seluruh level) | Cenderung lebih hemat (simpan satu jalur) |
| Kompleksitas | O(V + E) | O(V + E) |

> **Di Project Kalian:** DFS dipakai di **tiga** tempat, dan ini menunjukkan kalian memahami DFS lebih dalam daripada sekadar "menjelajah graph":
>
> 1. **Deteksi cycle** (`hasCycleDFS`) — mengecek saat program mulai, apakah data prasyarat kalian mengandung cycle terlarang. Ini "penjaga" yang memastikan data valid.
> 2. **Mengumpulkan prasyarat** (`collectRequiredDFS`) — menelusuri mundur dari target, mengumpulkan semua modul yang mungkin dibutuhkan.
> 3. **Enumerasi kandidat jalur** (`dfsEnumerate`) — ini yang paling penting. DFS dengan **backtracking** dipakai untuk menemukan **semua** kemungkinan urutan belajar yang valid. Teknik "pilih → coba → batalkan (backtrack) → coba yang lain" inilah yang menghasilkan beberapa kandidat berbeda untuk dibandingkan.
>
> Backtracking di poin 3 adalah jantung "kecerdasan" sistem kalian. Tanpa itu, kalian cuma dapat satu jalur; dengan itu, kalian dapat banyak jalur untuk dinilai dan dipilih yang terbaik.

---

## 6. Big O Notation: Cara Mengukur "Kecepatan" Algoritma

### 6.1 Kenapa Perlu Big O?

Bayangkan kalian punya dua resep membuat kue yang hasilnya sama enak. Resep A butuh 30 menit, resep B butuh 3 jam. Tentu kalian pilih A. Big O adalah cara mengukur hal serupa untuk algoritma: **seberapa cepat/lambat sebuah algoritma saat jumlah datanya bertambah.**

Tapi ada bedanya dengan "menit": Big O tidak mengukur waktu dalam detik (karena itu tergantung komputer yang dipakai). Big O mengukur **seberapa cepat jumlah langkah bertambah ketika data bertambah.** Menurut GeeksforGeeks, Big O menggambarkan batas atas (upper bound) kompleksitas waktu atau ruang suatu algoritma — perilaku pertumbuhan terhadap ukuran input.

**Konsep kunci: worst-case (kasus terburuk).** Big O umumnya mengukur skenario paling buruk. Kenapa? Menurut Towards Data Science, kita pakai worst-case untuk menghilangkan ketidakpastian — algoritma tidak akan pernah berkinerja lebih buruk dari yang kita perkirakan. Kalau kita mencari nama di daftar, worst-case adalah namanya ada di posisi paling akhir (atau tidak ada sama sekali).

### 6.2 Aturan Menyederhanakan Big O

Dua aturan yang wajib kalian tahu:

1. **Abaikan konstanta.** Kalau algoritma butuh 2n langkah, tulis O(n), bukan O(2n). Kalau butuh 100 langkah tetap (tak peduli ukuran data), tulis O(1), bukan O(100).
2. **Ambil suku tertinggi saja.** Kalau algoritma butuh n² + n + 7 langkah, tulis O(n²). Menurut DataCamp, ekspresi 4N² + 2N + 7 disederhanakan menjadi O(N²), karena saat N besar, suku N² yang paling dominan.

Analogi aturan 2: kalau kalian punya utang Rp 1.000.000 + Rp 500 + Rp 3, yang benar-benar penting adalah yang sejuta. Angka kecil "tenggelam" dibanding angka besar.

### 6.3 Jenis-Jenis Kompleksitas (dari Tercepat ke Terlambat)

Ini tabel paling penting untuk kalian hafal dan pahami:

| Notasi | Nama | Arti Sederhana | Contoh |
|---|---|---|---|
| **O(1)** | Konstan | Selalu sama, tak peduli ukuran data | Mengambil elemen array lewat index: `arr[5]` |
| **O(log n)** | Logaritmik | Bertambah sangat pelan; tiap langkah memangkas separuh data | Binary search |
| **O(n)** | Linear | Bertambah sebanding data; data 2×, langkah 2× | Mencari satu per satu di daftar (linear search) |
| **O(n log n)** | Linierithmik | Sedikit lebih lambat dari linear | Merge sort, quick sort |
| **O(n²)** | Kuadratik | Data 2×, langkah 4×; melambat cepat | Nested loop (loop di dalam loop) |
| **O(2ⁿ)** | Eksponensial | Data +1, langkah 2×; sangat lambat | Mencoba semua kombinasi/subset |

**Penjelasan tiap jenis dengan analogi:**

- **O(1) — Konstan.** Seperti menekan tombol lift lantai 5: entah gedungnya 5 lantai atau 50 lantai, kalian tetap menekan satu tombol. Menurut GeeksforGeeks, operasi seperti mengakses array lewat index selalu butuh waktu sama tanpa peduli ukuran array.

- **O(log n) — Logaritmik.** Seperti mencari kata di kamus (yang sudah terurut): buka tengah, kalau kata yang dicari ada di belakang, abaikan separuh depan, cari lagi di tengah sisanya. Tiap langkah membuang separuh. Menurut Towards Data Science, O(log n) berarti algoritma bertambah satu langkah setiap kali data berlipat ganda.

- **O(n) — Linear.** Seperti membaca semua nama di daftar hadir satu per satu. Kalau daftarnya 2× lebih panjang, waktunya 2× lebih lama. Menurut GeeksforGeeks, ini berarti waktu eksekusi bertambah sebanding ukuran input.

- **O(n²) — Kuadratik.** Seperti setiap orang di ruangan bersalaman dengan setiap orang lain. Kalau ada n orang, jumlah salamannya sekitar n×n. Menurut GeeksforGeeks, ini muncul di nested loop: untuk tiap elemen di loop luar, loop dalam jalan n kali, total n×n iterasi.

- **O(2ⁿ) — Eksponensial.** Sangat lambat. Menurut GeeksforGeeks, contohnya adalah membuat semua subset dari sebuah himpunan. Tiap tambahan satu elemen melipatgandakan jumlah kemungkinan.

**Catatan:** Selain Big O (yang mengukur worst-case/batas atas), ada juga notasi lain: **Big Omega (Ω)** untuk best-case, dan **Big Theta (Θ)** untuk kasus tepat. Tapi untuk tugas S1, biasanya cukup Big O.

> **Di Project Kalian:** Setiap fungsi utama di kode kalian sudah punya komentar `// Kompleksitas: O(...)`. Ini penting untuk makalah — kalian harus bisa menjelaskan *kenapa* tiap fungsi punya kompleksitas tertentu. Contoh yang menarik dibahas:
> - Fungsi graph (buildGraph, collectRequired) = O(V+E) — tiap vertex & edge disentuh sekali.
> - Binary search = O(log n) — memangkas separuh tiap langkah.
> - Merge sort = O(n log n) — konsisten di semua kasus.
> - **DFS enumerasi kandidat = O(2ⁿ) worst-case** — ini paling menarik. Karena backtracking mengeksplorasi banyak kombinasi, kompleksitasnya eksponensial. Untuk 7 modul, itu maksimal 128 kemungkinan (murah). Tapi kalian bisa jelaskan trade-off: *pendekatan ini boros secara teori, tapi acceptable karena skala datanya kecil.* Ini menunjukkan pemahaman, bukan sekadar hafalan.

---

## 7. Searching: Linear Search & Binary Search

Searching = mencari data tertentu di dalam kumpulan data. Ada dua metode dasar yang wajib kalian pahami.

### 7.1 Linear Search (Pencarian Linear)

**Cara kerja:** cek satu per satu dari awal sampai ketemu (atau sampai habis). Menurut Towards Data Science, ini berarti memeriksa setiap elemen di daftar sampai menemukan yang dicari.

**Analogi:** mencari kunci di dalam laci yang berantakan. Kalian ambil satu benda, cek, bukan; ambil lagi, cek, bukan; sampai ketemu atau laci habis.

- **Kompleksitas:** O(n) — worst-case, benda yang dicari ada di paling akhir atau tidak ada.
- **Kelebihan:** tidak butuh data terurut, sederhana.
- **Kekurangan:** lambat untuk data besar.

### 7.2 Binary Search (Pencarian Biner)

**Cara kerja:** mulai dari tengah. Kalau target lebih besar dari elemen tengah, cari di separuh kanan; kalau lebih kecil, cari di separuh kiri. Ulangi — tiap langkah membuang separuh kemungkinan.

**Analogi:** menebak angka 1–100. Teman kalian membatin sebuah angka, kalian menebak. Tebakan pintar: mulai dari 50. Kalau dijawab "terlalu kecil", kalian tahu jawabannya 51–100 (buang separuh). Tebak 75, dan seterusnya. Dalam ~7 tebakan, angka pasti ketemu — jauh lebih cepat daripada menebak 1, 2, 3, ... satu per satu.

- **Kompleksitas:** O(log n) — jauh lebih cepat dari linear search.
- **Syarat WAJIB:** data **harus sudah terurut** dulu. Ini kunci yang sering dilupakan.

**Trade-off penting untuk makalah:** Menurut Towards Data Science, meski binary search jauh lebih efisien, kalian tidak selalu memilihnya — karena binary search mengharuskan data terurut, dan proses mengurutkan (sorting) itu sendiri punya biaya. Dalam kondisi tertentu (data kecil atau jarang dicari), linear search justru lebih praktis daripada harus sorting dulu.

> **Di Project Kalian:** Kalian mengimplementasikan **keduanya secara manual** (bukan pakai fungsi bawaan C++ `std::find` atau `std::binary_search`) — ini sesuai requirement tugas. `linearSearchModule()` bisa mencari berdasarkan kode ATAU nama modul (fleksibel, tidak butuh urut). `binarySearchModule()` mencari berdasarkan kode dan mengasumsikan katalog sudah terurut — asumsi ini valid karena katalog kalian didefinisikan berurutan M1 sampai M7. Kalian bisa menjelaskan di makalah *kapan* masing-masing metode cocok dipakai.

---

## 8. Sorting: Merge Sort

Sorting = mengurutkan data (misal dari kecil ke besar). Ada banyak algoritma sorting; project kalian memakai **merge sort**.

### 8.1 Perbandingan Algoritma Sorting

| Algoritma | Best Case | Average | Worst Case | Stabil? |
|---|---|---|---|---|
| Bubble Sort | O(n) | O(n²) | O(n²) | Ya |
| Selection Sort | O(n²) | O(n²) | O(n²) | Tidak |
| Insertion Sort | O(n) | O(n²) | O(n²) | Ya |
| **Merge Sort** | O(n log n) | O(n log n) | **O(n log n)** | Ya |
| Quick Sort | O(n log n) | O(n log n) | O(n²) | Tidak |

### 8.2 Cara Kerja Merge Sort

Merge sort memakai strategi **divide and conquer** (bagi dan taklukkan). Idenya: masalah besar dipecah jadi masalah-masalah kecil yang mudah, lalu hasilnya digabung.

**Tiga langkah:**
1. **Divide (bagi):** pecah daftar jadi dua separuh, terus-menerus, sampai tiap bagian tinggal 1 elemen. (Satu elemen otomatis "sudah terurut" — tidak ada yang perlu diurutkan.)
2. **Conquer (selesaikan):** ini otomatis terjadi saat bagian sudah tinggal 1 elemen.
3. **Merge (gabung):** gabungkan dua bagian yang sudah terurut jadi satu bagian besar yang terurut, dengan membandingkan elemen terdepan dari masing-masing.

**Analogi: mengurutkan tumpukan kartu.** Bagi tumpukan jadi dua, beri ke dua teman. Masing-masing bagi lagi, dan seterusnya, sampai tiap orang pegang 1 kartu. Lalu gabungkan berpasangan sambil mengurutkan: bandingkan dua kartu, susun yang benar, gabung jadi tumpukan kecil terurut; gabung dua tumpukan kecil jadi tumpukan sedang terurut; sampai jadi satu tumpukan besar terurut.

### 8.3 Kenapa Merge Sort untuk Project Ini?

Perhatikan tabel di 8.1: **merge sort adalah satu-satunya yang worst-case-nya tetap O(n log n).** Quick sort, meski rata-ratanya sama cepat, bisa jatuh ke O(n²) di kasus terburuk (misalnya data yang sudah hampir terurut dengan pemilihan pivot yang naif).

Untuk sistem rekomendasi yang butuh hasil **predictable** (bisa diprediksi, konsisten), merge sort adalah pilihan yang lebih aman. Ini alasan teknis yang kuat untuk dijelaskan di makalah.

> **Di Project Kalian:** Merge sort diimplementasikan **manual** (bukan `std::sort`) di `mergeSortCandidates()`, dipakai untuk mengurutkan kandidat jalur belajar dari skor tertinggi ke terendah. Kompleksitasnya O(k log k) di mana k = jumlah kandidat (bukan jumlah modul — ini perlu dibedakan). Alasan memilih merge sort dibanding quick sort (worst-case konsisten) adalah poin akademis yang bagus untuk ditonjolkan.

---

## 9. Rangkuman & Peta ke Requirement Tugas

### 9.1 Peta Materi ke Requirement

| Requirement Tugas IF204 | Materi (Bab) | Diterapkan di Fungsi |
|---|---|---|
| Konsep graph | Bab 2 & 3 | `buildGraph`, `ModuleGraph` |
| BFS | Bab 4 | `findBaselinePath` |
| DFS | Bab 5 | `hasCycleDFS`, `collectRequiredDFS`, `dfsEnumerate` |
| Big O notasi | Bab 6 | Komentar `// Kompleksitas:` di tiap fungsi |
| Searching | Bab 7 | `linearSearchModule`, `binarySearchModule` |
| Sorting | Bab 8 | `mergeSortCandidates` |
| Tema AI + bermanfaat | Semua | Sistem rekomendasi jalur belajar |

### 9.2 Alur Berpikir Sistem (Untuk Menjelaskan "Cara Kerja" di Makalah)

Kalau dosen bertanya "bagaimana sistem ini bekerja secara keseluruhan?", jawabannya adalah rangkaian konsep di dokumen ini:

1. Data modul + prasyarat disimpan sebagai **graph (DAG)** → Bab 2–3
2. **DFS** menjelajahi graph untuk menemukan **semua** urutan belajar valid (backtracking) → Bab 5
3. **BFS** memberi satu jalur baseline sebagai pembanding → Bab 4
4. Tiap kandidat dinilai dengan aturan skoring (rule-based) → (bagian "AI" project)
5. **Merge sort** mengurutkan kandidat dari skor terbaik → Bab 8
6. **Searching** dipakai untuk fitur cari modul → Bab 7
7. **Big O** dipakai menganalisis efisiensi tiap langkah → Bab 6

### 9.3 Poin-Poin yang Menunjukkan Pemahaman (Bukan Sekadar Copas)

Kalau kalian ingin makalah dan presentasi terlihat memahami materi, tonjolkan pembahasan-pembahasan ini:

- **Kenapa adjacency list, bukan matrix?** (graph sparse, hemat memori)
- **Kenapa data prasyarat harus DAG (tanpa cycle)?** (kalau ada cycle, mustahil dikerjakan)
- **Kenapa OR-of-AND penting?** (menciptakan banyak jalur untuk dibandingkan)
- **Kenapa DFS backtracking, bukan topological sort biasa?** (butuh banyak kandidat, bukan satu urutan)
- **Kenapa DFS enumerasi itu O(2ⁿ), dan kenapa masih acceptable?** (skala kecil, 7 modul)
- **Kenapa merge sort, bukan quick sort?** (worst-case konsisten O(n log n), predictable)
- **Kenapa binary search butuh data terurut, dan kapan linear search lebih baik?** (trade-off sorting)

Setiap poin di atas adalah pertanyaan "kenapa" — dan kemampuan menjawab "kenapa" itulah yang membedakan memahami dari menghafal.

---

## 10. Daftar Sumber

Sumber-sumber berikut adalah referensi terpercaya dan terbaru (2024–2026) yang dipakai menyusun materi ini. Cocok untuk dicantumkan di daftar pustaka makalah kalian:

**Graph & Representasi:**
- GeeksforGeeks — "Representation of Graph" (update Okt 2025): https://www.geeksforgeeks.org/dsa/graph-and-its-representations/
- GeeksforGeeks — "Adjacency List Representation": https://www.geeksforgeeks.org/dsa/adjacency-list-meaning-definition-in-dsa/
- Wikipedia — "Adjacency list": https://en.wikipedia.org/wiki/Adjacency_list
- Ajit Singh — "Complete Guide to Graph Data Structure" (Jan 2026): https://singhajit.com/data-structures/graph/

**DAG & Topological Sort:**
- GeeksforGeeks — "Topological Sorting" (update Des 2025): https://www.geeksforgeeks.org/dsa/topological-sorting/
- Wikipedia — "Directed acyclic graph": https://en.wikipedia.org/wiki/Directed_acyclic_graph
- Baeldung — "Topological Sort of Directed Acyclic Graph": https://www.baeldung.com/cs/dag-topological-sort

**BFS & DFS:**
- freeCodeCamp — "Graph Algorithms in Python: BFS, DFS, and Beyond" (Sep 2025): https://www.freecodecamp.org/news/graph-algorithms-in-python-bfs-dfs-and-beyond/
- ScholarHat — "Breadth First & Depth First Traversal" (Sep 2025): https://www.scholarhat.com/tutorial/datastructures/breadth-first-traversal-and-depth-first-traversal
- BTechSmartClass — "Graph Traversal BFS/DFS": http://www.btechsmartclass.com/data_structures/graph-traversal-bfs.html

**Big O Notation:**
- GeeksforGeeks — "Analysis of Algorithms: Big O Analysis" (Mar 2026): https://www.geeksforgeeks.org/dsa/analysis-algorithms-big-o-analysis/
- DataCamp — "Big O Notation and Time Complexity Guide": https://www.datacamp.com/tutorial/big-o-notation-time-complexity
- Towards Data Science — "Introduction to Big O Notation": https://towardsdatascience.com/introduction-to-big-o-notation-820d2e25d3fd/

**Searching & Sorting:**
- W3Schools — "DSA Sorting & Searching": https://www.w3schools.com/dsa/
- GeeksforGeeks — "What does Big O - O(log N) complexity mean?": https://www.geeksforgeeks.org/dsa/what-does-big-o-olog-n-complexity-mean/

---

*Catatan: Semua penjelasan konseptual sudah disederhanakan untuk pembaca yang baru mengenal pemrograman. Kalau ada bagian yang masih terasa sulit, catat spesifik bab dan bagiannya, supaya bisa dijelaskan ulang dengan pendekatan berbeda. Kode program dikerjakan terpisah — dokumen ini murni materi pemahaman konsep.*
