#include "DHT.h"
#define DHTPIN D2
#define DHTTYPE DHT11
#include "CTBot.h"

CTBot myBot;

String ssid = "*****";
String pass = "*****";
String token = "**********";
const int id = 407617129;

int buzzer = D0;
int ledGreen = D6;
int ledRed = D8;

//mq7
float sensor_volt;
float RS_gas;
float ratio;
float sensorValue;
float b = -0.318;
float m = 1.133;
float R0 = 0.91;
double ppm_log;
double ppm;
const int gasD0 = D3;

//dht11
float t = 0;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  pinMode(gasD0, INPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
  dht.begin();

  Serial.println("Starting telegram bot");
  myBot.wifiConnect(ssid, pass);
  myBot.setTelegramToken(token);

  if(myBot.testConnection()) {
    Serial.println("Connected to Telegram");
    myBot.sendMessage(id, "Send /update for current temperature and CO concentration status");
  }
  else {
    Serial.println("Cannot connect to Telegram");
  }
}
    
void loop() {
  Serial.println("Starting the system...");
  digitalWrite(ledGreen, HIGH);
  delay(200);
  digitalWrite(ledGreen, LOW);
  delay(50);
  digitalWrite(ledGreen, HIGH);
  delay(200);
  digitalWrite(ledGreen, LOW);

  //Calculate temperature
  t = dht.readTemperature();
  delay(500);
  // Check if any reads failed and exit early (to try again).
  if(isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print("Temperature = ");
  Serial.print(t);
  Serial.print("C     ");

  //Calculate concentration carbon monoxide
  sensorValue = analogRead(A0);
  sensor_volt = ((float)sensorValue/1024)*5.0;
  RS_gas = (5.0-sensor_volt)/sensor_volt;
  ratio = RS_gas/R0;
  ppm_log = (log10(ratio)-b)/m;  
  ppm = pow(10, ppm_log);
    
    
  if(t>44) {
    Serial.println("TEMPERATURE TOO HIGH");
    digitalWrite(ledRed, HIGH);
    delay(200);
    digitalWrite(ledRed, LOW);
    delay(20);
    digitalWrite(ledRed, HIGH);
    delay(200);
    digitalWrite(ledRed, LOW);
    delay(20);
    digitalWrite(buzzer, HIGH);
    delay(300);
    digitalWrite(buzzer, LOW);
    delay(100);
     
    //Send message to phone
    myBot.sendMessage(id, "DANGER! TEMPERATURE HIGH");
    Serial.println("Message sent");
  }
  
  else {
    Serial.print("CO = ");
    Serial.print(ppm);
    Serial.println("ppm");
    
    if(ppm>4) {
      Serial.println("CARBON MONOXIDE CONCENTRATION TOO HIGH");
      digitalWrite(ledRed, HIGH);
      delay(200);
      digitalWrite(ledRed, LOW);
      delay(20);
      digitalWrite(ledRed, HIGH);
      delay(200);
      digitalWrite(ledRed, LOW);
      delay(20);
      digitalWrite(buzzer, HIGH);
      delay(300);
      digitalWrite(buzzer, LOW);
      delay(100);
      
      //Send message to phone
      myBot.sendMessage(id, "DANGER! PPM HIGH");
      Serial.println("Message sent");
    }
  }
  delay(2000);

  String update = "Temperature: " + String(t) + "C\nCO Concentration = " + String(ppm) + "ppm";
  TBMessage msg;
  if(myBot.getNewMessage(msg)) {
    if(msg.text.equalsIgnoreCase("/update")) {
      myBot.sendMessage(id, update);
    }
  }
  
}
