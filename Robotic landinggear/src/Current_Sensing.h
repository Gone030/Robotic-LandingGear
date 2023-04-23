#include <Adafruit_INA219.h>

#define Serial_Debug false // 시리얼모니터 출력시 true

Adafruit_INA219 ina219_1(0x40);
Adafruit_INA219 ina219_2(0x41);
Adafruit_INA219 ina219_3(0x44);
Adafruit_INA219 ina219_4(0x45);

class Current_Sensors
{
    float j[4];

public:
    Current_Sensors() {}
    ~Current_Sensors() {}
    void set()
    { 
        // 각각의 전류센서 보정, 측정치와 비슷할 경우 가장 정확도 높음
        ina219_1.setCalibration_32V_1A();
        ina219_2.setCalibration_32V_1A();
        ina219_3.setCalibration_32V_1A();
        ina219_4.setCalibration_32V_1A();
        ina219_1.begin();
        ina219_2.begin();
        ina219_3.begin();
        ina219_4.begin();
    }
    float operate(int num)
    {
        switch (num)
        {
        case 0:
            j[num] = ina219_1.getCurrent_mA();
            if (Serial_Debug)
            {
                Serial.print(F("1번 다리 : "));
                Serial.print(j[0]);
                Serial.print(F("   "));
            }
            break;

        case 1:
            j[num] = ina219_2.getCurrent_mA();
            if (Serial_Debug)
            {
                Serial.print(F("2번 다리 : "));
                Serial.print(j[1]);
                Serial.print(F("   "));
            }
            break;
        case 2:
            j[num] = ina219_3.getCurrent_mA();
            if (Serial_Debug)
            {
                Serial.print(F("3번 다리 : "));
                Serial.print(j[2]);
                Serial.print(F("   "));
            }
            break;
        case 3:
            j[num] = ina219_4.getCurrent_mA();
            if (Serial_Debug)
            {
                Serial.print(F("4번 다리 : "));
                Serial.println(j[3]);
            }
            break;
        }
        return j[num];
    }
};