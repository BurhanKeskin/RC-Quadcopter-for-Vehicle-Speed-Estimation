#include <avr/wdt.h>
#include "common.h"

void setup() {
  Serial.begin(115200);   // UART için serial bağlantı başlatma
  wdt_enable(WDTO_1S);    // 1 saniye aşım süreli watch dog timer etkinleştirildi.

  Init_Receiver();        // Quadcopter üzerindeki nRF modülünü aktif etme
  Init_Motors();          // Quadcopter üzerindeki motorları aktif etme
  Init_IMU();             // Quadcopter üzerindeki mpu6050'yi aktif etme
}

void loop() {
  wdt_reset();   // watch dog timerın aşım süresi sıfırlandı - sistemde sorun yok anlamına gelir.
  
  struct Signal data = GetSignal();                   // Kumandadan joystick değerlerini okuma
  struct IMU_Values imuValues = Get_IMU_Values();     // mpu6050'den veri okuma
  
  if(imuValues.Error || data.Throttle < THROTTLE_START_POINT){
    stopMotors();
    resetPidVariables();
    return;
  }

  if (imuValues.NewDataAvailable) {
    struct MotorPowers motorPowers = calculateMotorPowers(data, imuValues);
    spinMotors(motorPowers);
  }

}
