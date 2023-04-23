#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <math.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
RF24 radio(7, 8);

#define Servo_freq 50
#define Servo_AdditionalAngle 20
#define Servo_DelayLanding 10
#define Servo_DelayLittle 30
#define Servo_Land_DelayTime 0
#define Servo_TimeSet_DelayTime 5000

#define Echo_ExploreDefaultValue 260
#define Echo_AdditonalDis 30
#define Echo_DelayTime 20

#define Pressure_DefaultValue 150
#define Pressure_DelayTime 2000

#define Radio_DelayTime 200
#define Radio_On 0b11110000
#define Radio_Off 0b100001111
#define Radio_LandingSuccess 0b10010000
#define Radio_LandingFail 0b10000000
#define Radio_EchoInoperative 0b10100000
#define Radio_OK 0b11000000



const byte address[2][6] = {"00001", "00000"};

int Array_set_up[4] = {1, 1, 1, 1};
int Array_set_down[4] = {0, 0, 0, 0};

int Landing_Posibility[4] = {0, 0, 0, 0};
int Landing_Operation_LegNum[4] = {1, 1, 1, 1};
String Landing_Button="off"; //빈칸으로 바꾸기
int Landing_result;

String Radio_Mode = "recieve";
int Radio_DataRecieve;
int Radio_DataSend;

int Servo_angle[4] = {250, 250, 250, 250};
String Servo_mode = "set";
int Servo_error[4];

int trig[4] = {23, 25, 27, 29}; //핀번호
int echo[4] = {22, 24, 26, 28}; //핀번호
String Echo_mode = "explore";
int Echo_error[4];
double Echo_distance[4] = {0, 0, 0, 0};
const double Echo_landDefaultValue[4] = {91, 102, 102, 96};
int Echo_ExploreResult;

int Piezo = 11; //피에조 핀
int Piezo_Tone[] = {2093, 2349, 2637, 2793};

int Button_leg[] = {2, 3, 4, 5};
int Button_leg_error[4];

unsigned int Echo_Time_past;
unsigned int Servo_Time_past;
unsigned int Servo_TimeSet_past;
unsigned int Radio_Time_past;
unsigned int Pressure_Time_past;
unsigned int Time_present;

void setup() {
  Serial.begin(9600);

  radio.begin();
  radio.openReadingPipe(1, address[0]);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();

  for (int count = 0; count <= 3; count += 1) {
    pinMode(trig[count], OUTPUT);
    pinMode(echo[count], INPUT);
    constrain(Servo_angle[count], 250, 500);
    pinMode(Button_leg[count], INPUT_PULLUP);
  }

  pinMode(Piezo, OUTPUT);

  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(Servo_freq);

  Servo_operation("set");
  Echo_Time_past = millis();
  Servo_Time_past = millis();
  Radio_Time_past = millis();
  Pressure_Time_past = millis();
}

void loop() {
  Serial.println();
  Serial.println("----------Main Loop Start----------");
  Serial.print("Landing_Button : ");
  Serial.println(Landing_Button);
  Serial.print("Landing_Operation_LegNum : ");
  Serial.print(Landing_Operation_LegNum[0]); Serial.print(Landing_Operation_LegNum[1]);
  Serial.print(Landing_Operation_LegNum[2]); Serial.println(Landing_Operation_LegNum[3]);
  Serial.print("Servo_mode : "); Serial.println(Servo_mode);
  Serial.print("Echo_mode : "); Serial.println(Echo_mode);
  Serial.print("Echo_Time_past : "); Serial.println(Echo_Time_past);
  Serial.print("Servo_Time_past : "); Serial.println(Servo_Time_past);
  Serial.print("Radio_Time_past : "); Serial.println(Radio_Time_past);

  Time_present = millis();
  Serial.print("Time_present : "); Serial.println(Time_present);

  if (Time_present - Radio_Time_past >= Radio_DelayTime) {//Radio_DataSend에 값이 들어가있으면 전송하고 다시 리시브
    Serial.println("라디오 탐지 시작");
    Radio_operation_decision();
  }


  if (Landing_Button == "on" && Time_present - Echo_Time_past >= Echo_DelayTime) {//초음파센서 작동
    Serial.println("거리 측정 시작");
    Echo_operation_decision();
  }

  if (Landing_Button == "on" && Servo_mode == "land" && Time_present - Servo_Time_past >= Servo_Land_DelayTime) {//서보모터 작동
    Serial.println("land 모드 시작");
    Button_leg_operation();
    Servo_operation_decision();
  }

  Serial.println("----------Main Loop Finish----------");
}
