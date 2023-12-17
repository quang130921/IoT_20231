#include <LiquidCrystal_I2C.h>
#include <Wire.h>
int ENA = 5;
int IN1 = 3;
int IN2 = 4;
const int BUTTON_ON = 6;
const int BUTTON_OFF = 7;
LiquidCrystal_I2C lcd(0x27,16,2);


void setup() {
  lcd.init();
  lcd.backlight();
  lcd.print("Tau dien nhom 3");
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(BUTTON_ON, INPUT);
  pinMode(BUTTON_OFF, INPUT);
  // pinMode(BUTTON_UP, INPUT);
  // pinMode(BUTTON_DOWN, INPUT);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  //Khởi động chức năng serial 
}

void loop() {
  //analogWrite(ENA, 100);
  int Push_button_on_state = digitalRead(BUTTON_ON);
  int Push_button_off_state = digitalRead(BUTTON_OFF);

  if (Push_button_on_state == HIGH) {
    //Khởi động động hệ thống
    
    
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Start...");
    //Đặt tốc độ cho tàu, đi từ trạm đầu tới cuối hết 5s
    analogWrite(ENA, 255);
    delay(5000);    

    //Sau đó giảm tốc độ, đón trả khách trong 5s
    slowDown();
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Slow down");
    delay(5000);

    // Đón trả khách xong lại tăng tốc và tiếp tục đi
    speedUp();
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Speed up...");
    delay(5000);
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Running");
    //lcd.print("ON...");
    //delay(2000);
    //lcd.clear();
    //lcd.print("ON");
  }

  if (Push_button_off_state == HIGH) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    //lcd.print("OFF...");
    //delay(2000);
    //lcd.clear();
    //lcd.print("OFF");
  }
}

void speedUp() {
  for (int i = 155; i  <= 255; i++) {
    analogWrite(ENA, i);
    delay(20);
  }
}

void slowDown() {
  for (int i = 255; i >= 155; i--) {
    analogWrite(ENA, i);
    delay(20);
  }
}
