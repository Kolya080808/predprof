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
Servo servo; 
 
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
 
 
 
RF24 radio(9, 10); 
byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; 
void setup() { 
 Serial.begin(9600); 
 radio.begin(); 
 radio.setAutoAck(1); 
 radio.setRetries(0, 15); 
 radio.enableAckPayload(); 
 radio.setPayloadSize(32); 
 radio.openReadingPipe(1, address[0]); 
 radio.setChannel(0x60); 
 radio.setPALevel(RF24_PA_MAX); 
 radio.setDataRate(RF24_250KBPS); 
 radio.powerUp(); 
 radio.startListening(); 
 Serial.println("Радио готово к приему сообщений..."); 
 lcd.init(); 
 servo.attach(2); 
 servo.write(127); 
} 
 
 
String encryptedMessage; 
int a = 3; 
int b = 5; 
 
 
void loop() { 


 String decryptedAzimyth; 
 radio.setChannel(0x60); 
 lcd.clear(); 
 lcd.setCursor(0,0); 
 float temp = analogRead(A0); 
 lcd.print("temp "); 
 lcd.print(temp); 
 lcd.print("C"); 
 if(radio.getChannel() == 0x60) { 
  char receivedData[32]; 
  byte pipeNo; 
  while (radio.available(&pipeNo)) { 
   radio.read(&receivedData, sizeof(receivedData)); 
  } 
  for (byte i = 0; i < sizeof(receivedData); i++) { 
   encryptedMessage += receivedData[i]; 
  } 
  Serial.println(encryptedMessage); 
  memset(receivedData, 0, sizeof(receivedData));
 
 
  vector<int> encryptedForDecryption; 
  stringstream ss(encryptedMessage.c_str()); 
  int num; 
  while (ss >> num) { 
   encryptedForDecryption.push_back(num); 
  } 
 
  decryptedAzimyth = decryptionMessage(encryptedForDecryption, a, b); 
 
  Serial.println(decryptedAzimyth); 
 
 
  radio.setChannel(0x61); 
 
 } 
 
 
 if(radio.getChannel() == 0x61) { 
  char receivedData[32]; 
  byte pipeNo; 
  while (radio.available(&pipeNo)) { 
   radio.read(&receivedData, sizeof(receivedData)); 
  } 
  for (byte i = 0; i < sizeof(receivedData); i++) { 
   encryptedMessage += receivedData[i]; 
  } 
  Serial.println(encryptedMessage); 
  memset(receivedData, 0, sizeof(receivedData));
 
 
  vector<int> encryptedForDecryption; 
  stringstream ss(encryptedMessage.c_str()); 
  int num; 
  while (ss >> num) { 
   encryptedForDecryption.push_back(num); 
  } 
 
  String decryptedAzimyth = decryptionMessage(encryptedForDecryption, a, b); 
 
  Serial.println(decryptedAzimyth); 
 
 
 
 
 
  while (radio.available(&pipeNo)) { 
   radio.read(&receivedData, sizeof(receivedData)); 
  } 
  for (byte i = 0; i < sizeof(receivedData); i++) { 
   encryptedMessage += receivedData[i]; 
  } 
  Serial.println(encryptedMessage); 
  memset(receivedData, 0, sizeof(receivedData));
 
 
  while (ss >> num) { 
   encryptedForDecryption.push_back(num); 
  } 
 
  decryptedAzimyth = decryptionMessage(encryptedForDecryption, a, b); 
 
  Serial.println(decryptedAzimyth); 
 
 
 
 
 
 
  while (radio.available(&pipeNo)) { 
   radio.read(&receivedData, sizeof(receivedData)); 
  } 
  for (byte i = 0; i < sizeof(receivedData); i++) { 
   encryptedMessage += receivedData[i]; 
  } 
  Serial.println(encryptedMessage); 
  memset(receivedData, 0, sizeof(receivedData));
 
 
  while (ss >> num) { 
   encryptedForDecryption.push_back(num); 
  } 
 
  decryptedAzimyth = decryptionMessage(encryptedForDecryption, a, b);
  Serial.println(decryptedAzimyth);
  radio.setChannel(0x60);
 }
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Azimyth ");
 lcd.print(decryptedAzimyth);
 lcd.print(" ");
 lcd.print(decryptedAzimyth);
 lcd.setCursor(0,1);
 lcd.print(decryptedAzimyth);
 lcd.print(" ");
 lcd.print(decryptedAzimyth);
 delay(10000);


}
