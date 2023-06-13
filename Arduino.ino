

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// RemoteXY select connection mode and include library
#define REMOTEXY_MODE__HARDSERIAL

#include <RemoteXY.h>

// RemoteXY connection settings
#define REMOTEXY_SERIAL Serial
#define REMOTEXY_SERIAL_SPEED 9600


// RemoteXY configurate
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =  // 234 bytes
  { 255, 7, 0, 12, 0, 227, 0, 16, 31, 1, 130, 0, 0, 23, 63, 19, 180, 130, 0, 22,
    0, 19, 68, 80, 130, 0, 0, 68, 63, 16, 13, 130, 0, 0, 85, 63, 15, 29, 1, 0,
    23, 5, 17, 17, 119, 31, 70, 87, 68, 0, 1, 0, 3, 24, 17, 17, 28, 31, 76, 69,
    70, 84, 0, 1, 0, 43, 24, 17, 17, 28, 31, 82, 73, 71, 72, 84, 0, 1, 0, 23,
    45, 17, 17, 1, 31, 66, 87, 68, 0, 4, 128, 4, 90, 56, 7, 2, 26, 67, 5, 22,
    34, 19, 8, 2, 26, 11, 129, 0, 23, 27, 17, 5, 25, 83, 80, 69, 69, 68, 0, 4,
    176, 1, 75, 62, 6, 2, 26, 129, 0, 22, 87, 17, 5, 25, 83, 80, 69, 69, 68, 0,
    129, 0, 15, 70, 33, 5, 25, 83, 101, 114, 118, 111, 32, 67, 111, 110, 116, 114, 111, 108,
    0, 129, 0, 50, 71, 9, 4, 8, 62, 62, 62, 62, 0, 129, 0, 4, 71, 9, 4, 8,
    60, 60, 60, 60, 0, 10, 48, 5, 7, 12, 12, 4, 26, 31, 97, 117, 116, 111, 109, 97,
    116, 105, 99, 0, 31, 109, 97, 110, 117, 97, 108, 0, 129, 0, 5, 2, 10, 4, 17, 99,
    111, 110, 116, 114, 111, 108, 108, 0, 65, 65, 46, 6, 11, 12 };

// this structure defines all the variables and events of your control interface
struct {

  // input variables
  uint8_t button_Forward;   // =1 if button pressed, else =0
  uint8_t button_Left;      // =1 if button pressed, else =0
  uint8_t button_Right;     // =1 if button pressed, else =0
  uint8_t button_Backward;  // =1 if button pressed, else =0
  int8_t slider_1;          // =0..100 slider position
  int8_t slider_2;          // =-100..100 slider position
  uint8_t pushSwitch_1;     // =1 if state is ON, else =0

  // output variables
  char text_Speed[11];  // string UTF8 end zero
  uint8_t led_1_r;      // =0..255 LED Red brightness

  // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////


#include <Servo.h>
#include <AFMotor.h>
#define Echo A1
#define Trig A0
#define motor 10
#define Speed 170
#define spoint 103
char value;
int distance;
int Left;
int Right;
int L = 0;
int R = 0;
int L1 = 0;
int R1 = 0;
const int sensorPin = A2;
boolean val = 0;
Servo servo;
AF_DCMotor M1(1);
AF_DCMotor M2(2);
AF_DCMotor M3(3);
AF_DCMotor M4(4);

void setup() {
  RemoteXY_Init();

  Serial.begin(9600);
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  pinMode(sensorPin, INPUT);

  servo.attach(motor);
  M1.setSpeed(Speed);
  M2.setSpeed(Speed);
  M3.setSpeed(Speed);
  M4.setSpeed(Speed);
  // TODO you setup code
}
// Mic code 
void loop() {
  RemoteXY_Handler();
  val = digitalRead(sensorPin);
  if (val == 1) {
    delay(2000);
    RemoteXY.led_1_r = 255;
  } else {
    RemoteXY.led_1_r = 0;
  }
  if (RemoteXY.pushSwitch_1 == 1) {
    Obstacle();
  } else {
    Bluetoothcontrol();
  }
}
void Bluetoothcontrol() {
  if (RemoteXY.button_Forward == 1) {
    forward();
  } else if (RemoteXY.button_Backward == 1) {
    backward();
  } else if (RemoteXY.button_Left == 1) {
    left();
  } else if (RemoteXY.button_Right == 1) {
    right();
  } else {
    Stop();
  }
}
void Obstacle() {
  distance = ultrasonic();
  if (distance <= 12) {
    Stop();
    backward();
    delay(100);
    Stop();
    L = leftsee();
    servo.write(spoint);
    delay(800);
    R = rightsee();
    servo.write(spoint);
    if (L < R) {
      right();
      delay(500);
      Stop();
      delay(200);
    } else if (L > R) {
      left();
      delay(500);
      Stop();
      delay(200);
    }
  } else {
    forward();
  }
}
// Ultrasonic sensor distance reading function
int ultrasonic() {
  digitalWrite(Trig, LOW);
  delayMicroseconds(4);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  long t = pulseIn(Echo, HIGH);
  long cm = t / 29 / 2;  //time convert distance
  return cm;
}
void forward() {
  M1.run(FORWARD);
  M2.run(FORWARD);
  M3.run(FORWARD);
  M4.run(FORWARD);
}
void backward() {
  M1.run(BACKWARD);
  M2.run(BACKWARD);
  M3.run(BACKWARD);
  M4.run(BACKWARD);
}
void right() {
  M1.run(BACKWARD);
  M2.run(BACKWARD);
  M3.run(FORWARD);
  M4.run(FORWARD);
}
void left() {
  M1.run(FORWARD);
  M2.run(FORWARD);
  M3.run(BACKWARD);
  M4.run(BACKWARD);
}
void Stop() {
  M1.run(RELEASE);
  M2.run(RELEASE);
  M3.run(RELEASE);
  M4.run(RELEASE);
}
int rightsee() {
  servo.write(20);
  delay(800);
  Left = ultrasonic();
  return Left;
}
int leftsee() {
  servo.write(180);
  delay(800);
  Right = ultrasonic();
  return Right;
}