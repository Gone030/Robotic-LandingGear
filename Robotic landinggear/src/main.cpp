#include <Arduino.h>
#include "UltraSound_Sensing.h"
#include "Current_Sensing.h"
#include "Servo_Control.h"
#include "Servo.h"

#include "Tilt_Sensing.h"

Current_Sensors Current;
SERVO Servo;
tilt MPU;
Ultra ULT;

bool print = true;
int mode = 1;      // 0 : 동작 안함, 1 : 착륙 감지, 2 : 착륙, 3 : 균형 유지
bool *legstate[4]; // 다리 상태 변수. 포인터 배열.
bool near = false;
bool if_fly = false;

void setup()
{
  Serial.begin(9600);
  Current.set();
  Servo.set();
  MPU.set();
  ULT.set();
  Serial.println(F("READY"));
}
//
void loop()
{

  MPU.operate();
  float angle_x = MPU.return_Angle_x();
  float angle_y = MPU.return_Angle_y();
  switch (mode)
  {
  case 1:
    Servo.Servo_reset();
    if (print)
    {
      Serial.println(F("거리 측정 중"));
      print = false;
    }
    near = ULT.operate();

    if (near == true && ULT.return_distance() <= 15)
    {
      Serial.println(F("착륙 준비"));
      mode = 2;
      print = true;
      break;
    }
    break;
  case 2:
    if (print)
    {
      Serial.println(F("착륙 시작"));
      print = false;
    }
    for (float pulse = ZERO_POINT; pulse <= MAXIMUM_POINT; pulse += Servo_Speed)
    {
      for (int count_leg = 0; count_leg <= 3; count_leg++)
      {
        Servo.operate(pulse, Current.operate(count_leg), count_leg);
      }
    }
    *legstate = Servo.returnlegstate();
    if (*legstate[0] == false && *legstate[1] == false && *legstate[2] == false && *legstate[3] == false)
    {
      mode = 3; // 수평유지 모드로 변환
      Serial.println(F("균형유지 준비"));
      print = true;
      break;
    }
    break;
  case 3:
    if (print)
    {
      Serial.println(F("균형유지 모드"));
      print = false;
    }
    int Pose = MPU.sensing();
    Serial.println(Pose);
    Servo.Stay_state(Pose, angle_x, angle_y);

    if (Pose == 9)
    {
      Serial.println(F("착륙완료."));
      ULT.operate();
      if (ULT.return_distance() > 30)
      {
        for (int num = 0; num <= 3; num++)
        {
          pwm.setPWM(num, 0, 250);
          pwm.setPWM(num + 4, 0, 250);
        }
        print = true;
        mode = 1;

        break;
      }
    }
    break;
  }
}
