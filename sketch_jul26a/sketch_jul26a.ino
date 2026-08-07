#include <DHT.h>

#define DHTPIN 4 //pin suhu
#define DHTTYPE DHT11 
#define RELAY1 25


#define LED_HIJAU  32
#define LED_KUNING 33
#define LED_MERAH  27
// #define RELAY2 26

DHT dht(DHTPIN, DHTTYPE);

float batasSuhu = 30.0; // suhu batas (ubah sesuai kebutuhan)
float suhuBahaya = 35.0;


void setup() {
  Serial.begin(115200);
  
  dht.begin(); 

  pinMode(RELAY1, OUTPUT);
  digitalWrite(RELAY1, HIGH); // OFF

  // for led lamp
  pinMode(LED_HIJAU, OUTPUT);
  pinMode(LED_KUNING, OUTPUT);
  pinMode(LED_MERAH, OUTPUT);
}

void loop() {
  float suhu = dht.readTemperature();

  if(isnan(suhu)) {
    Serial.println("Gagal baca DHT");
    return;
  }

   // Reset semua lampu
  digitalWrite(LED_HIJAU, LOW);
  digitalWrite(LED_KUNING, LOW);
  digitalWrite(LED_MERAH, LOW);

  Serial.print("Suhu: ");
  Serial.print(suhu);
  Serial.println(" °C");

  if(suhu >= suhuBahaya) {
    digitalWrite(LED_MERAH, HIGH);

    Serial.print("Suhu bahaya: ");
    Serial.print(suhu);
    Serial.println(" °C");
  }else if (suhu > batasSuhu) {
    digitalWrite(LED_KUNING, HIGH);
    digitalWrite(RELAY1, LOW); // NYALA
    Serial.println("RELAY ON (PANAS)");
  } else  {
    digitalWrite(LED_HIJAU, HIGH);
    digitalWrite(RELAY1, HIGH); // MATI
    Serial.println("RELAY OFF (AMAN)");
  }

  delay(2000); 
}
