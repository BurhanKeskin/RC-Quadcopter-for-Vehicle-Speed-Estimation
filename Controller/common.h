#ifndef COMMON_H
#define COMMON_H

#define THROTTLE_PIN A0
#define YAW_PIN A1
#define PITCH_PIN A2
#define ROLL_PIN A3

// Video Kaydı İçin
#define VIDEO_BUTTON_PIN 4

// Led Pini
#define LED_PIN 2

struct Signal {
    double throttle;
    double roll;
    double pitch;
    char videoStatus;
    double YawAngleChange;
};

#endif
