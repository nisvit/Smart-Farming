#include <SoftwareSerial.h>
#include <ArduinoJson.h>

//Initialise Arduino to NodeMCU (2=Rx & 3=Tx)
//SoftwareSerial nodemcu(2, 3); // Menggunakan pin 2 dan pin 3 pada arduino
SoftwareSerial nodemcu(2, 3);

//Kelembaban Tanah
const int dry = 688;   //Nilai analog sensor di udara 987
const int wet = 78;
int sensorValue11;
int sensorValue ;
int sensorPin1 = A0;    
int sensorValue1; 

//Ph Tanah
int sensorPin2 = A1; 
int analogph;
float sensorValue2 = 0;  

void setup()
{
  Serial.begin(9600);
  nodemcu.begin(9600);
//  delay(1000);
  Serial.println("Program started");
//  pinMode(7,OUTPUT);
//  Serial.begin(9600);
 }

void loop() {

  const size_t capacity = JSON_OBJECT_SIZE(5);
  DynamicJsonBuffer jsonBuffer(capacity);

//  StaticJsonBuffer<1000> jsonBuffer; //Mengambil data
  JsonObject& data = jsonBuffer.createObject(); //Memisahkan data
  
  analog_func();

  //Data dari analog_func diberi nama
  data["analog1"] = sensorValue1;
  data["analog2"] = sensorValue2;
//  data["analog3"] = sensor_ph; 

  //Send data to NodeMCU
  data.printTo(nodemcu);
  jsonBuffer.clear();
}

//Pembacaan hasil sensor yang digunakan
void analog_func() {
  //Kelembaban Tanah
  
  sensorValue = analogRead(sensorPin1); 
  sensorValue11 = map(sensorValue, dry,wet, 10, 100);
  sensorValue1 = sensorValue11 + 1,03;
  sensorValue1 = constrain(sensorValue1, 10, 100);

  delay(5000);
//  Serial.print("Nilai ADC :");
//  Serial.println(sensorValue);
//  Serial.print("Sensor 1 :");
//  Serial.println(sensorValue1);
//  
 
  //Ph Tanah
  analogph = analogRead(sensorPin2 );
  sensorValue2 = ((-0.05)*analogph)+ (14.1);
  
//  sensorValue2 = 5;
//  digitalWrite(7,HIGH);
//  delay(5000);
  Serial.print("Nilai ADC PH = ");
  Serial.println(analogph);
//  Serial.println("on");
  Serial.print("Sensor 2 :");
  Serial.println(sensorValue2);
//  digitalWrite(7,LOW);
//  Serial.println("off");
  delay(10000);
  
}
