#include <ESP32Time.h> // Library ESP32Time oleh fbiego
#include <WaktuLokal.h>

ESP32Time rtc;
WaktuLokal kalender;

void setup() {
  Serial.begin(115200);
  // Set waktu manual (contoh saja, biasanya dari NTP)
  rtc.setTime(30, 15, 10, 3, 3, 2026); // Detik, Menit, Jam, Tgl, Bln, Thn
}

void loop() {
  // Ambil data dari RTC internal
  int tgl = rtc.getDay();
  int bln = rtc.getMonth() + 1; // ESP32Time bulan 0-11
  int thn = rtc.getYear();
  int hari = rtc.getDayofWeek(); // 0=Minggu

  HasilWaktu data = kalender.hitungSemua(tgl, bln, thn, hari, 18, 5, rtc.getHour(true), rtc.getMinute(), 0, true);

  Serial.println("--- JWS ESP32 INTERNAL ---");
  Serial.println(data.displayHari + " | " + data.fullHijriah);
  Serial.println(data.infoBesok);
  Serial.println(data.infoSunnah);

  delay(10000);
}
