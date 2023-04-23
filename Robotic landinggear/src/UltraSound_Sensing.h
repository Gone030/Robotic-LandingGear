#include <Arduino.h>

#define TRIG 8
#define ECHO 9         // 초음파센서 핀
#define Stack_limit 25 // 이상 스택 쌓이면 mode 2
class Ultra
{
private:
    unsigned int stack = 0;
    float readings_bf, duration, distance;
public:
    Ultra() {}
    ~Ultra() {}
    void set()
    {
        pinMode(TRIG, OUTPUT);
        pinMode(ECHO, INPUT);

        digitalWrite(TRIG, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG, LOW);
        duration = (float)pulseIn(ECHO, HIGH);
        distance = ((float)(340 * duration) / 10000) / 2;
        distance = distance > 100 ? 100 : distance;
        readings_bf = distance;
    }
    bool operate()
    {
        digitalWrite(TRIG, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG, LOW);
        duration = (float)pulseIn(ECHO, HIGH);
        distance = ((float)(340 * duration) / 10000) / 2;
        distance = distance > 100 ? 100 : distance;
        if (readings_bf > distance) //가까워지면 스택 증가
        {
            stack++;
        }
        else if (readings_bf < distance) //멀어지면 스택 감소
        {
            if (stack > 1)
            {
                stack--;
            }
        }
        readings_bf = distance;

        if (stack > Stack_limit)
        {
            return true;
        }
        return false;
    }
    int return_distance() { return distance; }
};