#include <Adafruit_PWMServoDriver.h>
#define Servo_freq 50
#define Limit 170.0
#define ZERO_POINT 249
#define MAXIMUM_POINT 500
#define Servo_Speed 0.5
#define Angle_range 7

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x47);
class SERVO
{
    float pos[4];
    float set_pos[4];
    bool leg[4] = {true, true, true, true};
    int pulse[4];

public:
    SERVO() {}
    ~SERVO() {}
    void set()
    {
        pwm.begin();
        pwm.setOscillatorFrequency(27000000);
        pwm.setPWMFreq(Servo_freq);
        pwm.setPWM(0, 0, ZERO_POINT);
        pwm.setPWM(4, 0, ZERO_POINT); // 전류 측정 모터
        pwm.setPWM(1, 0, ZERO_POINT);
        pwm.setPWM(5, 0, ZERO_POINT); // 전류 측정 모터
        pwm.setPWM(2, 0, ZERO_POINT);
        pwm.setPWM(6, 0, ZERO_POINT); // 전류 측정 모터
        pwm.setPWM(3, 0, ZERO_POINT);
        pwm.setPWM(7, 0, ZERO_POINT); // 전류 측정 모터
        delay(1000);
    }
    bool *returnlegstate()
    {
        return leg;
    }
    void operate(float i, float j, int num)
    {
        if (j > Limit && leg[num] == true)
        {
            set_pos[num] = pwm.getPWM(num);
            leg[num] = false;
        }
        else if (j < Limit && leg[num] == true)
        {
            pos[num] = constrain(i, 250, 500);
            pwm.setPWM(num, 0, pos[num]);
            pwm.setPWM(num + 4, 0, pos[num]);
        }
        if (leg[num] == false)
        {
            pwm.setPWM(num, 0, set_pos[num]);
            pwm.setPWM(num + 4, 0, set_pos[num]);
        }
        pwm.setPWM(num, 0, pos[num]);
        pwm.setPWM(num + 4, 0, pos[num]);
    }

    void Stay_state(int pose, float angle_X, float angle_Y)
    {
        switch (pose)
        {
        case 1:
            //좌상
            if ((angle_X < Angle_range) && (angle_Y < Angle_range))
            {
                break;
            }
            pwm.setPWM(4, 0, set_pos[0] - 10);
            pwm.setPWM(0, 0, set_pos[0]);
            set_pos[0] += Servo_Speed;

            pwm.setPWM(6, 0, set_pos[2] + 10);
            pwm.setPWM(2, 0, set_pos[2]);
            set_pos[2] -= Servo_Speed;

            break;
        case 2:
            //좌
            //if (angle_X < 0)
            //{
            //    break;
            //} 센서가 원점에서 우측으로 부착되어있음
            pwm.setPWM(4, 0, set_pos[0] - 10);
            pwm.setPWM(0, 0, set_pos[0]);
            set_pos[0] += Servo_Speed;

            pwm.setPWM(5, 0, set_pos[1] - 10);
            pwm.setPWM(1, 0, set_pos[1]);
            set_pos[1] += Servo_Speed;

            pwm.setPWM(6, 0, set_pos[2] + 10);
            pwm.setPWM(2, 0, set_pos[2]);
            set_pos[2] -= Servo_Speed;

            pwm.setPWM(7, 0, set_pos[3] + 10);
            pwm.setPWM(3, 0, set_pos[3]);
            set_pos[3] -= Servo_Speed;
            break;
        case 3:
            //좌하
            if (angle_X > -Angle_range && angle_Y < Angle_range)
            {
                break;
            }
            pwm.setPWM(5, 0, set_pos[1] - 10);
            pwm.setPWM(1, 0, set_pos[1]);
            set_pos[1] += Servo_Speed;

            pwm.setPWM(7, 0, set_pos[3] + 10);
            pwm.setPWM(3, 0, set_pos[3]);
            set_pos[3] -= Servo_Speed;

            break;
        case 4:
            //하
            if (angle_X > -Angle_range)
            {
                break;
            }
            pwm.setPWM(5, 0, set_pos[1] - 10);
            pwm.setPWM(1, 0, set_pos[1]);
            set_pos[1] += Servo_Speed;

            pwm.setPWM(6, 0, set_pos[2] - 10);
            pwm.setPWM(2, 0, set_pos[2]);
            set_pos[2] += Servo_Speed;

            pwm.setPWM(4, 0, set_pos[0] + 10);
            pwm.setPWM(0, 0, set_pos[0]);
            set_pos[0] -= Servo_Speed;

            pwm.setPWM(7, 0, set_pos[3] + 10);
            pwm.setPWM(3, 0, set_pos[3]);
            set_pos[3] -= Servo_Speed;

            break;
        case 5:
            //우하
            if (angle_X > -Angle_range && angle_Y > -Angle_range)
            {
                break;
            }

            pwm.setPWM(6, 0, set_pos[2] - 10);
            pwm.setPWM(2, 0, set_pos[2]);
            set_pos[2] += Servo_Speed;

            pwm.setPWM(4, 0, set_pos[0] + 10);
            pwm.setPWM(0, 0, set_pos[0]);
            set_pos[0] -= Servo_Speed;

            break;
        case 6:
            //우
            if (angle_Y > -Angle_range)
            {
                break;
            }
            pwm.setPWM(6, 0, set_pos[2] - 10);
            pwm.setPWM(2, 0, set_pos[2]);
            set_pos[2] += Servo_Speed;

            pwm.setPWM(7, 0, set_pos[3] - 10);
            pwm.setPWM(3, 0, set_pos[3]);
            set_pos[3] += Servo_Speed;

            pwm.setPWM(4, 0, set_pos[0] + 10);
            pwm.setPWM(0, 0, set_pos[0]);
            set_pos[0] -= Servo_Speed;

            pwm.setPWM(5, 0, set_pos[1] + 10);
            pwm.setPWM(1, 0, set_pos[1]);
            set_pos[1] -= Servo_Speed;

            break;
        case 7:
            //우상
            if (angle_Y > -Angle_range && angle_X < Angle_range)
            {
                break;
            }
            pwm.setPWM(7, 0, set_pos[3] - 10);
            pwm.setPWM(3, 0, set_pos[3]);
            set_pos[3] += Servo_Speed;

            pwm.setPWM(5, 0, set_pos[1] + 10);
            pwm.setPWM(1, 0, set_pos[1]);
            set_pos[1] -= Servo_Speed;

            break;
        case 8:
            //상
            if (angle_X < Angle_range)
            {
                break;
            }

            pwm.setPWM(4, 0, set_pos[0] - 10);
            pwm.setPWM(0, 0, set_pos[0]);
            set_pos[0] += Servo_Speed;

            pwm.setPWM(7, 0, set_pos[3] - 10);
            pwm.setPWM(3, 0, set_pos[3]);
            set_pos[3] += Servo_Speed;

            pwm.setPWM(5, 0, set_pos[1] + 10);
            pwm.setPWM(1, 0, set_pos[1]);
            set_pos[1] -= Servo_Speed;

            pwm.setPWM(6, 0, set_pos[2] + 10);
            pwm.setPWM(2, 0, set_pos[2]);
            set_pos[2] -= Servo_Speed;

            break;
        case 9:
            //원점
            break;
        }
    }
    void Servo_reset()
    {
        for (int i = 0; i <= 3; i++)
        {
            set_pos[i] = 0;
            pos[i] = 0;
            leg[i] = true;
        }
    }
};