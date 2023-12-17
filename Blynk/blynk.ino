#define BLYNK_TEMPLATE_ID "TMPL6CSZEoWQ9"
#define BLYNK_TEMPLATE_NAME "AutoPeopleMover"
#define BLYNK_AUTH_TOKEN "JvEx_NaxsBfbrlq3ANtPDeXxEttWeFbc"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SerialESP8266wifi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>


#define BLYNK_PRINT Serial

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels


char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "abcd"; 
char pass[] = "1234567889";

int ENA = 14;
int IN1 = 12;
int IN2 = 13;

int increment = 15;
bool isAutomatic = false;
int speed = 0;
TrainState status = TrainState::STOP;
Position position = Position::STATION1;

void setup() {
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run();
}

BLYNK_WRITE(V3)
{
  int buttonState = param.asInt();
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

BLYNK_WRITE(V4)
{
  int buttonState = param.asInt();
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




