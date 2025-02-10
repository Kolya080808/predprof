#include <StandardCplusplus.h> 
#include <SPI.h> 
#include "nRF24L01.h" 
#include "RF24.h" 
#include <Wire.h> 
#include <Servo.h> 
#include <iostream> 
#include <vector> 
#include <string> 
#include <sstream> 
#include <LiquidCrystal_I2C.h> 
LiquidCrystal_I2C lcd(0x27, 20, 2); 
long timer;
long wait;
long wait2;
long timer2;
Servo servo; 
String messange1="";
String messange2="";
String fullmessange = "";
using namespace std;
String decryptionMessage(const vector<int> &encrypted, int a, int b) { 
 String decrypted; 
 int a_inv = 0; 
 
 for (int i = 1; i < 128; ++i) { 
  if ((a * i) % 128 == 1) { 
   a_inv = i; 
   break; 
  } 
 } 
 for (size_t i = 1; i < encrypted.size() - 1; ++i) { 
  int val = encrypted[i]; 
  int decryptedVal = (a_inv * (val - b + 128)) % 128; 
  decrypted += static_cast<char>(decryptedVal); 
 } 
 
 return decrypted; 
} 
String encryptedMessage; 
int a = 3; 
int b = 5; 
RF24 radio(9, 10);
byte address[][6] =  {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"};
void setup() {
 Serial.begin(9600); 
 radio.begin(); 
 radio.setAutoAck(1); 
 radio.setRetries(0, 15); 
 radio.enableAckPayload(); 
 radio.setPayloadSize(64); 
 radio.openReadingPipe(1, address[0]); 
 radio.setChannel(0x60); 
 radio.setPALevel(RF24_PA_MAX); 
 radio.setDataRate(RF24_250KBPS); 
 radio.powerUp(); 
 radio.startListening(); 
 Serial.println("Радио готово к приему сообщений...");
 servo.attach(2);
 servo.write(0);
 lcd.init();
 lcd.backlight();
 timer=millis();
 wait=millis();
 timer2 = millis();
 wait2 = millis();
}
void loop() {
  byte pipino;
  byte read;
  int print= 1;
  lcd.clear();
  lcd.setCursor(0,0); 
  // radio.read(&read,1);
  // if (read = 1){
  //   radio.openWritingPipe( address[0]);
  //   radio.stopListening();
  //   radio.write(&print,1);
  //   radio.openReadingPipe(1, address[0]);
  //   radio.startListening();
  // }
  String encryptedMessage;
  char messange[100];
  while(radio.available(&pipino)){
    radio.read(&messange,sizeof(messange));
    for(byte i = 0;i<sizeof(messange);i++){
      encryptedMessage += messange[i];
    }
    vector<int> encryptedForDecryption; 
    stringstream ss(encryptedMessage.c_str()); 
    int num; 
    while (ss >> num) { 
    encryptedForDecryption.push_back(num);  decryptionMessage(encryptedForDecryption, a, b); 
    }
    if((timer-wait)<=1000 and decryptionMessage(encryptedForDecryption, a, b) != messange2) {
       messange1 = decryptionMessage(encryptedForDecryption, a, b);// Управляем сервоприводом

    }
    if ((timer-wait)>=1000 and decryptionMessage(encryptedForDecryption, a, b) !=messange1) {
      messange2 = decryptionMessage(encryptedForDecryption, a, b);
      char seekangel = '*';
      if (messange2.indexOf(seekangel) != -1) { // Проверяем, что символ найден
        String sangel = messange2.substring(0,messange2.indexOf(seekangel));
        int angel = sangel.toInt(); // Преобразуем в число
        servo.write(angel);
        Serial.println(angel);
      } 
      timer = millis();
      wait = millis();
      fullmessange =  messange1 + messange2;
      messange1="";
      messange2="";
      }
    lcd.print(fullmessange);
    memset(messange,0,sizeof(messange));
    delay(1000);
    wait = (wait-1000);
    wait2 = (wait2-1000);
  }
  else{
    float temp = analogRead(A0);
    lcd.print("temp ");
    lcd.print(temp);
    lcd.print("C");
    delay(1000);
  }
  

  if((timer2-wait2)>=10000){
    lcd.clear();
    float temp = analogRead(A0);
    lcd.print("temp ");
    lcd.print(temp);
    lcd.print("C");
    delay(8000);
    lcd.clear();
    lcd.setCursor(0,0);
    timer = millis();
    wait = millis();
    timer2 = millis();
    wait2 = millis();
    
  }
}
