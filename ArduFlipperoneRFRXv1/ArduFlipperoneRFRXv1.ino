
//////////////////////////////////////////
// Flipper Zero Arduino 5-1-2023     ///
//       MaDe By DeXtErBoT  TX+RX      ///
//////////////////////////////////////////
//#include <HIDKeyboard.h>
//HIDKeyboard keyboard;
//#define debug1  // debug  //debug1 for analog inputs //
//#define badusb  // Bad USB ON !
////////////////////////////////////////////
//#define KEY_DELAY 50
//const char command [] = "echo hello hackers";
//#include <IRremote.h>
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
//#define ledPinM 5
//#define ledPinX 9
#define screensavertime 30 //screensaver popup time //
//////////////////////////////////////////////
#define analogInPin1 A0  // Analog input pin that the potentiometer is attached to
#define analogInPin2 A1  // Analog input pin that the potentiometer is attached to
int sensorValue1 = 0;
int sensorValue2 = 0;
uint8_t buff = 0;
uint8_t selection = 0;
uint8_t saverscr = 0;
uint8_t buttonState = 0;
int store1 = 0;
uint8_t bitR1 = 0;
uint8_t protoc = 0;
uint8_t menu = 0;
uint8_t sellect = 0;
uint8_t lister = 1;
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
  Serial.print("|"); ////////// serial out !!
  Serial.print("0"); ////////// serial out !!
  Serial.print("|"); ////////// serial out !!
  Serial.println("0"); ////////// serial out !!
  sensorValue1 = analogRead(analogInPin1);
  sensorValue2 = analogRead(analogInPin2);
  buttonState = digitalRead(pushButton);
  delay (100);
  if (buttonState == LOW) {
    display.clearDisplay();
    display.drawBitmap(35, 15, Thumb, 64, 64, 1);
    display.display();
    buttonState = HIGH;
    delay(100);
    sellect = 1;
    buttonState = HIGH;
  }
  if (sellect == 1) {
    buttonState = HIGH;
    // delay(00);
    menw();
    return;
  }

  if (sensorValue1 > 600 )teslachargeport();
  // if (sensorValue1 < 500 )select = 3;;
  if (sensorValue2 > 600 )screensaver ();
  if (sensorValue2 < 500 )screensaver ();
#ifdef debug1
  if (sensorValue1 > 600 )Serial.println("UP");
  if (sensorValue1 < 500 )Serial.println("DOWN");
  if (sensorValue2 > 600 )Serial.println("RIGHT");
  if (sensorValue2 < 500 )Serial.println("LEFT");
#endif
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
    delay(2000);
#endif
    display.clearDisplay();
    display.drawBitmap(0, 0, myBitmapC, 128, 64, 1);
    display.display();
    delay(200);
    if (buff == 1)select = 1;
    if (buff == 2)select = 2;
    if (buff == 3)select = 3;
  }
  if (select == 1) {
    rfrx();
    return;
    // teslachargeport();
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
  delay (200);
  digitalWrite(ledPinP, LOW);
}

///////////////////////////////////////////////////////////////////////////////////////////////
void teslachargeport() {
  for (uint8_t t = 0; t < transmissions; t++) {
    display.clearDisplay();
    display.drawBitmap(0, 20, Teslalogo, 128, 36, 1);
    display.display();
    delay (500);
    //    digitalWrite(ledPinX, LOW);
    for (uint8_t i = 0; i < messageLength; i++) sendByte(sequence[i]);
    digitalWrite(signalPin, LOW);
    delay(messageDistance);
    Serial.print("|"); ////////// serial out !!
    Serial.print("1"); ////////// serial out !!
    Serial.print("|"); ////////// serial out !!
    Serial.println("0"); ////////// serial out !!
    select = 0;
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
void menw() {
  //buttonState == HIGH;
  delay(200);
  buttonState = digitalRead(pushButton);
  if (buttonState == LOW) {
    display.clearDisplay();
    display.drawBitmap(35, 15, Thumb, 64, 64, 1);
    display.display();
    buttonState = HIGH;
    delay(200);
    sellect = 0;
    delay(200);
    buttonState = HIGH;
    return;
  }
  if (sensorValue1 > 600 )menu = menu - 1;
  if (sensorValue1 < 500 )menu = menu + 1;
  // if (sensorValue2 > 600 )lister = lister + 1;
  // if (sensorValue2 < 500 )lister = lister - 1;
  if (menu > 3)menu = 1;
  if (menu < 1)menu = 3;
  delay(100);
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
  // delay(500);
  if (menu == 1) {
    display.setTextColor(WHITE);
    display.setCursor(0, 20);
    display.print("*");;
    display.display();
    delay(100);
    display.setTextColor(BLACK);
    display.setCursor(0, 20);
    display.print(" ");
    display.display();
    delay(100);
    select = 1;
  }
  if (menu == 2) {
    display.setTextColor(WHITE);
    display.setCursor(0, 40);
    display.print("*");
    display.display();
    delay(100);
    display.setTextColor(BLACK);
    display.setCursor(0, 40);
    display.print(" ");
    display.display();
    delay(100);
    select = 2;
  }
  if (menu == 3) {
    display.setTextColor(WHITE);
    display.setCursor(0, 60);
    display.print("*");
    display.display();
    delay(100);
    display.setTextColor(BLACK);
    display.setCursor(0, 60);
    display.print(" ");
    display.display();
    delay(100);
    select = 0;
  }
}
////////////////////////////
void screensaver () {
  saverscr = 0;
  display.clearDisplay();
  display.drawBitmap(0, 0, younoob, 128, 64, 1);
  display.display();
  delay (1000);
  Serial.print("|"); ////////// serial out !!
  Serial.print("2"); ////////// serial out !!
  Serial.print("|"); ////////// serial out !!
  Serial.println("0"); ////////// serial out !!
}

void rfrx () {
  if (mySwitch.available()) {
    saverscr = 0;
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.clearDisplay();
    display.drawBitmap(0, 0, rximage, 128, 64, 1);
    display.setCursor(70, 17);
    display.print("433 Mhz");
    display.setCursor(70, 29);
    display.print("  FSK");
    display.setCursor(70, 43);
    display.print("FOUND !");
    display.display();
    delay(3000);
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
    display.println( mySwitch.getReceivedProtocol() );
    display.display();
    store1 = mySwitch.getReceivedValue();
    bitR1 = mySwitch.getReceivedBitlength();
    protoc = mySwitch.getReceivedProtocol();
    delay(3000);
    Serial.print("|"); ////////// serial out !!
    Serial.print("3"); ////////// serial out !!
    Serial.print("|"); ////////// serial out !!
    Serial.println(store1); ////////// serial out !!
    if (value == 0) {
      display.clearDisplay();
      display.setCursor(0, 20);
      // Serial.print("Unknown encoding");
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
  display.print("<< RECEIVED RF CODE ");
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
  delay(500);
  //  display.clearDisplay();
  //   display.setTextColor(WHITE);
  //   display.setCursor(5, 3);
  //   display.print("<< RECEIVED RAW CODE ");
  //   display.setCursor(10, 20);
  //   display.drawLine(0, 14, 128, 14, WHITE);
  //  display.print(rawdat);;
  //   display.display();
  //   delay(500);
}

