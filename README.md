# ArduflipperOne
Arduflipper is an Arduino based Flipper device , nothing else !
i made this because i see many people like the functions of the Flipperzero ,
mostly opening a tesla charge port , and the RF cloning function for simple RF device's .

![IMG_20221225_15362634r5](https://user-images.githubusercontent.com/20719445/210774538-07f84027-da5c-4b93-aead-fa06398cc496.jpg)

so for now its called Arduflipper One because whe have one function at the moment , 
already added the support for BadUSB , and IR blaster , ( NOT ENOUGH SPACE inside the Arduino Nano and Uno )
so for now have fun opening tesla charge ports and clone your RF remote and transmit .
Have Fun , i wil update when i can 

//////////////////////////////////////////
// ArduFlipper one 7-1-2023  21:51 ///
//       MaDe By DeXtErBoT  TX+RX      ///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// wil store 2 keys in eeprom after recieved one or 2 , NOTE !
// third key wil replacing the first key again onley 2 keys max fit in storage //
// wil remember the keys after power off , and reload them again //
// to use the memory storage keys for transmiting ,press left for using storage 1 and right for storage 2 key //
// it also wil store the bit rate type of the key //
// pushing joystick up wil transmit a Tesla charge point key //
// fast push button wil send you to menu ,in menu you do 2 second long press to get in the selected subject , RX or TX or BACK //
// wil make things better , enjoy this Beta version .// 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

for making a Arduflipper one Version 1.01 ,

what do you need !

1 Arduino (nano or a uno ) 

![download (4)](https://user-images.githubusercontent.com/20719445/210767024-758f3b28-886a-4724-bd37-35992bebf89a.jpg)

wil upgrade my project to a other one (Arduino Mega Mini ) for Bad usb and al other functions to fit .

1 oled screen 128x64 i2c SSD1306 ,(the color you prefer )

![download (2)](https://user-images.githubusercontent.com/20719445/210766292-b1a1427d-084e-433c-b7e5-55eb19ebbd40.jpg)

For image resolution isue's 
NOTE ! (for the oled screen change the resolution in the Adafruit_SSD1306.H file )

   #define SSD1306_128_64   
   
//   #define SSD1306_128_32

or just copy paste the Adafruit_SSD1306.H ( included ) over the original one ,and re upload your sketch 


1 433 Mhz Transmitter and Reciever Module . 

![download](https://user-images.githubusercontent.com/20719445/210765297-00b1fe7e-c71d-4d87-af0a-19b12c4b3851.jpg)

1 Arduino joypad ( wil upgrade this in to buttons in the future )

![download (1)](https://user-images.githubusercontent.com/20719445/210766173-3ea37b9a-acb4-4f7b-8bef-a455aa8f3d12.jpg)

(Optional BT HC-05/HC-06 Bluetooth module for the DIY app i also wil make available for Android onley ,because its build using MIT APP inventor )

![download (3)](https://user-images.githubusercontent.com/20719445/210766449-b4e2159d-f46e-492d-b96a-377c3fd109bf.jpg)

And a box to fit it , i made a 3D printable , wil also update this soon and make it available !

![IMG20230103145011](https://user-images.githubusercontent.com/20719445/210771874-05d3b8c1-cf02-4feb-a99e-f5ced7c6b473.jpg)
![IMG20230103145100](https://user-images.githubusercontent.com/20719445/210772371-4cd14c2e-b175-4388-a8e2-bf5cbbd46beb.jpg)
![IMG20230103145006](https://user-images.githubusercontent.com/20719445/210772512-0f5f2d3b-8ad7-4d5a-8af6-e21a3cd9d274.jpg)


If your upload is a succes !
if you push up , it wil transmit a Tesla charge point beacon twice .(you wil see Tesla logo on the screen )
if you presh the sw button on the joypad you wil enter the menu for RX RF remotes .

wil ad eeprom storage for recieved codes , and a seccond menu !


![Arduflipper pin setup](https://user-images.githubusercontent.com/20719445/210862213-3f1c1e45-ea4c-42ae-8809-15828baefeb5.jpg)
