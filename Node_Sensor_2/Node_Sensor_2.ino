// Import required libraries
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

// Set your access point network credentials
const char* ssid     = "KebunCerdasPNJ"; //KebunCerdasPNJ
const char* password = "123ey8h6"; //123ey8h6

// Set your Static IP address
IPAddress local_IP(192, 168, 1, 113);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

// Create ESP8266WebServer object on port 80
ESP8266WebServer server(80); //Menyatakan Webserver pada port 80

void handleRoot();              // function prototypes for HTTP handlers
void handleDataAirpupuk();              // function prototypes for HTTP handlers
void handleNotFound();

// Pengukuran Air
const int trigPin = D12;
const int echoPin = D11;
long duration;
int distance;
int tinggi = 45;
int rendah = 0;
int tinggi1 = 40;
int rendah1 = 0;

//pengukuran Pupuk
const int trig2Pin = D10;
const int echo2Pin = D9;
long duration2;
int distance2;

String levelair;
String levelpupuk;
String kondisiair;
String kondisipupuk;

void setup(void) {
  // Serial port for debugging purposes
  Serial.begin(9600);
  Serial.println();

  // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }

  // Mengatur WiFi
  Serial.print("Connecting to "); Serial.println(ssid);
  WiFi.begin(ssid, password); // Mencocokan SSID dan Password
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // Tampilkan status Connected
  Serial.println("");
  Serial.print("MAC:"); Serial.println(WiFi.softAPmacAddress());
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  server.on("/", handleRoot);               // Call the 'handleRoot' function when a client requests URI "/"
  server.on("/postData", handleDataAirpupuk);               // Call the 'handleData' function when a client requests URI "/"
  server.on("/restart", handleDataAirpupuk);               // Call the 'handleRestart' function when a client requests URI "/"
  server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

  // Start server
  server.begin();
  Serial.println("Webserver dijalankan");

  pinMode(trigPin, OUTPUT); //Pengukuran Air
  pinMode(echoPin, INPUT);// Pengukuran Air
  pinMode(trig2Pin, OUTPUT); //Pengukuran Air
  pinMode(echo2Pin, INPUT);// Pengukuran Air
}
unsigned long loopcount = 0;

void loop(void) {
  server.handleClient();                    // Listen for HTTP requests from clients

}

void handleRoot() {
  server.send(200, "text/html", String("<center><h1>SEDANG APA?</h1></center>"));   // Send HTTP status 200 (Ok) and send some text to the browser/client
}

void handleDataAirpupuk() {
  
//  loopcount++; 

  // Sensor Ultrasonik Air
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
//    distance = random(0, 30);


  levelair = String(map(distance, tinggi, rendah, 0, 100));
  Serial.print("Jarak Air      : ");
  Serial.println(distance);
  Serial.print("Persentase Air: ");
  Serial.print(levelair);
  Serial.print("%");
  Serial.print(" || ");
  Serial.println(kondisiair);
  Serial.println("");

  if (distance <= 15 )
  {
    kondisiair = "Air Banyak";
  }
  else if (distance > 15 && distance < 30)
  {
    kondisiair = "Air Setengah";
  }
  else if (distance >= 30)
  {
    kondisiair = "Air Sedikit";
  }

  // Sensor Ultrasonik Pupuk
  digitalWrite(trig2Pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trig2Pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig2Pin, LOW);
  duration2 = pulseIn(echo2Pin, HIGH);
  distance2 = duration2 * 0.034 / 2;
//    distance2 = random(0, 30);

  //  distance2 = random(0, 45);
  levelpupuk = String(map(distance2, tinggi1, rendah1, 0, 100));
  Serial.print("Jarak Pupuk: ");
  Serial.println(distance2);
  Serial.print("Persebtase Pupuk: ");
  Serial.print(levelpupuk);
  Serial.print("%");
  Serial.print(" || |");
  Serial.println(kondisipupuk);

  if (distance2 <= 15)
  {
    kondisipupuk = "Pupuk Banyak";
  }
  else if (distance2 > 15 && distance2 < 30)
  {
    kondisipupuk = "Pupuk Setengah";
  }
  else if (distance2 >= 30)
  {
    kondisipupuk = "Pupuk Sedikit";
  }

  //Jadi ke web
  const size_t capacity = JSON_OBJECT_SIZE(4);
  StaticJsonBuffer<1000> jsonBuffer;
//  DynamicJsonBuffer jsonBuffer(capacity);

  JsonObject& root = jsonBuffer.createObject();
  root["levelair"] = String(levelair);
  root["levelpupuk"] = String(levelpupuk);
  root["kondisiair"] = String(kondisiair);
  root["kondisipupuk"] = String(kondisipupuk);
  root["idAirpupuk"] = String("1");
  String output = " ";
  root.printTo(output);
  server.send(200, "text/plain", String(output));   // Send HTTP status 200 (Ok) and send some text to the browser/client
//  Serial.println(loopcount); // entry counter

//  if (loopcount = 30) {
//    ESP.restart();
//  }
//  else{
//    Serial.println(loopcount);
//  }

}

void handleNotFound() {
  server.send(404, "text/plain", "MO NGAPAIN LU!!"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
