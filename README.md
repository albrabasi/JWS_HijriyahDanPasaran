# JWS_HijriyahDanPasaran
"Library Hijriah Kuwaiti dan Pasaran Jawa untu pembuatan JWS".
 * ==============================================================================
 * LIBRARY : WaktuLokal.h (JWS Terpadu: Hijriah Kuwaiti & Pasaran Jawa)
 * AUTHOR  :  Ahmad AlBrabasi & Practical Developer JWS
 * VERSI   : 1.0 (Maret 2026)
 * ==============================================================================
 * DESKRIPSI:
 * Library ini dirancang khusus untuk proyek Jadwal Sholat (JWS).
 * Menyatukan Algoritma Kuwaiti yang presisi untuk penanggalan Hijriah dan logika 
 * perhitungan Pasaran Jawa (Legi, Pahing, Pon, Wage, Kliwon).
 * * FITUR UNGGULAN:
 * 1. Dual Julian Date: Koreksi tanggal Hijriah tidak akan merusak Pasaran Jawa.
 * 2. Auto-Maghrib: Tanggal Hijriah/Jawa otomatis berganti saat waktu Maghrib tiba.
 * 3. Hemat RAM: Nama bulan dan hari disimpan di PROGMEM (Flash Memory).
 * 4. Regional Switch: Opsi untuk menampilkan atau menyembunyikan Pasaran Jawa.
 * * CARA PENGGUNAAN:
 * 1. Sertakan library: #include "WaktuLokal.h"
 * 2. Buat objek: WaktuLokal kalender;
 * 3. Panggil fungsi hitungSemua() di dalam void loop():
 * HasilWaktu data = kalender.hitungSemua(tgl, bln, thn, h_masehi,j_maghrib, m_maghrib,j_rtc, m_rtc,  kor_hijriah, tpl_pasaran);
 * * PENJELASAN PARAMETER:
 * - tgl, bln, thn: Tanggal Masehi dari RTC (int).
 * - h_masehi  : Index hari (0=Minggu, 1=Senin, dst).
 * - j/m_maghrib: Waktu maghrib hari ini (untuk pergantian tanggal).
 * - j/m_rtc    : Waktu saat ini dari RTC.
 * - kor_hijriah  : 0 (pas), 1 (mundur 1 hari), -1 (maju 1 hari).
 * - tpl_pasaran   : true (tampil pasaran), false (tidak di tampilkan).
 * * DATA YANG DIHASILKAN:
 * - data.fullHijriah : "14 Sya'ban 1447 H"
 * - data.displayHari : "Selasa Pon" (jika pasaran true) atau "Selasa" (jika false).
 * - data.tglH, data.blnH, data.thnH : Data angka tanggal Hijriah.
 * ==============================================================================
   
