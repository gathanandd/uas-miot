// Library yang diperlukan
#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>
#include <DHT.h>

// Mendefinisikan pin dan tipe sensor DHT
#define DHTPIN D7
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Harus diisi
#define FIREBASE_HOST "https://iotprak---dht11-with-esp8266-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "Rzb2hn1lUwBp5OYhqtCGac6BZ8fW1S2sFul45HzY"
#define WIFI_SSID "Kost Kenanga"
#define WIFI_PASSWORD "kenanga76"  

// mendeklarasikan objek data dari FirebaseESP8266
FirebaseData firebaseData;

void setup() {
  
  Serial.begin(9600);
  
  dht.begin();
  
  // Koneksi ke Wifi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop(){
  
  // Sensor DHT11 membaca suhu dan kelembaban
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  // Memeriksa apakah sensor berhasil mambaca suhu dan kelembaban
  if (isnan(t) || isnan(h)) {
    Serial.println("Gagal membaca sensor DHT11");
    return;
  }

  // Menampilkan suhu dan kelembaban pada serial monitor
  Serial.print("Suhu: ");
  Serial.print(t);
  Serial.println(" *C");
  Serial.print("Kelembaban: ");
  Serial.print(h);
  Serial.println(" %");
  Serial.println();

  // Memberikan status suhu dan kelembaban kepada firebase
  if (Firebase.setFloat(firebaseData, "/Hasil_Pembacaan/suhu", t)){
      Serial.println("Suhu terkirim");
    } else{
      Serial.println("Suhu tidak terkirim");
      Serial.println("Karena: " + firebaseData.errorReason());
    } 
    
  if (Firebase.setFloat(firebaseData, "/Hasil_Pembacaan/kelembaban", h)){
      Serial.println("Kelembaban terkirim");
      Serial.println();
    } else{
      Serial.println("Kelembaban tidak terkirim");
      Serial.println("Karena: " + firebaseData.errorReason());
    }

      if (t > 9 && t < 20 ){
    Serial.println("Suhu Terlalu Rendah");
    Firebase.set(firebaseData, "/Hasil_Pembacaan/Hasil", "Suhu-Terlalu-Dingin");    
  } else if (t > 21 && t < 32){
    Serial.println("Suhu Rendah");
    Firebase.set(firebaseData, "/Hasil_Pembacaan/Hasil", "Suhu-Dingin");
  } else if (t > 31 && t < 37 ){
    Serial.println("Suhu Normal");
    Firebase.set(firebaseData, "/Hasil_Pembacaan/Hasil", "Suhu-Normal");
  } else if (t >= 38 && t <= 45){
    Serial.println("Suhu Tinggi");
    Firebase.set(firebaseData, "/Hasil_Pembacaan/Hasil", "Suhu-Hangat");
  }
    
  delay(1000);
}