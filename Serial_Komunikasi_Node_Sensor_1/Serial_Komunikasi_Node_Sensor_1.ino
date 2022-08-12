// Import required libraries
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
SoftwareSerial nodemcu(15, 13); //D8 = Rx & D7 = Tx //Untuk ESP8266
//SoftwareSerial nodemcu(D8, D7); //Untuk WemosD1

// Set your access point network credentials
const char* ssid     = "KebunCerdasPNJ"; //KebunCerdasPNJ
const char* password = "123ey8h6"; //123ey8h6

// Set your Static IP address
IPAddress local_IP(192, 168, 1, 112);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

// Create ESP8266WebServer object on port 80
ESP8266WebServer server(80); //Menyatakan Webserver pada port 80

void handleRoot();              // function prototypes for HTTP handlers
void handleDataPohon();              // function prototypes for HTTP handlers
void handleNotFound();

String kondisi;
String kondisi2;

//const int dry = 705;   //Nilai analog sensor di udara 987
//const int wet = 78;

void setup(void) {
  // Serial port for debugging purposes
  Serial.begin(9600);
  nodemcu.begin(9600);
  while (!Serial) continue;

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
  Serial.println("WiFi connected");
  Serial.print("IP: "); Serial.println(WiFi.localIP());
  Serial.print("MAC:"); Serial.println(WiFi.softAPmacAddress());

  server.on("/", handleRoot);               // Call the 'handleRoot' function when a client requests URI "/"
  server.on("/postData", handleDataPohon);               // Call the 'handleRoot' function when a client requests URI "/"
  server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

  // Start server
  server.begin();
  Serial.println("Webserver dijalankan");
}

void loop(void) {
  server.handleClient();                    // Listen for HTTP requests from clients
}

void handleRoot() {
  server.send(200, "text/html", String("<center><h1>SEDANG APA?</h1></center>"));   // Send HTTP status 200 (Ok) and send some text to the browser/client
}

void handleDataPohon() {
  //  int kelembaban = random(350, 550);
  //  float ph2 = random(4, 6);

  StaticJsonBuffer<5000> jsonBuffer;
  JsonObject& data = jsonBuffer.parseObject(nodemcu);

  Serial.println("JSON Object Recieved");
  //Sensor Kelembaban Tanah
  int kelembaban = data["analog1"];
//  int kelembaban = map(kelembaban1, wet, dry, 10, 1);

  if (kelembaban > 70 ) {
    kondisi = ("Tanah Basah");
  }
  else if (kelembaban >30 && kelembaban <=70) {
    kondisi = ("Tanah Lembab");
  }
  else if (kelembaban <=30 ) {
    kondisi = ("Tanah Kering");
  }
  //Sensor PH Tanah
  float ph2 = data["analog2"];
  float ph = data["analog3"];
  if (ph2 >=8 ) {
    kondisi2 = ("PH Basa");
  }
  else if (ph2 >= 6 && ph2 <8) {
    kondisi2 = ("PH Normal");
  }
  else if (ph2 < 6) {
    kondisi2 = ("PH Asam");
  }

  JsonObject& root = jsonBuffer.createObject();
  root["kelembapan"] = kelembaban;
  root["ph"] = ph2;
  root["kondisi"] = kondisi;
  root["kondisi2"] = kondisi2;
  root["idPohon1"] = "1";
  String output = " ";
  root.printTo(output);
  jsonBuffer.clear();
  server.send(200, "text/plain", String(output));   // Send HTTP status 200 (Ok) and send some text to the browser/client
  Serial.print("ADC: ");
//  Serial.println(kelembaban1);
  Serial.print("Kelembaban Tanah: ");
  Serial.println(kelembaban);
  Serial.print("ph Lama: ");
  Serial.println(ph2);
  Serial.print("ph baru: ");
  Serial.println(ph);

  delay(1000);
  ESP.restart();
}

void handleNotFound() {
  server.send(404, "text/plain", "MO NGAPAIN LU!!"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
