#include <MPU6050.h>
#include <I2Cdev.h>

#define Pi 3.141592
#define T 5
#define Radians_to_degrees 180 / 3.14159
#define fs 131.0; //자이로센서의 출력값을 각속도로 변환
#define MPU_X_POINT 10
#define MPU_Y_POINT 10
#define X_offset 0
#define Y_offset 8.5
#define tilt_limit 7.0
#define SerialDebug false //시리얼출력 원하면

MPU6050 mpu;

class tilt
{
private:
    uint8_t MpuIntStatus;
    int16_t ax, ay, az;
    int16_t gx, gy, gz;
    int16_t mx, my, mz;

    //자이로(각속도)
    float Gyro_x, Gyro_y;
    //최종 가속도,자이로 각도
    float Accel_x, Accel_y;
    float Gyro_angle_x = 0, Gyro_angle_y = 0;
    //상보필터 거친 각도
    float Angle_x = 0, Angle_y = 0;
    //자이로센서 바이어스값
    float Base_gx = 0, Base_gy = 0;
    //오프셋 적용한 각도
    int C_Angle_x;
    int C_Angle_y;
    unsigned long Pre_msec = 0;
    bool leg_tilt_states[4] = {true};

public:
    tilt() {}
    ~tilt() {}
    void set()
    {
        mpu.initialize();                  //센서 초기화
        MpuIntStatus = mpu.getIntStatus(); //센서 상태 읽기

        //Calibrate
        int loop = 10;
        for (int i = 0; i < loop; i++)
        {
            mpu.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
            Base_gx += gx;
            Base_gy += gy;
        }
        Base_gx /= loop;
        Base_gy /= loop;
    }

    void operate()
    {
        MpuIntStatus = mpu.getIntStatus();
        //단위시간 변화량
        float dt = (millis() - Pre_msec) / 1000.0;
        Pre_msec = millis();

        mpu.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);

        //가속도값 아크탄젠트->각도변환
        Accel_x = atan(ay / sqrt(pow(ax, 2) + pow(az, 2))) * Radians_to_degrees;
        Accel_y = atan(-1 * ax / sqrt(pow(ay, 2) + pow(az, 2))) * Radians_to_degrees;

        //자이로 -32766~+32766을 실제 250degree/s로 변환
        //앞에서 계산한 오차의 평균값을 빼줌
        Gyro_x = (gx - Base_gx) / fs;
        Gyro_y = (gy - Base_gy) / fs;

        
        Gyro_angle_x = Angle_x + dt * Gyro_x;
        Gyro_angle_y = Angle_y + dt * Gyro_y;

        //상보필터
        Angle_x = 0.95 * Gyro_angle_x + 0.05 * Accel_x;
        Angle_y = 0.95 * Gyro_angle_y + 0.05 * Accel_y;

        C_Angle_x = Angle_x + X_offset;
        C_Angle_y = Angle_y + Y_offset;
    }
    int sensing()
    { //  ↖=1 ⇦=2 ↙=3 ⇩=4 ↘=5 ⇨=6 ↗=7 ⇧=8
        // (x=+, y=+) 1번다리, (x=+, y=-) 2번다리
        // (x=-, y=-) 3번다리, (x=-, y=+) 4번다리
        int pose = 0;
        if (C_Angle_x > tilt_limit)
        {
            if (C_Angle_y > tilt_limit)
            {
                pose = 1;
                return pose;
            }
            else if (C_Angle_y < -tilt_limit)
            {
                pose = 7;
                return pose;
            }
            pose = 8;
            return pose;
        }
        else if (C_Angle_x < -tilt_limit)
        {
            if (C_Angle_y > tilt_limit)
            {
                pose = 3;
                return pose;
            }
            else if (C_Angle_y < -tilt_limit)
            {
                pose = 5;
                return pose;
            }

            pose = 4;
            return pose;
        }
        else
        {
            if (C_Angle_y > tilt_limit)
            {
                pose = 2;
                return pose;
            }
            if (C_Angle_y < -tilt_limit)
            {
                pose = 6;
                return pose;
            }
            else
            {
                pose = 9;
                return pose;
            }
        }
        //return pose;
    }
    float return_Angle_x() { return C_Angle_x; }
    float return_Angle_y() { return C_Angle_y; }
    bool *returnlegstate()
    {
        return leg_tilt_states;
    }
};