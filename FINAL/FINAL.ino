#include <Wire.h>

#include <iarduino_I2C_Bumper.h>
iarduino_I2C_Bumper bum(0x0C);

#include <iarduino_I2C_Motor.h>
iarduino_I2C_Motor mot_R (0x0A);
iarduino_I2C_Motor mot_L (0x0B);

#include <iarduino_HC_SR04.h>
iarduino_HC_SR04 UZ_front(12,11);
iarduino_HC_SR04 UZ_right(2,  7);

#include <Servo.h>
Servo servo_arm;
Servo servo_finger;

void robot_setup() {
  bum.begin();
  mot_L.begin();
  mot_R.begin();
  mot_L.setDirection(false);
  mot_R.setDirection(true);
  servo_finger.attach(9);
  servo_finger.write(40);
  servo_arm.attach(3);
  servo_arm.write(180);
  delay(1000);
  servo_finger.detach();
  servo_arm.detach();
}

void robot_main() {
  number_1_1();
  number_2_1();
  number_3_1();
  number_4();
  number_3_2();
  number_2_2();
  number_1_2();
}

void setup() {
  robot_setup();
  robot_main();
}

void loop() {}
