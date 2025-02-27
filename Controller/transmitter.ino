#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "common.h"

const uint64_t pipeOut = 0xF0F0F0F0E1LL;  // Bu Adres Alıcı İçerisindeki ile Aynı Olmalı | This address has to be the same as the address in in the receiver
RF24 radio(9, 10);                        // select CE,CSN pin | CE ve CSN pinlerin seçimi

void Init_Transmitter() {

  //Serial.begin(9600);  // Serial iletişimi başlat
  //Serial.println("Initializing Transmitter...");

  radio.begin();
  radio.openWritingPipe(pipeOut);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.stopListening();  // Start the radio communication for Transmitter | Verici olarak sinyal iletişimi başlatılıyor

  //Serial.println("Transmitter setup is completed.");
}

struct Signal SendSignal() {
  struct Signal data;

  // Potansiyometrelerden anlık verileri oku | Read the current values from potantiometers
  int throttle_input = analogRead(THROTTLE_PIN);
  int roll_input = analogRead(ROLL_PIN);
  int pitch_input = analogRead(PITCH_PIN);
  int yaw_input = analogRead(YAW_PIN);

  // Değerleri Sınırla | Limit the values
  throttle_input = constrain(throttle_input, 415, 615);
  roll_input = constrain(roll_input, 410, 625);
  pitch_input = constrain(pitch_input, 425, 610);
  yaw_input = constrain(yaw_input, 425, 610);

  // Değerleri Belirlenen Aralıkla Maple | Map the values to specified range
  data.throttle = map_double(throttle_input, 615, 415, 0, 180);
  data.roll = map_double(roll_input, 625, 410, -20, 20);
  data.pitch = map_double(pitch_input, 610, 425, -20, 20);
  data.YawAngleChange = map_double(ignoreDeadBand(yaw_input), 610, 425, -180, 180);

  
  if (digitalRead(VIDEO_BUTTON_PIN) == HIGH) {
    data.videoStatus = 'S';  // Start sinyali
  } else if (digitalRead(VIDEO_BUTTON_PIN) == LOW) {
    data.videoStatus = 'E';  // End sinyali
  }

  // Veri Gönderimi | Data Transmission
  if ((radio.write(&data, sizeof(Signal))) && (data.videoStatus == 'S')) {
    digitalWrite(LED_PIN, HIGH);
    Serial.println(data.videoStatus);
  } else if ((radio.write(&data, sizeof(Signal))) && (data.videoStatus == 'E')) {
    digitalWrite(LED_PIN, LOW);
    Serial.println(data.videoStatus);
  }

  delay(50);

  return data;
}

int ignoreDeadBand(int val) {
  int center = (JOYSTICK_MIN_VALUE + JOYSTICK_MAX_VALUE) / 2;
  if (abs(val - center) <= JOYSTICK_DEAD_BAND)
    return center;
  else
    return val;
}

double map_double(double x, double in_min, double in_max, double out_min, double out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
