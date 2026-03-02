

/* * ==============================================================================
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
 */


#ifndef WAKTULOKAL_H
#define WAKTULOKAL_H

#include <Arduino.h>

// Simpan nama di PROGMEM agar hemat RAM
const char namaBulanHijriyah[][15] PROGMEM = {
  "Muharram", "Safar", "Rabi'ul Awal", "Rabi'ul Akhir", 
  "Jumadil Awal", "Jumadil Akhir", "Rajab", "Sya'ban", 
  "Ramadhan", "Syawal", "Dzulqaidah", "Dzulhijjah"
}; [cite: 1]

const char namaPasaran[][8] PROGMEM = {
  "Legi", "Pahing", "Pon", "Wage", "Kliwon"
}; [cite: 2]

const char namaHariMasehi[][8] PROGMEM = {
  "Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jum'at", "Sabtu"
}; [cite: 2]

struct HasilWaktu {
  int tglH;
  int blnH;
  int thnH;
  String bulanH_str;
  String pasaran;
  String hariM;
  String fullHijriah; // Berisi: "Tgl Bln Thn H"
  String displayHari; // Berisi: "Hari" atau "Hari Pasaran"
};

class WaktuLokal {
public:
    // Parameter 'tampilPasaran' (true/false) ditambah di fungsi ini
    HasilWaktu hitungSemua(int d, int m, int y, int hr, int magribJam, int magribMenit, int skrgJam, int skrgMenit, int adj, bool tampilPasaran) {
      HasilWaktu res;
      
      double tanggal = (double)d;
      double bulan = (double)m;
      double tahun = (double)y;

      // Logika Pergantian Hari Hijriah & Jawa saat Magrib 
      int menitMagrib = (magribJam * 60) + magribMenit;
      int menitSekarang = (skrgJam * 60) + skrgMenit;
      if (menitSekarang >= menitMagrib) {
        tanggal++; 
      }

      // --- PERHITUNGAN JULIAN DATE DASAR ---
      if (bulan < 3) {
        tahun -= 1;
        bulan += 12;
      }
      double a = floor(tahun / 100.0);
      double b = 2 - a + floor(a / 4.0);
      if (tahun < 1583) b = 0;
      
      // jd_asli digunakan untuk Pasaran agar TIDAK berubah saat ada koreksi 
      double jd_asli = floor(365.25 * (tahun + 4716)) + floor(30.6001 * (bulan + 1)) + tanggal + b - 1524; [cite: 1]
      
      // jd_koreksi digunakan khusus untuk kalender Hijriah [cite: 1]
      double jd_koreksi = jd_asli - (double)adj; 

      // --- LOGIKA HARI MASEHI ---
      char bufferHari[10];
      strcpy_P(bufferHari, namaHariMasehi[hr]);
      res.hariM = String(bufferHari);

      // --- LOGIKA PASARAN JAWA (Mandiri) ---
      if (tampilPasaran) {
        long julianLong = (long)(jd_asli + 1); 
        int indexPasaran = (julianLong % 5); [cite: 2]
        char bufferPasaran[10];
        strcpy_P(bufferPasaran, namaPasaran[indexPasaran]); [cite: 2]
        res.pasaran = String(bufferPasaran);
        res.displayHari = res.hariM + " " + res.pasaran; // Contoh: "Senin Pon"
      } else {
        res.pasaran = "";
        res.displayHari = res.hariM; // Hanya "Senin"
      }

      // --- LOGIKA HIJRIAH (Menggunakan jd_koreksi) ---
      double z = jd_koreksi - 1948084; [cite: 1]
      double cyc = floor(z / 10631.0);
      z = z - 10631 * cyc;
      double j = floor((z - (8.01/60.0)) / (10631.0/30.0));
      double iy = 30 * cyc + j;
      z = z - floor(j * (10631.0/30.0) + (8.01/60.0));
      double im = floor((z + 28.5001) / 29.5);
      if (im == 13) im = 12;
      double id = z - floor(29.5001 * im - 29);

      res.tglH = (int)id;
      res.blnH = (int)im;
      res.thnH = (int)iy;

      char bufferBulan[20];
      strcpy_P(bufferBulan, namaBulanHijriyah[res.blnH - 1]); [cite: 1]
      res.bulanH_str = String(bufferBulan);

      res.fullHijriah = String(res.tglH) + " " + res.bulanH_str + " " + String(res.thnH) + " H";
      
      return res;
    }
};



#endif