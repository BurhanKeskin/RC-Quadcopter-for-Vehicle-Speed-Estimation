#include "common.h"

double roll_pid_i,  roll_last_error,  pitch_pid_i,  pitch_last_error,  yaw_pid_i,  yaw_last_error;
double roll_control_signal,  pitch_control_signal,  yaw_control_signal;

struct MotorPowers calculateMotorPowers(struct Signal data, struct IMU_Values imu_values) {
  // calculate orientation errors (error: difference between desired orientation and actual orientation)
  double rollError = data.RollAngle - imu_values.CurrentOrientation.RollAngle;
  double pitchError = data.PitchAngle - imu_values.CurrentOrientation.PitchAngle;
  double yawError = calculateYawError(data, imu_values);

  // hatalar üzerinden kontrol kazançlarını hesaplama
  roll_control_signal = getControlSignal(rollError, KP_roll_pitch, KI_roll_pitch, KD_roll_pitch, roll_pid_i, roll_last_error, imu_values.DeltaTimeInSeconds);
  pitch_control_signal = getControlSignal(pitchError, KP_roll_pitch, KI_roll_pitch, KD_roll_pitch, pitch_pid_i, pitch_last_error, imu_values.DeltaTimeInSeconds);
  yaw_control_signal = getControlSignal(yawError, KP_yaw, KI_yaw, KD_yaw, yaw_pid_i, yaw_last_error, imu_values.DeltaTimeInSeconds);

  // roll-pitch kontrol sinyallerini limitleme
  roll_control_signal = constrain(roll_control_signal, -ROLL_PITCH_CONTROL_SIGNAL_LIMIT, ROLL_PITCH_CONTROL_SIGNAL_LIMIT);
  pitch_control_signal = constrain(pitch_control_signal, -ROLL_PITCH_CONTROL_SIGNAL_LIMIT, ROLL_PITCH_CONTROL_SIGNAL_LIMIT);
  

  // Her bir motor için güç hesaplama
  struct MotorPowers motorPowers;
  motorPowers.frontLeftMotorPower = round(data.Throttle + roll_control_signal + pitch_control_signal - yaw_control_signal);
  motorPowers.frontRightMotorPower = round(data.Throttle - roll_control_signal + pitch_control_signal + yaw_control_signal);
  motorPowers.rearLeftMotorPower = round(data.Throttle + roll_control_signal - pitch_control_signal + yaw_control_signal);
  motorPowers.rearRightMotorPower = round(data.Throttle - roll_control_signal - pitch_control_signal - yaw_control_signal);

  return motorPowers;
}


double calculateYawError(struct Signal data, struct IMU_Values imu_values) {
  double imuYawAngleChangeInDeltaTime = fix360degrees(imu_values.CurrentOrientation.YawAngle - imu_values.PreviousOrientation.YawAngle);
  double imuYawAngleChangePerSecond = imuYawAngleChangeInDeltaTime / imu_values.DeltaTimeInSeconds;
  double yawError = data.YawAngleChange - imuYawAngleChangePerSecond;
  yawError = constrain(yawError, -QUADCOPTER_MAX_YAW_ANGLE_CHANGE_PER_SECOND, QUADCOPTER_MAX_YAW_ANGLE_CHANGE_PER_SECOND);
  return yawError;
}


void resetPidVariables() {
  roll_pid_i = 0;
  roll_last_error = 0;
  pitch_pid_i = 0;
  pitch_last_error = 0;
  yaw_pid_i = 0;
  yaw_last_error = 0;
}

double fix360degrees(double val) {
  if (val > 180) {
    return val - 360;
  } else if (val < -180) {
    return val + 360;
  } else {
    return val;
  }
}
