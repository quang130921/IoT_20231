#define BLYNK_TEMPLATE_ID "TMPL6CSZEoWQ9"
#define BLYNK_TEMPLATE_NAME "AutoPeopleMover"
#define BLYNK_AUTH_TOKEN "JvEx_NaxsBfbrlq3ANtPDeXxEttWeFbc"

enum class TrainState {
  STOP,
  SPEEDUP,
  RUNNING,
  SLOWDOWN
};

enum class Position {
  STATION1,
  STATION2
};

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>

#define BLYNK_PRINT Serial

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels



char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "GodC"; 
char pass[] = "00000111";


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int doorSensor = 2;
int ENA = 14;
int IN1 = 12;
int IN2 = 13;

int increment = 15;
bool isAutomatic = false;
int value;
int pos = 0;
int speed = 0;
int doorState = 0;
TrainState status = TrainState::STOP;
Position position = Position::STATION1;
Servo myservo;

void setup() {
  Serial.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);

  myservo.attach(15);
  myservo.write(0);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW); 

  Blynk.begin(auth, ssid, pass);
}

void printMessage(String message) {
  Serial.println(message);
  display.setCursor(0, 0);
  display.clearDisplay();
  display.println(message);
  display.display();
}

void loop() {
  Blynk.run();
  printMessage("Tau van chuyen nhom ");
  Serial.println("start check automatic");
  delay(5000);
  Serial.println("end check automatic");
  if(isAutomatic) {
    automatic();
  } else {
    if(status != TrainState::STOP) {
      slowDown();
    }
  }
}

BLYNK_WRITE(V3)
{
  int buttonState = param.asInt();
  if(status != TrainState::STOP) {
    if (buttonState == HIGH)
    {
      if (speed < 255 - increment)
      {
        speed += increment;
      }
      else
      {
        speed = 255;
      }
      if(position == Position::STATION1) {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      } else {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      }
      analogWrite(ENA, speed);
      Blynk.virtualWrite(V3, speed);
    }
  }
}

BLYNK_WRITE(V4)
{
  int buttonState = param.asInt();
  if(status != TrainState::STOP) {
    if (buttonState == HIGH)
    {
      if (speed > increment)
      {
        speed -= increment;
      }
      else
      {
        speed = 0;
      }
      if(position == Position::STATION1) {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
      } else {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
      }
      analogWrite(ENA, speed);
      Blynk.virtualWrite(V3, speed);
    }
  }
}

BLYNK_WRITE(V1)
{
  int buttonState = param.asInt();
  if (buttonState == HIGH)
  { 
    isAutomatic = true;
  }
  else
  {
    isAutomatic = false;
  }
}

BLYNK_WRITE(V0)
{
  int buttonState = param.asInt();
  if(status == TrainState::STOP) {
    if (buttonState == HIGH)
    { 
      closeDoor();
    }
  else
    {
      openDoor();
    }
  }
}

void automatic() {
  //closing door
  closeDoor();
  //speed up
  speedUp();
  //running
  running();
  //slow down
  slowDown();
  //opening door
  openDoor();
}

void running() {
  //time running with max speed
  printMessage("Dang chay...");
  status = TrainState::RUNNING;
  for(int time = 0; time <= 5000; time = time + 250) {
    analogWrite(ENA, speed);
    Serial.print("Speed: ");
    Serial.println(speed);
    Serial.println(time);
    Blynk.virtualWrite(V2, speed);
    delay(250);
    if(time == 5000) {
      break;
    }
  }
  Serial.println("After running");
}

void closeDoor () {
  if(doorState == 0) {
    printMessage("Dang dong cua");
    for(pos = 0; pos < 180; pos += 1){ 
      myservo.write(pos);
      delay(15);    
      value = digitalRead(doorSensor);
      if(value == 0){
        break;
      }
    }
    if(value == 0) {
      interupt();
    } else {
      printMessage("Cua da dong");
      doorState = 1;
      Blynk.virtualWrite(V0, 1);
      myservo.attach(16);
    }
  }
}

void openDoor () {
  if(doorState == 1) {
    myservo.attach(15);
    printMessage("Dang mo cua");
    for(pos = 180; pos>=1; pos-=1) {                           
      myservo.write(pos);
      delay(15);  
    } 
    status = TrainState::STOP;
    doorState = 0;
    Blynk.virtualWrite(V0, 0);
    printMessage("Cua da mo");
  }
}

void interupt () {
  printMessage("Dang mo cua");
  for(pos; pos>=1; pos-=1) {
    myservo.write(pos);
    delay(15);
  }
  status = TrainState::STOP;
  printMessage("Cua da mo");
  delay(5000);
  closeDoor();
}

void speedUp() {
  status = TrainState::SPEEDUP;
  if(position == Position::STATION1) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  } else {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  }
  printMessage("Dang tang toc...");
  for (speed = 0; speed <= 255; speed++) {
    analogWrite(ENA, speed);
    Blynk.virtualWrite(V2, speed);
    delay(15);
  }
}

void slowDown() {
  printMessage("Dang giam toc...");
  for (speed ; speed >= 0; speed = speed - 10) {
    Blynk.virtualWrite(V2, speed);
    analogWrite(ENA, speed);
    delay(15);
    if(speed == 0) {
      
      break;
    }
  }
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  if(position == Position::STATION1) {
    position = Position::STATION2;
  } else {
    position = Position::STATION1;
  }
  delay(700);
}




