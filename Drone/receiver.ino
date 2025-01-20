#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "common.h"

const uint64_t pipeIn = 0xF0F0F0F0E1LL;
RF24 radio(8, 10);  // radio(CE, CSN)

double prev_throttle = 0;
double prev_roll = 0;
double prev_pitch = 0;
double prev_yaw_change = 0;

unsigned long lastReceiveTime = 0;

void Init_Receiver() {

  radio.begin();
  radio.openReadingPipe(1, pipeIn);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.startListening();  // Start the radio communication for receiver | Alıcı olarak sinyal iletişimi başlatılıyor

  //Serial.println("Receiver setup is completed.");
}

struct Signal GetSignal() {
  struct Signal data;

  // Önceki değerleri değişkenlere ata | Assign the previous values to the variables
  data.Throttle = prev_throttle;
  data.RollAngle = prev_roll;
  data.PitchAngle = prev_pitch;
  data.YawAngleChange = prev_yaw_change;

  if (radio.available()) {

    radio.read(&data, sizeof(Signal));
    lastReceiveTime = millis();  // Receive the data | Data alınıyor

    // Önceki Değerleri Güncelle | Update the Previous Values
    prev_throttle = data.Throttle;
    prev_roll = data.RollAngle;
    prev_pitch = data.PitchAngle;
    prev_yaw_change = data.YawAngleChange;

    if (data.videoStatus == 'S') {
      //Serial.println("Start signal received. Sending to Raspberry Pi...");
      Serial.write('S');  // Send to Raspberry Pi via UART
    } else if (data.videoStatus == 'E') {
      //Serial.println("End signal received. Sending to Raspberry Pi...");
      Serial.write('E');  // Send to Raspberry Pi via UART
    }

  } else if (millis() - lastReceiveTime > 1000) {
    data.Throttle = 0;
    data.RollAngle = 0;
    data.PitchAngle = 0;
    Serial.println("Signal lost.");
  }

  return data;
}

