#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <Fuzzy.h>

//D6 = Rx & D5 = Tx
SoftwareSerial nodemcu(D5, D6);


String statusAir;
String statusPupuk;
 

//Inisialisasi Pompa
int pompa1 = D7;
int pompa2 = D8;

//Buzzer
const int toneAir = 3; //buzzer to arduino pin 4
const int tonePupuk = 4; //buzzer to arduino pin 4
    
// Inisialisasi Fuzzy Objek
Fuzzy *fuzzy = new Fuzzy();
  
//Fuzzy Set
//Fuzzy Set Kelembaban Tanah
FuzzySet *kering       = new FuzzySet(0, 0, 29, 30);
FuzzySet *lembab       = new FuzzySet(29, 30, 40, 60);
FuzzySet *basah        = new FuzzySet(50, 70, 80, 80);
  
//Fuzzy set PH Tanah
FuzzySet *asam         = new FuzzySet(3, 4, 5, 6);
FuzzySet *netral       = new FuzzySet(5, 6, 7, 8);
FuzzySet *basa         = new FuzzySet(7, 7, 8, 8);
  
  
//Fuzzy Set pompa air
FuzzySet* mati        = new FuzzySet(0, 2, 3, 5);
FuzzySet* hidup       = new FuzzySet(6, 8, 9, 10);
  
//Fuzzy Set pompa pupuk
FuzzySet* mati1       = new FuzzySet(0, 2, 3, 5);
FuzzySet* hidup1      = new FuzzySet(6, 8, 9, 10);
  
  
void setup() {
  // Initialize Serial port
  Serial.begin(9600);
  nodemcu.begin(9600);
  delay(1000);
  
  //Inisialisasi pin pompa
  pinMode(pompa1, OUTPUT);
  pinMode(pompa2, OUTPUT);

  //Fuzzy Input
  //Fuzzy Input Kelembaban Tanah 
  FuzzyInput* kelembaban = new FuzzyInput(1);

  kelembaban->addFuzzySet(kering);
  kelembaban->addFuzzySet(lembab);
  kelembaban->addFuzzySet(basah);
  
  fuzzy->addFuzzyInput(kelembaban);

  //Fuzzy Input ph
  FuzzyInput* ph = new FuzzyInput(2);

  ph->addFuzzySet(asam);
  ph->addFuzzySet(netral);
  ph->addFuzzySet(basa);

  fuzzy->addFuzzyInput(ph);
  

  //Fuzzy Output Penyiraman Air
  FuzzyOutput* pompa1 = new FuzzyOutput(1);

  pompa1->addFuzzySet(mati); 
  pompa1->addFuzzySet(hidup); 

  fuzzy->addFuzzyOutput(pompa1);

  //Fuzzy Output PH
  FuzzyOutput* pompa2 = new FuzzyOutput(2);

  pompa2->addFuzzySet(mati1); 
  pompa2->addFuzzySet(hidup1); 

  fuzzy->addFuzzyOutput(pompa2);


  //Fuzzy Rule
  //Penyiraman Air
   FuzzyRuleConsequent* thenmati = new FuzzyRuleConsequent();
   thenmati->addOutput(mati);
   FuzzyRuleConsequent* thenhidup = new FuzzyRuleConsequent(); 
   thenhidup->addOutput(hidup);  

   //Penyiraman Pupuk
   FuzzyRuleConsequent* thenmati1 = new FuzzyRuleConsequent();
   thenmati1->addOutput(mati1);
   FuzzyRuleConsequent* thenhidup1 = new FuzzyRuleConsequent(); 
   thenhidup1->addOutput(hidup1);  
    
   //Fuzzy Rule1
   FuzzyRuleAntecedent *ifkelembabanbasah = new FuzzyRuleAntecedent();
  ifkelembabanbasah->joinSingle(basah);

   FuzzyRule* fuzzyRule01 = new FuzzyRule(1, ifkelembabanbasah, thenmati);
   fuzzy->addFuzzyRule(fuzzyRule01); 

   //Fuzzy Rule2
   FuzzyRuleAntecedent *ifkelembabanlembab = new FuzzyRuleAntecedent();
  ifkelembabanlembab->joinSingle(lembab);

   FuzzyRule* fuzzyRule02 = new FuzzyRule(2, ifkelembabanlembab, thenmati);
   fuzzy->addFuzzyRule(fuzzyRule02);

   //Fuzzy Rule3
   FuzzyRuleAntecedent* ifkelembabankering = new FuzzyRuleAntecedent();
   ifkelembabankering->joinSingle(kering); 

   FuzzyRule* fuzzyRule03 = new FuzzyRule(3, ifkelembabankering, thenhidup);
   fuzzy->addFuzzyRule(fuzzyRule03);

   //Fuzzy Rule4
   FuzzyRuleAntecedent* ifphasam = new FuzzyRuleAntecedent();
   ifphasam->joinSingle(asam); 

   FuzzyRule* fuzzyRule04 = new FuzzyRule(4, ifphasam, thenhidup1);
   fuzzy->addFuzzyRule(fuzzyRule04);

   //Fuzzy Rule5
   FuzzyRuleAntecedent* ifphnetral = new FuzzyRuleAntecedent();
   ifphnetral->joinSingle(netral); 

   FuzzyRule* fuzzyRule05 = new FuzzyRule(5, ifphnetral, thenmati1);
   fuzzy->addFuzzyRule(fuzzyRule05);

   //Fuzzy Rule6
   FuzzyRuleAntecedent* ifphbasa = new FuzzyRuleAntecedent();
   ifphbasa->joinSingle(basa); 

   FuzzyRule* fuzzyRule06 = new FuzzyRule(6, ifphbasa, thenhidup1);
   fuzzy->addFuzzyRule(fuzzyRule06);
   
}

