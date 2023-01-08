//1.01 Beta
//////////////////////////////////////////
// Flipper Zero Arduino 7-1-2023     ///
//       MaDe By DeXtErBoT  TX+RX      ///
//////////////////////////////////////////
// wil store 2 keys in eeprom after recieved one or 2 , NOTE !
// third key wil replacing the first key again onley 2 keys max fit in storage //
// wil remember the keys after power off , and reload them again //
// to use the memory storage keyes press left for using storage 1 and right for storage 2 key //
// it also wil store the bit rate type of the key //
// pushing joystick up wil transmit a Tesla charge point key //
// fast push button wil send you to menu ,in menu you do 2 second long press to get in the selected subject , RX or TX or BACK //
// wil make things better , enjoy this Beta version .//
///////////////////////////
//#include <HIDKeyboard.h>
//HIDKeyboard keyboard;
//#define debug1  // debug  //debug1 for analog inputs //
//#define badusb  // Bad USB ON !
////////////////////////////////////////////
//#define KEY_DELAY 50
//const char command [] = "echo hello hackers";
//#include <IRremote.h>
////////////////////////////////////////////
#include <EEPROM.h>
////////////////////////////////////////////
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
#define ARDUINO_RESET 5
Adafruit_SSD1306 display(OLED_RESET);
////////////////////////////////////////////////////////
#include "images.h"  // images i use
#include "TeslaBeacon.h" // beacon code for tesla charge port
//////////////////////////////////
#define signalPin  2
#define ledPinP 13
#define pushButton 5
#define screensavertime 30 //screensaver popup time //
//////////////////////////////////////////////
#define analogInPin1 A0  // Analog input pin that the potentiometer is attached to
#define analogInPin2 A1  // Analog input pin that the potentiometer is attached to
uint16_t sensorValue1 = 0;
uint16_t sensorValue2 = 0;
uint8_t buff = 0;
uint8_t selection = 0;
uint8_t saverscr = 0;
uint8_t buttonState = 0;
int store1 = 0;
uint8_t bitR1 = 0;
uint8_t protoc = 0;
uint8_t menu = 1;
uint8_t sellect = 0;
//////////////////////////////////////////////////////////////////////
uint8_t eepromcount = 0;
////////////////////////////////////////////////////////////////////////////////////////////////////
int lastButtonState = LOW;   // the previous reading from the input pin
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 80;    // the debounce time; increase if the output flickers
int reading = 0;
////////////////////////////////////
#include <RCSwitch.h>
RCSwitch mySwitch = RCSwitch();
////////////////////////////////////////////////
void setup() {
  // IrReceiver.begin(12);
  pinMode(pushButton, INPUT);
  digitalWrite(pushButton, INPUT_PULLUP);
#ifdef badusb
  keyboard.begin();
  delay(2000);
#endif
  pinMode(ledPinP, OUTPUT);
  pinMode(signalPin, OUTPUT);
  digitalWrite(signalPin, LOW);
  /// Oled settings //////////////////////////
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);//3d or 3c
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.clearDisplay();
  /////////////////////////////////////
  delay(100);
  Serial.begin(9600);  // serial speed for BT device default 9600
  display.clearDisplay();
  display.drawBitmap(0, 0, FlipperLogo, 128, 64, 1);
  display.display();
  delay (1500);
  display.clearDisplay();
  display.drawBitmap(0, 0, Ardulogo, 128, 64, 1);
  display.display();
  delay (1000);
  display.clearDisplay();
  display.drawBitmap(0, 0, madeby, 128, 64, 1);
  display.display();
  delay (1500);
  digitalWrite(ledPinP, LOW);
  mySwitch.enableReceive(1);  // Receiver on interrupt 0 => that is pin #2
  mySwitch.enableTransmit(signalPin);
}

