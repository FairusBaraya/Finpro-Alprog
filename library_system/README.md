# Sistem Manajemen Perpustakaan
**Mata Kuliah Algoritma dan Pemrograman | Teknik Elektro | Universitas Indonesia**

---

## Struktur Project

```
library_system/
├── include/
│   ├── LibraryItem.h     ← Abstract class (Abstraksi + Enkapsulasi)
│   ├── Book.h            ← Child class (Inheritance + Polymorphism)
│   ├── Magazine.h        ← Child class (Inheritance + Polymorphism)
│   ├── Member.h          ← Enkapsulasi data anggota
│   ├── LinkedList.h      ← BONUS: Linked List manual (tanpa std::list)
│   ├── Algorithms.h      ← BONUS: Quick Sort + Binary Search + Big-O
│   ├── Library.h         ← Business logic utama
│   └── Protocol.h        ← Konstanta protokol jaringan
├── src/
│   ├── server.cpp        ← Server (Winsock2)
│   └── client.cpp        ← Client interaktif (CLI)
├── json/
│   └── json.hpp          ← nlohmann/json (download manual, lihat bawah)
├── README.md
└── build.bat             ← Script kompilasi otomatis
```

---

## Prasyarat

- **Compiler**: MinGW-w64 (g++ versi 8+)
  - Download: https://winlibs.com atau via MSYS2
  - Pastikan `g++` tersedia di PATH
- **Library JSON**: nlohmann/json (single header)

---

## Langkah Setup

### 1. Download json.hpp

Buka PowerShell di folder `library_system/` dan jalankan:

```powershell
Invoke-WebRequest -Uri "https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp" -OutFile "json/json.hpp"
```

Atau download manual dari:
https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp
→ simpan sebagai `json/json.hpp`

### 2. Kompilasi

Jalankan script build:
```cmd
build.bat
```

Atau kompilasi manual:
```cmd
g++ -std=c++17 -O2 -Wall -Ijson -Iinclude -o server.exe src/server.cpp -lws2_32
g++ -std=c++17 -O2 -Wall -Ijson -Iinclude -o client.exe src/client.cpp -lws2_32
```

---

## Cara Menjalankan

**Buka dua terminal terpisah:**

Terminal 1 — jalankan server:
```cmd
server.exe
```

Terminal 2 — jalankan client:
```cmd
client.exe
```

---

## Fitur Aplikasi

| No | Fitur | Keterangan |
|----|-------|------------|
| 1  | Lihat semua koleksi | Ditampilkan terurut A-Z (Quick Sort) |
| 2  | Cari koleksi | Substring search (Linear Search) |
| 3  | Pinjam item | Validasi ketersediaan |
| 4  | Kembalikan item | Update status otomatis |
| 5  | Daftar member | Tampilkan jumlah pinjaman aktif |
| 6  | Tambah buku | Admin — tambah ke koleksi |
| 7  | Tambah member | Daftarkan anggota baru |
| 8  | Hapus item | Hanya bisa jika tidak dipinjam |

---

## Pemenuhan Requirement Teknis

### Struktur Data
- `LinkedList<T>` — implementasi manual **tanpa std::list/std::vector**
- Node-based, mendukung pushBack O(1), removeIf O(n), iterator

### Algoritma (implementasi mandiri)
| Algoritma | Lokasi | Best | Average | Worst | Space |
|-----------|--------|------|---------|-------|-------|
| Quick Sort | `Algorithms.h` | O(n log n) | O(n log n) | O(n²) | O(log n) |
| Binary Search | `Algorithms.h` | O(1) | O(log n) | O(log n) | O(1) |
| Linear Search | `Algorithms.h` | O(1) | O(n) | O(n) | O(1) |

### OOP — 4 Pilar
| Pilar | Implementasi |
|-------|-------------|
| **Abstraksi** | `LibraryItem` — abstract class dengan pure virtual `getInfo()`, `getType()` |
| **Enkapsulasi** | Semua atribut `private`/`protected`, akses via getter/setter |
| **Inheritance** | `Book` dan `Magazine` mewarisi `LibraryItem` |
| **Polimorfisme** | `display()` virtual — output berbeda untuk Book vs Magazine |

### Networking
- **Protokol**: TCP/IP via Winsock2
- **Arsitektur**: Client-Server (1 client — sesuai ketentuan Elektro)
- **Port**: 54321

### Data Interchange
- Semua pesan client ↔ server menggunakan format **JSON**
- Library: `nlohmann/json` (single header)
- Contoh request: `{"action":"borrow","data":{"member_id":1,"item_id":3}}`
- Contoh response: `{"status":"ok","message":"Alice berhasil meminjam \"Laskar Pelangi\""}`

### Bonus (+10%)
- ✅ **Linked List manual** — `include/LinkedList.h` tanpa std::list/vector
- ✅ **Big-O Notation** — didokumentasikan di setiap fungsi di `Algorithms.h` dan `Library.h`
- ❌ Multithreading — di-skip (hanya 1 client, ketentuan Elektro)

---

## Contoh Alur JSON

```
Client → Server:
{"action":"list_items"}

Server → Client:
{"status":"ok","message":"7 item ditemukan","data":[
  {"id":4,"type":"Buku","title":"Atomic Habits","author":"James Clear",
   "year":2018,"available":true,"info":"..."},
  ...
]}
```

```
Client → Server:
{"action":"search","data":{"query":"laskar"}}

Server → Client:
{"status":"ok","message":"1 hasil untuk \"laskar\"","data":[
  {"id":1,"type":"Buku","title":"Laskar Pelangi","author":"Andrea Hirata",
   "year":2005,"available":true}
]}
```

---

*Dibuat untuk Tugas Akhir Mata Kuliah Algoritma dan Pemrograman*
*Teknik Elektro, Universitas Indonesia — 2026*
