#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int trig = 8;
const int echo = 7;

void setup()
{
  Serial.begin(9600); // giao tiếp series qua cổng usb giữa arduino với lap với baudrate = 9600
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Distance:");
  
  lcd.setCursor(13, 0);
  lcd.print(" cm");
  
  pinMode(trig, OUTPUT); // chân trig sẽ phát tín hiệu
  pinMode(echo, INPUT); // chân echo sẽ nhận tín hiệu
}

void loop()
{
  unsigned long duration; // biến đo thời gian
  int distance; // biến lưu khoảng cách

  /* Phát xung từ chân trig */
  digitalWrite(trig, 0); // tắt chân trig bằng cách ghi giá trị LOW
  delayMicroseconds(2); // xung có độ dài 2 microSeconds
  digitalWrite(trig, 1); // phát xung từ chân trig bằng cách ghi giá trị HIGH
  delayMicroseconds(5); // xung có độ dài 5 microSeconds
  digitalWrite(trig, 0); // tắt chân trig bằng cách ghi giá trị LOW

   /* Tính toán thời gian và khoảng cách*/
  duration = pulseIn(echo, HIGH); // Đo độ rộng xung ở chân echo khi ở trạng thái HIGH
  distance = int(duration / 2 / 29.412); // Tính khoảng cách đến vật. 29,412 microSeconds/cm = (10^6 / (340*100)) để chuyển đổi từ micro giây sang cm

  /* In kết quả ra Serial Monitor */
  lcd.setCursor(10, 0);
  lcd.print(distance); // Đặt con trỏ và hiển thị giá trị khoảng cách trên màn hình.
  if (distance < 10) { // Kiểm tra và làm sạch vị trí thừa nếu khoảng cách là một số có một chữ số.
    lcd.setCursor(11, 0);
    lcd.print("  ");
  } 
  else if (distance < 100) { // Kiểm tra và làm sạch vị trí thừa nếu khoảng cách là một số có hai chữ số.
    lcd.setCursor(12, 0);
    lcd.print(' ');
  } 
    else if (distance < 1000) { // Kiểm tra và làm sạch vị trí thừa nếu khoảng cách là một số có ba chữ số.
    lcd.setCursor(13, 0);
    lcd.print(' ');
  } 
  delay(200);
}
