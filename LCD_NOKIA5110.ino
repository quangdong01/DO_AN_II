#include <Wire.h>
#include "Nokia_5110.h"

// Dinh nghia cac chan cua Arduino Nano
#define RST 12
#define CE  11
#define DC  10
#define DIN 9
#define CLK 8
#define BL  7

Nokia_5110 lcd = Nokia_5110(RST, CE, DC, DIN, CLK);

/* Địa chỉ của DS1307 */
const byte DS1307 = 0x68;

/* Số byte dữ liệu sẽ đọc từ DS1307 */
const byte NumberOfFields = 7;
 
/* khai báo các biến thời gian */
int second, old_second, minute, hour, day, wday, month, year;
 
void setup()
{
  Wire.begin();
  /* cài đặt thời gian cho module */
  setTime(14, 30, 45, 3, 14, 3, 23); // 12:30:45 CN 08-02-2015
  Serial.begin(9600);

  pinMode(BL, OUTPUT);
  digitalWrite(BL, 1);

  // thiet lap do sang 
  lcd.setContrast(55);
  // di chuyen vi tri hien thi toi toa do (10, 0)
  lcd.setCursor(10, 0);
  // Hien thi dong chu "DS1307 CLOCK"
  lcd.print("DS1307 CLOCK");

  // di chuyen vi tri hien thi toi toa do (0, 1)
  lcd.setCursor(0, 1);
  // Hien thi dong chu "NOKIA 5110 LCD"
  lcd.print("BACH KHOA HANOI");
             
}
 
void loop()
{
  /* Đọc dữ liệu của DS1307 */
  readDS1307();
  // hien thi thoi gian ra LCD
  show();
  digitalClockDisplay();
  delay(1000);
}
 


void show()
{
    if (old_second != second)
    {
      old_second = second;
      lcd.setCursor(30, 2);
      lcd.print("DATE");
      lcd.setCursor(14, 3);
      if(day < 10)
      {
        lcd.print("0");
        lcd.print(day);
      } 
      else
      {
        lcd.print(day);
      }
      lcd.print("/");
      if(month < 10)
      {
        lcd.print("0");
        lcd.print(month);
      }
      else
      {
        lcd.print(month);
      }
      lcd.print("/");
      lcd.print(year);

      lcd.setCursor(30, 4);
      lcd.print("TIME");
      lcd.setCursor(20, 5);
      


      if(hour < 10)
      {
        lcd.print("0");
        lcd.print(hour);
      } 
      else
      {
        lcd.print(hour);
      }
      lcd.print(":");
      if(minute < 10)
      {
        lcd.print("0");
        lcd.print(minute);
      }
      else
      {
        lcd.print(minute);
      }
      lcd.print(":");
      if(second < 10)
      {
        lcd.print(0);
        lcd.print(second);
      }
      else
      {
        lcd.print(second);
      }
    }
}

// Ham doc thoi gian thuc
void readDS1307()
{
        Wire.beginTransmission(DS1307);
        Wire.write((byte)0x00);
        Wire.endTransmission();
        Wire.requestFrom(DS1307, NumberOfFields);
        
        second = bcd2dec(Wire.read() & 0x7f);
        minute = bcd2dec(Wire.read() );
        hour   = bcd2dec(Wire.read() & 0x3f); // chế độ 24h.
        wday   = bcd2dec(Wire.read() );
        day    = bcd2dec(Wire.read() );
        month  = bcd2dec(Wire.read() );
        year   = bcd2dec(Wire.read() );
        year += 2000;    
}
/* Chuyển từ format BCD (Binary-Coded Decimal) sang Decimal */
int bcd2dec(byte num)
{
        return ((num/16 * 10) + (num % 16));
}
/* Chuyển từ Decimal sang BCD */
int dec2bcd(byte num)
{
        return ((num/10 * 16) + (num % 10));
}
 
void digitalClockDisplay(){
    // digital clock display of the time
    Serial.print(hour);
    printDigits(minute);
    printDigits(second);
    Serial.print(" ");
    Serial.print(day);
    Serial.print(" ");
    Serial.print(month);
    Serial.print(" ");
    Serial.print(year); 
    Serial.println(); 
}
 
void printDigits(int digits){
    // các thành phần thời gian được ngăn chách bằng dấu :
    Serial.print(":");
        
    if(digits < 10)
        Serial.print('0');
    Serial.print(digits);
}
 
/* cài đặt thời gian cho DS1307 */
void setTime(byte hr, byte min, byte sec, byte wd, byte d, byte mth, byte yr)
{
        Wire.beginTransmission(DS1307);
        Wire.write(byte(0x00)); // đặt lại pointer
        Wire.write(dec2bcd(sec));
        Wire.write(dec2bcd(min));
        Wire.write(dec2bcd(hr));
        Wire.write(dec2bcd(wd)); // day of week: Sunday = 1, Saturday = 7
        Wire.write(dec2bcd(d)); 
        Wire.write(dec2bcd(mth));
        Wire.write(dec2bcd(yr));
        Wire.endTransmission();
}