void loop() {
  sensorValue1 = analogRead(analogInPin1);
  sensorValue2 = analogRead(analogInPin2);
  buttonreading();
  if (selection == 1) {
    menw();
    return;
  }
  Serial.print("|"); ////////// serial out !!
  Serial.print("0"); ////////// serial out !!
  Serial.print("|"); ////////// serial out !!
  Serial.println("0"); ////////// serial out !!
  saverscr = saverscr + 1;
  if (saverscr >= screensavertime )screensaver();
  ////// Serial BT input handler //////////////////////////////////////////////////
  while (Serial.available() > 0) {
    buff = Serial.read();
#ifdef debug
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Received ");
    display.setCursor(0, 20);
    display.print(buff);
    display.display();
    delay(1000);
#endif
    display.clearDisplay();
    display.drawBitmap(0, 0, myBitmapC, 128, 64, 1);
    display.display();
    //delay(200);
    if (buff == 1)select = 1;
    if (buff == 2)select = 2;
    if (buff == 3)select = 3;
  }
  if (select == 1) {
    rfrx();
    return;
  }
  if (select == 2) {
    Serial.print("|"); ////////// serial out !!
    Serial.print("4"); ////////// serial out !!
    Serial.print("|"); ////////// serial out !!
    Serial.println(store1); ////////// serial out !!
    display.clearDisplay();
    display.drawBitmap(0, 0, myBitmapC, 128, 64, 1);
    display.display();
    delay(800);
    mySwitch.send(store1, bitR1);
    select = 0;
  }
  if (select == 3) {
    select = 0;
    return;
  }
  //////////////////////////////////////////////////////////////////////////////////
  digitalWrite(ledPinP, LOW);
  display.clearDisplay();
  display.drawBitmap(0, 0, FlipperLogo, 128, 64, 1);
  display.display();
  // delay (100);
  if (sensorValue1 > 600 )teslachargeport();
  if (sensorValue1 < 500 )screensaver ();
  if (sensorValue2 > 600 )TX1();//screensaver ();
  if (sensorValue2 < 500 )TX2();//screensaver ();
#ifdef debug1
  if (sensorValue1 > 600 )Serial.println("UP");
  if (sensorValue1 < 500 )Serial.println("DOWN");
  if (sensorValue2 > 600 )Serial.println("RIGHT");
  if (sensorValue2 < 500 )Serial.println("LEFT");
#endif
  digitalWrite(ledPinP, LOW);
}

