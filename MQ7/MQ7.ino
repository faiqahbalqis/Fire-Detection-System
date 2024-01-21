const int LED = 13;
const int D0 = 5;

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(D0, INPUT);
}

void loop() {
  int alarm = 0;
  float sensor_volt;
  float RS_gas;
  float ratio;
  float R0 = 0.91;

  int sensorValue = analogRead(A0);
  sensor_volt = ((float)sensorValue/1024)*5.0;
  RS_gas = (5.0-sensor_volt)/sensor_volt;

  ratio = RS_gas/R0;

  float b = -0.318;
  float m = 1.133;
  double ppm_log = (log10(ratio)-b)/m; //Get ppm value in linear scale according to the the ratio value  
  double ppm = pow(10, ppm_log); //Convert ppm value to log scale 
  double percentage = ppm/10000; //Convert to percentage

  Serial.print("sensor_volt = ");
  Serial.println(sensor_volt);
  Serial.print("RS_ratio = ");
  Serial.println(RS_gas);
  Serial.print("Rs/R0 = ");
  Serial.println(ratio);
  Serial.print(ppm);
  Serial.println("ppm");
  Serial.print(percentage); //Load screen buffer with percentage value 
  Serial.println("%"); //Load screen buffer with "%"
  Serial.print("\n");

  alarm = digitalRead(D0);
  if(alarm==1) digitalWrite(LED, HIGH);
  else if (alarm==0) digitalWrite(LED, LOW);

  delay(1000);
}
