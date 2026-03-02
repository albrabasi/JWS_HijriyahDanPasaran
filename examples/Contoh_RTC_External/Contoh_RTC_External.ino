#include <Wire.h>
#include <RTClib.h> // Library populer dari Adafruit
#include <WaktuLokal.h>

RTC_DS3231 rtc;
WaktuLokal kalender;

void setup() {
  Serial.begin(115200);
  if (!rtc.begin()) {
    Serial.println("RTC Tidak Ditemukan!");
    while (1);
  }
}

void loop() {
  DateTime now = rtc.now();
  
  // Asumsi Maghrib jam 18:10, Koreksi Hijriah 0, Tampilkan Pasaran true
  HasilWaktu data = kalender.hitungSemua(now.day(), now.month(), now.year(), now.dayOfTheWeek(), 18, 10, now.hour(), now.minute(), 0, true);

  Serial.println(data.displayHari);
  Serial.println(data.fullHijriah);
  if(data.infoBesok != "") Serial.println(data.infoBesok);
  if(data.infoSunnah != "") Serial.println(data.infoSunnah);
  
  delay(10000); // Update tiap 10 detik
}
