#include "SPI.h"
#include "Wire.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include <Servo.h>     
Servo myservo; 
int pos = 0;

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

int sensor = 14;
int value;
int servo = 0;
int status = 0;
int ENA = 12;
int IN1 = 13;
int IN2 = 15;
int speed = 0;
int echoPin = 2;                                    
int initPin = 3;                            
unsigned long pulseTime = 0;          
unsigned long distance = 0;  

void setup() {
  Serial.begin(9600);
  pinMode(sensor, INPUT);
  pinMode(initPin, OUTPUT); 
  pinMode(echoPin, INPUT);
  myservo.attach(servo);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void loop() {
  printMessage("On Station!");
  delay(15000);
  closeDoor();
  delay(10000);
  openDoor2();
  slowDown();
  delay(15000);
}
 
void printMessage(String mess) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.clearDisplay();
  display.println(mess);
  display.display();
}

void closeDoor () {
  printMessage("Closing...");
  for(pos = 0; pos < 180; pos += 1){ 
        myservo.write(pos);
        delay(15);    
        value = digitalRead(sensor);
        if(value == 0){
          break;
          
        }
  }
  if(value == 0) {
    interupt();
  } else {
    status = 0;
    printMessage("Closed!");
    myservo.attach(2);
    speedUp();
  }
  
}

void openDoor () {
  myservo.attach(servo);
  printMessage("Opening...");
  for(pos = 180; pos>=1; pos-=1) {                           
        myservo.write(pos);
        delay(15);  
  } 
  status = 1;
  printMessage("Opened!");
  delay(500);
  printMessage("On Station!");
}

void interupt () {
  printMessage("Opening...");
  Serial.println("Interupt!");
  for(pos; pos>=1; pos-=1) {
    myservo.write(pos);
    delay(15);
  }
  status = 1;
  printMessage("Opened!");
  delay(5000);
  closeDoor();
}

void speedUp() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  printMessage("Speed up...");
  for (speed = 0; speed <= 255; speed++) {
    analogWrite(ENA, speed);
    delay(20);
    }
  printMessage("Current Speed");  
}

void slowDown() {
  printMessage("Slow down...");
  for (speed ; speed >= 0; speed = speed - 10) {
    analogWrite(ENA, speed);
    delay(20);
    if(speed == 0) {
      
      break;
    }
  }
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  delay(1000);
  openDoor();
}

