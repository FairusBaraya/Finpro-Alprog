#pragma once
#include <string>

// ============================================================
//  Protokol komunikasi Client-Server
//
//  Semua pesan dikirim sebagai JSON string diakhiri '\n'.
//
//  FORMAT REQUEST (client → server):
//  {
//    "action": "<string>",
//    "data":   { ... }      // opsional, tergantung action
//  }
//
//  FORMAT RESPONSE (server → client):
//  {
//    "status":  "ok" | "error",
//    "message": "<string>",
//    "data":    [ ... ] | { ... }   // opsional
//  }
//
//  ACTIONS yang didukung:
//    list_items         — daftar semua item (terurut)
//    search             — cari item berdasarkan judul
//      data: { "query": "<judul>" }
//    borrow             — pinjam item
//      data: { "member_id": <int>, "item_id": <int> }
//    return_item        — kembalikan item
//      data: { "member_id": <int>, "item_id": <int> }
//    list_members       — daftar semua member
//    add_book           — tambah buku (admin)
//      data: { "title","author","year","isbn","genre" }
//    add_member         — tambah member
//      data: { "name", "email" }
//    remove_item        — hapus item (admin)
//      data: { "item_id": <int> }
//    ping               — cek koneksi
// ============================================================

namespace Protocol {
    constexpr int    SERVER_PORT    = 54321;
    constexpr int    BUFFER_SIZE    = 65536;  // 64 KB
    constexpr char   DELIMITER      = '\n';   // pemisah antar pesan
}
