#include <StandardCplusplus.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <vector>

using namespace std;

// Функция шифрования сообщения афинным шифром
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

// Строки для передачи
String transmissionMessage1;
String transmissionMessage2;
String transmissionMessage3;
String transmissionMessage4;

void setup() {
  Serial.begin(9600);
  
  // Инициализация радио
  radio.begin();
  radio.setAutoAck(1);
  radio.setRetries(0, 15);
  radio.enableAckPayload();
  radio.setPayloadSize(32);
  radio.openWritingPipe(address[0]);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.powerUp();
  radio.stopListening();


  int a = 3; // множитель
  int b = 5; // сдвиг

  // 1-е сообщение: "Azimuth: 127*"
  String msg1 = "Azimuth: 127*";
  vector<int> encryptedMessage1 = encryptionMessage(msg1, a, b);
  transmissionMessage1 = "129 ";
  for (int val : encryptedMessage1) {
    transmissionMessage1 += String(val) + " ";
  }
  transmissionMessage1 += "130";
  
  // 2-е сообщение: "Battery:95%"
  String msg2 = "Battery: 95% ";
  vector<int> encryptedMessage2 = encryptionMessage(msg2, a, b);
  transmissionMessage2 = "129 ";
  for (int val : encryptedMessage2) {
    transmissionMessage2 += String(val) + " ";
  }
  transmissionMessage2 += "130";
  
  // 3-е сообщение: "Temp:-5CX:0125"
  String msg3 = "Temp: -5C ";
  vector<int> encryptedMessage3 = encryptionMessage(msg3, a, b);
  transmissionMessage3 = "129 ";
  for (int val : encryptedMessage3) {
    transmissionMessage3 += String(val) + " ";
  }
  transmissionMessage3 += "130";
  
  // 4-е сообщение: "X:0125 Y:0458 Z:0791"
  String msg4 = "X:0125 Y:0458 Z:0791 ";
  vector<int> encryptedMessage4 = encryptionMessage(msg4, a, b);
  transmissionMessage4 = "129 ";
  for (int val : encryptedMessage4) {
    transmissionMessage4 += String(val) + " ";
  }
  transmissionMessage4 += "130";
  
  // Вывод сформированных сообщений для отладки
  // Отправка сообщений по заданным каналам
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
  // Отправляем первое сообщение на канал 0x60
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

  Serial.println("Messages sent");
  delay(1000);
  Serial.println("Transmission Message 1: " + transmissionMessage1);
  Serial.println("Transmission Message 2: " + transmissionMessage2);
  Serial.println("Transmission Message 3: " + transmissionMessage3);
  Serial.println("Transmission Message 4: " + transmissionMessage4);
}
