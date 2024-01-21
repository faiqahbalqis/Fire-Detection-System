#include "CTBot.h"
CTBot myBot;

String ssid = "yesaya";
String pass = "faiqahbalqis";
String token = "5472570344:AAGVGt1t5mV3-litGeZ46X4n_3uqaB2l9y4";
const int id = 407617129;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting telegram bot");
  myBot.wifiConnect(ssid, pass);
  myBot.setTelegramToken(token);

  if(myBot.testConnection()) {
    Serial.println("Connected");
  }
  else {
    Serial.println("Cannot connect");
  }

  myBot.sendMessage(id, "DANGER! Fire in the house");
  Serial.println("Message sent");
}

void loop() {

}
