#include "common.h"

void setup() {
  Serial.begin(115200);
  Init_Transmitter();

  // Video Kaydı İçin Gerekli Olan Buton Ayarları
  pinMode(VIDEO_BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT); // LED pini çıkış olarak ayarlandı
  
  digitalWrite(LED_PIN, LOW); // Başlangıçta LED kapalı
}

void loop() {
  struct Signal data = SendSignal();
}
