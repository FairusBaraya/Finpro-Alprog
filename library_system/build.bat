@echo off
echo === Build: Sistem Manajemen Perpustakaan ===
echo.

REM Cek apakah json.hpp sudah ada
if not exist "json\json.hpp" (
    echo [!] json\json.hpp tidak ditemukan!
    echo     Jalankan perintah berikut di PowerShell:
    echo     Invoke-WebRequest -Uri "https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp" -OutFile "json/json.hpp"
    pause
    exit /b 1
)

echo [1/2] Kompilasi server.exe...
g++ -std=c++17 -O2 -Wall -Ijson -Iinclude -o server.exe src/server.cpp -lws2_32
if %errorlevel% neq 0 (
    echo [!] Gagal kompilasi server.exe
    pause
    exit /b 1
)
echo     OK: server.exe

echo [2/2] Kompilasi client.exe...
g++ -std=c++17 -O2 -Wall -Ijson -Iinclude -o client.exe src/client.cpp -lws2_32
if %errorlevel% neq 0 (
    echo [!] Gagal kompilasi client.exe
    pause
    exit /b 1
)
echo     OK: client.exe

echo.
echo === Build selesai! ===
echo Jalankan: server.exe  (terminal 1)
echo           client.exe  (terminal 2)
echo.
pause
