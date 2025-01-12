#define BLYNK_TEMPLATE_ID "TMPL290nBqKgi"
#define BLYNK_TEMPLATE_NAME "Smart Green House"
#define BLYNK_AUTH_TOKEN "TDCsWapbuW3Y6pCTMBRd0-fOexFpOptH"

#include<BlynkSimpleEsp8266.h>
#include "DHT.h"
#include <ESP8266WiFi.h>
#include<LiquidCrystal_I2C.h>
#include<Wire.h>


LiquidCrystal_I2C lcd(0x27 , 20, 4) ;
#define DHT11_PIN 14

#define TRIGGERPIN 13//D7
#define ECHOPIN 15//D8



char ssid[]= "ULK HOTSPOT";
char password[] = "ulkhotspot@2015";
/*char ssid[]= "ULK-SOCOBA";
char password[] = "socoba#2023";*/
char auth[]= BLYNK_AUTH_TOKEN;

DHT dht11(DHT11_PIN, DHT11);
int moistureSensorPin = A0;
int moistureSensorValue; 
long duration, distance;
int lamp= 0 ; // D3
int fan= 2 ; // D4
int motor= 12 ; // D6
double humi;
double tempC;

BlynkTimer timer;

void myTimer() {
  Blynk.virtualWrite(V0,humi);
  Blynk.virtualWrite(V1,tempC);
  Blynk.virtualWrite(V2,distance);
 
}

BLYNK_WRITE(V3){
  int value=param.asInt();
  if (value==1){
    digitalWrite(fan,HIGH);
    
    lcd.setCursor(11,0);
    lcd.clear();
    lcd.setCursor(11,0);
    lcd.print("FAN ON");
  }
  else{
    digitalWrite(fan,LOW);
    lcd.setCursor(11,0);
    lcd.print("FAN OFF");
  }
}

BLYNK_WRITE(V4){
  int value=param.asInt();
  if (value==1){
    digitalWrite(motor,HIGH);
    lcd.setCursor(0,0);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("MOTOR ON");
    
  }
  else{
    digitalWrite(motor,LOW);
    lcd.setCursor(0,0);
    lcd.print("MOTOR OFF");
  }
}
BLYNK_CONNECTED(){
  Blynk.syncVirtual(V3);
  Blynk.syncVirtual(V4);
}

void setup() {
  Serial.begin(115200);


  WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
    delay(3000);
    }
    Serial.println("Connected");
    delay(3000);
  dht11.begin(); // initialize the sensor
  pinMode(moistureSensorPin,INPUT);
  pinMode(TRIGGERPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);
  pinMode(lamp,OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(motor, OUTPUT);

  Blynk.begin(auth,ssid,password);
  timer.setInterval(1000L,myTimer);

  Wire.begin(); //BROCHES ESP8266
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Smart Green House");
  lcd.setCursor(5,1);
  lcd.print("Based on IoT");
  lcd.setCursor(0,2);
  lcd.print("Made by @Arn & @Arm");
  lcd.setCursor(5,3);
  lcd.print("08-MAY-2024");
}

void loop() {
  
  // read humidity
  humi  = dht11.readHumidity();
  // read temperature as Celsius
  tempC = dht11.readTemperature();
  // read temperature as Fahrenheit
  float tempF = dht11.readTemperature(true);

  // check if any reads failed
  if (isnan(humi) || isnan(tempC) || isnan(tempF)) {
    Serial.println("Failed to read from DHT11 sensor!");
  } else {
    Serial.print("DHT11# Humidity: ");
    Serial.print(humi);
    Serial.print("%");

    Serial.print("  |  "); 

    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print("°C ~ ");
    Serial.print(tempF);
    Serial.println("°F");

    lcd.setCursor(0,1);
    lcd.print("Temp:");

    lcd.setCursor(7,1);
    lcd.print(tempC);
    lcd.setCursor(13,1);
    lcd.print("C");

    lcd.setCursor(0,2);
    lcd.print("Humidity:");

    lcd.setCursor(11,2);
    lcd.print(humi);
    lcd.setCursor(16,2);
    lcd.print("%");

    lcd.setCursor(0,3);
    lcd.print("VidePresent:");
    lcd.setCursor(14,3);
    lcd.print(distance);
    lcd.setCursor(17,3);
    lcd.print("cm");
    

  }
  moistureSensorValue = analogRead(moistureSensorPin);
  Serial.print("moistureSensorValue: ");
  Serial.println (moistureSensorValue);

  if(moistureSensorValue>700){
    digitalWrite(lamp,HIGH);
  }
  else{
    digitalWrite(lamp,LOW);
  }
  
  digitalWrite(TRIGGERPIN, LOW);  
  delayMicroseconds(3); 
  
  digitalWrite(TRIGGERPIN, HIGH);
  delayMicroseconds(10); 
  
  digitalWrite(TRIGGERPIN, LOW);
  duration = pulseIn(ECHOPIN, HIGH);
  distance = (duration/2) / 29.1;
  Serial.print ("Distance from obsacle is: ");
  Serial.print(distance);
  Serial.println("Cm");



  Blynk.run();
  timer.run();
}