#include <StandardCplusplus.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <vector>

using namespace std;


vector<int> encryptionMessage(const String &Msg, int a, int b)
{
    vector<int> encrypted;
    for (char ch : Msg)
    {
        int asciiVal = static_cast<int>(ch); // Шифруем ascii
        int encryptedVal = ((a * asciiVal + b) % 128 + 128) % 128; // Сама шифровка
        encrypted.push_back(encryptedVal);
    }
    return encrypted;
}


RF24 radio(9, 10);
byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"};


String transmissionMessage1;
String transmissionMessage2;
String transmissionMessage3;
String transmissionMessage4;
String transmissionMessage5;
String transmissionMessage6;
String transmissionMessage7;
String transmissionMessage8;

void setup() {
  Serial.begin(9600);
  

  radio.begin();
  radio.setAutoAck(1);
  radio.setRetries(0, 15);
  radio.enableAckPayload();
  radio.setPayloadSize(64);
  radio.openWritingPipe(address[0]);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.powerUp();
  radio.stopListening();


  int a = 3; // множитель
  int b = 5; // сдвиг


  String msg1 = "Azimuth:";
  vector<int> encryptedMessage1 = encryptionMessage(msg1, a, b);
  transmissionMessage1 = "129 ";
  for (int val : encryptedMessage1) {
    transmissionMessage1 += String(val) + " ";
  }
  transmissionMessage1 += "130";

  String msg2 = " 127*";
  vector<int> encryptedMessage2 = encryptionMessage(msg2, a, b);
  transmissionMessage2 = "129 ";
  for (int val : encryptedMessage2) {
    transmissionMessage2 += String(val) + " ";
  }
  transmissionMessage2 += "130";

  

  String msg3 = "Battery:";
  vector<int> encryptedMessage3 = encryptionMessage(msg3, a, b);
  transmissionMessage3 = "129 ";
  for (int val : encryptedMessage3) {
    transmissionMessage3 += String(val) + " ";
  }
  transmissionMessage3 += "130";

  String msg4 = " 95%";
  vector<int> encryptedMessage4 = encryptionMessage(msg4, a, b);
  transmissionMessage4 = "129 ";
  for (int val : encryptedMessage4) {
    transmissionMessage4 += String(val) + " ";
  }
  transmissionMessage4 += "130";
  

  String msg5 = "Temp: ";
  vector<int> encryptedMessage5 = encryptionMessage(msg5, a, b);
  transmissionMessage5 = "129 ";
  for (int val : encryptedMessage5) {
    transmissionMessage5 += String(val) + " ";
  }
  transmissionMessage5 += "130";

    String msg6 = "-5C ";
  vector<int> encryptedMessage6 = encryptionMessage(msg6, a, b);
  transmissionMessage6 = "129 ";
  for (int val : encryptedMessage6) {
    transmissionMessage6 += String(val) + " ";
  }
  transmissionMessage6 += "130";
  

  String msg7 = "Status:";
  vector<int> encryptedMessage7 = encryptionMessage(msg7, a, b);
  transmissionMessage7 = "129 ";
  for (int val : encryptedMessage7) {
    transmissionMessage7 += String(val) + " ";
  }
  transmissionMessage7 += "130";

  String msg8 = "Working";
  vector<int> encryptedMessage8 = encryptionMessage(msg8, a, b);
  transmissionMessage8 = "129 ";
  for (int val : encryptedMessage8) {
    transmissionMessage8 += String(val) + " ";
  }
  transmissionMessage8 += "130";
    
  Serial.println("Стартуем МКА");
  delay(1000);
  Serial.println("Стартуем МКА.");
  delay(1200);
  Serial.println("Стартуем МКА..");
  delay(700);
  Serial.println("Стартуем МКА...");
  delay(250); 
  Serial.println("Готов к работе.");
  delay(2000);
  radio.setChannel(0x60);
}
void loop() {
    radio.write(transmissionMessage1.c_str(), transmissionMessage1.length() + 1);
    delay(1000);
    radio.write(transmissionMessage2.c_str(), transmissionMessage2.length() + 1);
    delay(1000);
    radio.write(transmissionMessage3.c_str(), transmissionMessage3.length() + 1);
    delay(1000);
    radio.write(transmissionMessage4.c_str(), transmissionMessage4.length() + 1);
    delay(1000);
    radio.write(transmissionMessage5.c_str(), transmissionMessage5.length() + 1);
    delay(1000);
    radio.write(transmissionMessage6.c_str(), transmissionMessage6.length() + 1);
    delay(1000);
    radio.write(transmissionMessage7.c_str(), transmissionMessage7.length() + 1);
    delay(1000);
    radio.write(transmissionMessage8.c_str(), transmissionMessage8.length() + 1);
  


  Serial.println("Messages sent");
  delay(1000);
  Serial.println("Transmission Message 4: " + transmissionMessage6 + transmissionMessage7 );
  }