void loop() {

  StaticJsonBuffer<1000> jB;
  JsonObject& proses = jB.parseObject(nodemcu);

  if (proses == JsonObject::invalid()) {
    //Serial.println("Invalid Json Object");
    jB.clear();
    return;
  }

  int kelembaban = proses["pohon"];
  float ph = proses["ph"];
  int levelair = proses["air"];
  int levelpupuk = proses["pupuk"];

//  int kelembaban = 6;
//  float ph = 5;
//  int levelair = 15;
//  int levelpupuk = 31;
  

  // fuzzyfication
//  fuzzy->setInput(1, kelembaban);
//  fuzzy->setInput(2, ph);
//  fuzzy->fuzzify();

  // defuzzyfication
//  int pompa_out1 = fuzzy->defuzzify(1);
//  if (pompa_out1 <=5 ) {
//    digitalWrite(pompa1, HIGH);
//    statusAir = "OFF";
//    
//  }
//  else if (pompa_out1 >= 6 ) {
//    digitalWrite(pompa1, LOW);
//    statusAir = "ON";
//  }

  
//  int pompa_out2 = fuzzy->defuzzify(2);
//  if (pompa_out2 <=5 ) {
//    digitalWrite(pompa2, HIGH);
//    statusPupuk = "OFF";
//  }
//  else if (pompa_out2 >=6 ) {
//    digitalWrite(pompa2, LOW);
//    statusPupuk = "ON";
//  }

  //Kelembaban Tanah
  if (kelembaban <=30) {
    digitalWrite(pompa1, LOW);
    statusAir = "ON";
  }
  else if (kelembaban >30 && kelembaban <=100){
    digitalWrite(pompa1, HIGH);
    statusAir = "OFF";
  }
  

  if (ph < 6) {
    digitalWrite(pompa2, LOW);
    statusPupuk = "ON";
    delay(20000);
    digitalWrite(pompa2, HIGH);
    {
    digitalWrite(pompa2, HIGH);
    delay(500000);
    statusPupuk = "OFF";
    }
}
  else if (ph >=6 && ph <=7) {
      digitalWrite(pompa2, HIGH);
      statusPupuk = "OFF";
    }
  else if (ph >=8) {
    digitalWrite(pompa2, LOW);
    statusPupuk = "OFF";
  }
 
  
//  //PH Tanah
//  if (levelpupuk <= 70 && ph < 6 ) {
//   digitalWrite(pompa2, HIGH);
//   statusPupuk = "OFF";
//   }
//  else if (levelpupuk > 70 && ph < 6){
//   digitalWrite(pompa2, LOW);
//   delay(20000);
//   statusPupuk = "ON";
//    }
//    
//   if (ph >=6 && ph <8){
//    digitalWrite(pompa2, HIGH);
//    statusPupuk = "OFF";
//   }
//   else if (ph >=8 && levelpupuk > 70){
//    digitalWrite(pompa2,LOW);
//    statusPupuk = "ON";
//   }
//   else if (ph >=8 && levelpupuk <= 70){
//    digitalWrite(pompa2, HIGH);
//    statusPupuk = "OFF";
//   }

    
  Serial.print("Kelembaban Tanah: ");
  Serial.println(kelembaban);
  Serial.print("PH Tanah   : ");
  Serial.println(ph);
  Serial.print("Fuzzi 1: ");
//  Serial.println(pompa_out1);
  Serial.print("Fuzzi 2: ");
//  Serial.println(pompa_out2);

//  if (levelair <= 30 && levelair >= 1){
//  tone(toneAir, 4000); // Send 1KHz sound signal...
//  delay(100);
//  tone(toneAir, 4000); // Send 1KHz sound signal...
//  delay(100);
//  tone(toneAir, 4000); // Send 1KHz sound signal...
//  delay(2000);
//  noTone(toneAir);     // Stop sound...
//  }
//  else{
//  noTone(toneAir);     // Stop sound...
//  }
//  
//  if (levelpupuk <= 30 && levelpupuk >= 1){
//  tone(tonePupuk, 2000); // Send 1KHz sound signal...
//  delay(100);
//  tone(tonePupuk, 2000); // Send 1KHz sound signal...
//  delay(100);
//  tone(tonePupuk, 2000); // Send 1KHz sound signal...
//  delay(2000);        // ...for 1 sec
//  noTone(tonePupuk);     // Stop sound...
//  }
//  else{
//  noTone(tonePupuk);     // Stop sound...
//  }

  
  JsonObject& getproses = jB.createObject();
  //Assign collected data to JSON Object
  getproses["statusAir"] = statusAir;
  getproses["statusPupuk"] = statusPupuk;
  
//  Send data to NodeMCU
  getproses.printTo(nodemcu);
  delay(1000);
  jB.clear();
  
}

  
