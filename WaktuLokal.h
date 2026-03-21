/* * ==============================================================================
 * LIBRARY : WaktuLokal.h (JWS Terpadu: Hijriah Kuwaiti & Pasaran Jawa)
 * AUTHOR  :  Ahmad AlBrabasi & Practical Developer JWS
 * VERSI   : 1.1.0 (Maret 2026)
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
 * * Jenis Puasa yang Didukung:
 * * Library ini secara otomatis mendeteksi:
 * Puasa Rutin: Senin & Kamis.
 * Puasa Bulanan: Ayyamul Bidh (kecuali hari tasyrik di bulan Dzulhijjah).
 * Puasa Tahunan: Tasu'a, Asyura, Syawal (6 hari), Tarwiyah, dan Arafah.
 * ==============================================================================
 */
 #ifndef WAKTULOKAL_H
#define WAKTULOKAL_H

#include <Arduino.h>

const char namaBulanHijriyah[][15] PROGMEM = {
  "Muharram", "Safar", "Rabi'ul Awal", "Rabi'ul Akhir", 
  "Jumadil Awal", "Jumadil Akhir", "Rajab", "Sya'ban", 
  "Ramadhan", "Syawal", "Dzulqaidah", "Dzulhijjah"
};

const char namaPasaran[][8] PROGMEM = {
   "Pahing", "Pon", "Wage", "Kliwon","Pahing"
};

const char namaHariMasehi[][8] PROGMEM = {
  "Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jum'at", "Sabtu"
};

struct HasilWaktu {
  int tglH, blnH, thnH;
  String bulanH_str;
  String pasaran;
  String hariM;
  String fullHijriah;
  String displayHari;
  String infoSunnah; // Hari ini
  String infoBesok;  // H-1
};

class WaktuLokal {
private:
  String cekPuasa(int hM, int tH, int bH) {
      if (hM == 1) return "Puasa Senin";
      if (hM == 4) return "Puasa Kamis";
      if (tH >= 13 && tH <= 15) {
        if (!(bH == 12 && tH >= 13)) return "Puasa Ayyamul Bidh";
      }
      if (bH == 1 && tH == 9) return "Puasa Tasu'a";
      if (bH == 1 && tH == 10) return "Puasa Asyura";
      if (bH == 10 && tH >= 2 && tH <= 7) return "Puasa Sunnah Syawal";
      if (bH == 12 && tH == 8) return "Puasa Tarwiyah";
      if (bH == 12 && tH == 9) return "Puasa Arafah";  
      return "";
    }

public:
    HasilWaktu hitungSemua(int d, int m, int y, int hr, int magribJ, int magribM, int skrgJ, int skrgM, int adj, bool tampilPasaran) {
      HasilWaktu res;
      double tanggal = (double)d;
      double bulan = (double)m;
      double tahun = (double)y;

      if ((skrgJ * 60 + skrgM) >= (magribJ * 60 + magribM)) tanggal++; 

      if (bulan < 3) { tahun -= 1; bulan += 12; }
      double a = floor(tahun / 100.0);
      double b = 2 - a + floor(a / 4.0);
      if (tahun < 1583) b = 0;
      
      double jd_asli = floor(365.25 * (tahun + 4716)) + floor(30.6001 * (bulan + 1)) + tanggal + b - 1524;
      double jd_koreksi = jd_asli - (double)adj; 

      // Data Hari & Pasaran
      char bufH[10]; strcpy_P(bufH, namaHariMasehi[hr]);
      res.hariM = String(bufH);
      if (tampilPasaran) {
        char bufP[10]; strcpy_P(bufP, namaPasaran[(long)(jd_asli + 1) % 5]);
        res.pasaran = String(bufP);
        res.displayHari = res.hariM + " " + res.pasaran;
      } else {
        res.displayHari = res.hariM;
      }

      // Hitung Hijriah
      double z = jd_koreksi - 1948084;
      double cyc = floor(z / 10631.0);
      z -= 10631 * cyc;
      double j = floor((z - 0.1335) / 354.366);
      double iy = 30 * cyc + j;
      z -= floor(j * 354.366 + 0.1335);
      double im = floor((z + 28.5001) / 29.5);
      if (im == 13) im = 12;
      double id = z - floor(29.5001 * im - 29);

      res.tglH = (int)id; res.blnH = (int)im; res.thnH = (int)iy;
      char bufB[20]; strcpy_P(bufB, namaBulanHijriyah[res.blnH - 1]);
      res.bulanH_str = String(bufB);
      res.fullHijriah = String(res.tglH) + " " + res.bulanH_str + " " + String(res.thnH) + " H";

      // Info Puasa Sunnah
      String hIni = cekPuasa(hr, res.tglH, res.blnH);
      res.infoSunnah = (hIni != "") ? "Hari ini " + hIni : "";
      
      // Estimasi Besok
      int tB = res.tglH + 1; int bB = res.blnH;
      if (tB > 30) { tB = 1; bB = (bB % 12) + 1; }
      String hBesok = cekPuasa((hr + 1) % 7, tB, bB);
      res.infoBesok = (hBesok != "") ? "Besok " + hBesok : "";

      return res;
    }
};
#endif