///////////////////////////////////////////////////////////////////////////////////////////////
void teslachargeport() {
  for (uint8_t t = 0; t < transmissions; t++) {
    display.clearDisplay();
    display.drawBitmap(0, 20, Teslalogo, 128, 36, 1);
    display.display();
    delay (500);
    for (uint8_t i = 0; i < messageLength; i++) sendByte(sequence[i]);
    digitalWrite(signalPin, LOW);
    delay(messageDistance);
    Serial.print("|"); ////////// serial out !!
    Serial.print("1"); ////////// serial out !!
    Serial.print("|"); ////////// serial out !!
    Serial.println("0"); ////////// serial out !!
    select = 1;
  }
  display.clearDisplay();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////// tesla send byte part ////
void sendByte(uint8_t dataByte) {
  digitalWrite(ledPinP, HIGH);
  for (int8_t bit = 7; bit >= 0; bit--) { // MSB
    digitalWrite(signalPin, (dataByte & (1 << bit)) != 0 ? HIGH : LOW);
    delayMicroseconds(pulseWidth);
  }
  digitalWrite(ledPinP, HIGH);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TX1() {
  long number1 = readLongFromEEPROM(100);
  int8_t Bitr1 = EEPROM.read(205);
  Serial.print("Number: ");
  Serial.println(number1);
  Serial.print("Bitrate: ");
  Serial.println(Bitr1);
  Serial.print("|"); ////////// serial out !!
  Serial.print("4"); ////////// serial out !!
  Serial.print("|"); ////////// serial out !!
  Serial.println(number1); ////////// serial out !!
  display.clearDisplay();
  display.drawBitmap(0, 0, myBitmapC, 128, 64, 1);
  display.display();
  delay(800);
  mySwitch.send(number1, Bitr1);
  select = 0;
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.clearDisplay();
  display.drawBitmap(0, 0, rximage, 128, 64, 1);
  display.setCursor(68, 17);
  display.print(number1);
  display.setCursor(70, 29);
  display.print("  FSK");
  display.setCursor(70, 43);
  display.print("TRANMIT");
  display.display();
  delay(3000);
}

void TX2() {
  long number2 = readLongFromEEPROM(200);
  int8_t Bitr2 = EEPROM.read(206);
  Serial.print("Number1: ");
  Serial.println(number2);
  Serial.print("Bitrate: ");
  Serial.println(Bitr2);
  Serial.print("|"); ////////// serial out !!
  Serial.print("4"); ////////// serial out !!
  Serial.print("|"); ////////// serial out !!
  Serial.println(number2); ////////// serial out !!
  display.clearDisplay();
  display.drawBitmap(0, 0, myBitmapC, 128, 64, 1);
  display.display();
  delay(800);
  mySwitch.send(number2, Bitr2);
  select = 0;
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.clearDisplay();
  display.drawBitmap(0, 0, rximage, 128, 64, 1);
  display.setCursor(68, 17);
  display.print(number2);
  display.setCursor(70, 29);
  display.print("  FSK");
  display.setCursor(70, 43);
  display.print("TRANMIT");
  display.display();
  delay(3000);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void menw() {
  buttonreading();
  delay(100);
  if (sensorValue1 > 600 )menu = menu - 1;
  if (sensorValue1 < 500 )menu = menu + 1;
  if (menu > 3)menu = 1;
  if (menu < 1)menu = 3;
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(10, 3);
  display.print("    MENU SELECT  ");
  display.setCursor(10, 18);
  display.print("RX Code ");;
  display.setCursor(10, 36);
  display.print("TX Code ");
  display.setCursor(10, 56);
  display.print("Back");
  display.drawBitmap(65, 15, badboy, 61, 65, 1);
  display.drawLine(0, 14, 128, 14, WHITE);
  display.drawLine(0, 30, 50, 30, WHITE);
  display.drawLine(0, 50, 50, 50, WHITE);
  display.display();
  if (menu == 1) {
    display.setTextColor(WHITE);
    display.setCursor(0, 20);
    display.print("*");;
    display.display();
    delay(50);
    display.setTextColor(BLACK);
    display.setCursor(0, 20);
    display.print(" ");
    display.display();
    delay(50);
    select = 1;
  }
  if (menu == 2) {
    display.setTextColor(WHITE);
    display.setCursor(0, 40);
    display.print("*");
    display.display();
    delay(50);
    display.setTextColor(BLACK);
    display.setCursor(0, 40);
    display.print(" ");
    display.display();
    delay(50);
    select = 2;
  }
  if (menu == 3) {
    display.setTextColor(WHITE);
    display.setCursor(0, 60);
    display.print("*");
    display.display();
    delay(50);
    display.setTextColor(BLACK);
    display.setCursor(0, 60);
    display.print(" ");
    display.display();
    delay(50);
    select = 0;
  }
  // reading = digitalRead(pushButton);
  // if (reading = LOW) return;
}
////////////////////////////
void screensaver () {
  saverscr = 0;
  display.clearDisplay();
  display.drawBitmap(0, 0, screensvrimage, 128, 64, 1);
  display.display();
  delay (1000);
  Serial.print("|"); ////////// serial out !!
  Serial.print("2"); ////////// serial out !!
  Serial.print("|"); ////////// serial out !!
  Serial.println("0"); ////////// serial out !!
}
//////////////////////////////////////////////////////////////////////////
void rfrx () {
  if (mySwitch.available()) {
    saverscr = 0;
    Serial.print("|"); ////////// serial out !!
    Serial.print("3"); ////////// serial out !!
    Serial.print("|"); ////////// serial out !!
    Serial.println("0"); ////////// serial out !!
    int value = mySwitch.getReceivedValue();
    display.clearDisplay();
    display.drawBitmap(64, 0, remoto, 64, 64, 1);
    display.setCursor(0, 0);
    display.print("Received ");
    display.setCursor(0, 20);
    display.print( mySwitch.getReceivedValue() );
    display.setCursor(0, 30);
    display.print( mySwitch.getReceivedBitlength() );
    display.print("bit ");
    display.setCursor(0, 50);
    display.print("Protocol: ");
    display.println(mySwitch.getReceivedProtocol() );
    display.display();
    store1 = mySwitch.getReceivedValue();
    bitR1 = mySwitch.getReceivedBitlength();
    protoc = mySwitch.getReceivedProtocol();
    eepromcount = eepromcount + 1;
    delay(2000);
    if (eepromcount >= 3)eepromcount = 1;
    if (eepromcount == 1) {
      EEPROM.write(205, bitR1);
      writeLongIntoEEPROM(100, store1);
      display.clearDisplay();
      display.setCursor(15, 20);
      display.print("   Stored 1 ");
      display.display();
      delay(500);
    }
    if (eepromcount == 2) {
      EEPROM.write(206, bitR1);
      writeLongIntoEEPROM(200, store1);
      display.clearDisplay();
      display.setCursor(15, 20);
      display.print("  Stored 2 ");
      display.display();
      delay(500);
    }

    Serial.print("|"); ////////// serial out !!
    Serial.print("3"); ////////// serial out !!
    Serial.print("|"); ////////// serial out !!
    Serial.println(store1); ////////// serial out !!
    if (value == 0) {
      display.clearDisplay();
      display.setCursor(0, 20);
      display.print("Unknown encoding");
      display.display();
      delay(2500);
    }
    Serial.print("|"); ////////// serial out !!
    Serial.print("4"); ////////// serial out !!
    Serial.print("|"); ////////// serial out !!
    Serial.println("0"); ////////// serial out !!
    mySwitch.resetAvailable();
  }
  delay(100);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(5, 3);
  display.print("  RECEIVED RF CODE ");
  display.setCursor(10, 20);
  display.print(bitR1);;
  display.setCursor(80, 20);
  display.print("BITS");;
  display.setCursor(10, 30);
  display.print(store1);
  display.setCursor(80, 30);
  display.print("CODE");
  display.setCursor(10, 40);
  display.print(protoc);
  display.setCursor(80, 40);
  display.print("PROTOCAL");
  display.drawLine(0, 14, 128, 14, WHITE);
  display.drawLine(0, 52, 128, 52, WHITE);
  display.display();
  delay(100);
  display.clearDisplay();
}

void writeLongIntoEEPROM(int address, long number)
{
  EEPROM.write(address, (number >> 24) & 0xFF);
  EEPROM.write(address + 1, (number >> 16) & 0xFF);
  EEPROM.write(address + 2, (number >> 8) & 0xFF);
  EEPROM.write(address + 3, number & 0xFF);
}

long readLongFromEEPROM(int address)
{
  return ((long)EEPROM.read(address) << 24) +
         ((long)EEPROM.read(address + 1) << 16) +
         ((long)EEPROM.read(address + 2) << 8) +
         (long)EEPROM.read(address + 3);
}
void buttonreading () {
  lastButtonState = HIGH;
  reading = digitalRead(pushButton);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
  }

  lastButtonState = reading;
  if (reading == LOW) {
    display.clearDisplay();
    display.drawBitmap(35, 15, Thumb, 64, 64, 1);
    display.display();
    delay(200);
    if (selection == 1) {
      selection = 0;
      return;
    }
    selection = 1;
  }
}
