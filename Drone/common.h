#ifndef COMMON_H
#define COMMON_H

#define IMU_COMMUNICATION_TIMEOUT_IN_MILLISECONDS 100

//----------- ESC ----------------
#define MIN_MOTOR_PULSE_WIDTH 1000
#define MAX_MOTOR_PULSE_WIDTH 2000

//----------- PINS ------------
#define FRONT_LEFT_MOTOR_PIN 3
#define FRONT_RIGHT_MOTOR_PIN 5
#define REAR_LEFT_MOTOR_PIN 6
#define REAR_RIGHT_MOTOR_PIN 9
#define INTERRUPT_PIN 2

//--- MPU6050 CALIBRATION ---
#define ACCEL_OFFSET_X -2446
#define ACCEL_OFFSET_Y 309 
#define ACCEL_OFFSET_Z 820
#define GYRO_OFFSET_X 120
#define GYRO_OFFSET_Y 0
#define GYRO_OFFSET_Z 14

//----------- LIMITS -----------
#define THROTTLE_START_POINT 10  // between 0-180
#define THROTTLE_LIMIT_POINT 180 // between 0-180
double QUADCOPTER_MAX_TILT_ANGLE = 20.00; // roll, pitch tilt angle limit in degrees
double QUADCOPTER_MAX_YAW_ANGLE_CHANGE_PER_SECOND = 180.00;

//----- PID CONFIGURATION-------
double KP_roll_pitch = 0.30;
double KI_roll_pitch = 0.10;
double KD_roll_pitch = 0.10;

double KP_yaw = 0.40;
double KI_yaw = 0.50;
double KD_yaw = 0.00;

//----------- PID CONTROL LIMITS-----------
double ROLL_PITCH_CONTROL_SIGNAL_LIMIT = KP_roll_pitch * QUADCOPTER_MAX_TILT_ANGLE * 2;


struct Signal {
    double Throttle;
    double RollAngle;
    double PitchAngle;
    char videoStatus;
    double YawAngleChange;
};


struct Orientation {
  double YawAngle;
  double PitchAngle;
  double RollAngle;
};


struct IMU_Values {
  bool Error;
  bool NewDataAvailable;
  double DeltaTimeInSeconds;
  struct Orientation CurrentOrientation;
  struct Orientation PreviousOrientation;
};


struct MotorPowers {
  int frontLeftMotorPower;
  int frontRightMotorPower;
  int rearLeftMotorPower;
  int rearRightMotorPower;
};

#endif