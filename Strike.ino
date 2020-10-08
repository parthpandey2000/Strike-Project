/* ---Soil Moisture Sensor
      VCC to 5V 
      Gnd to Gnd
      A0 to P34
   ---Water Level Sensor
      + to 5V
      - to Gnd
      S to P35
   ---Dust Sensor
      As per the Image*/
#include <WiFi.h>
#include <IOXhop_FirebaseESP32.h>
#include <dht.h>
#include <NTPClient.h>
#include <WiFiUdp.h>   
#define FIREBASE_HOST "examination-3bb69.firebaseio.com"                      
#define FIREBASE_AUTH "kxWcl1i3BN1UkixDy86HRI4vApkmVgRMF2UCDwMa" 
#define dhtpin 32 
const char* ssid     = "PandeyJi";   
const char* password = "Parth#2000";  
const int watle = 35;
const int mois = 34;
dht DHT;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800,60000);
String currentDate;         
void setup() {
    Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');
  
  WiFi.begin(ssid, password);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(ssid);
 
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(500);
    Serial.print('.');
  }
 
  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP()); 
Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); 

}
void loop() {
   timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();
 struct tm *ptm = gmtime ((time_t *)&epochTime); 

  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon+1;
 int currentYear = ptm->tm_year+1900;
 if(currentMonth<10){
  if(monthDay<10){
  currentDate = String(currentYear) + "-0" + String(currentMonth) + "-0" + String(monthDay) + "  " + timeClient.getFormattedTime();
  }
  else{
      currentDate = String(currentYear) + "-0" + String(currentMonth) + "-" + String(monthDay) + "  " + timeClient.getFormattedTime();
}
  }
  else{
   if(monthDay<10){
  currentDate = String(currentYear) + "-" + String(currentMonth) + "-0" + String(monthDay) + "  " + timeClient.getFormattedTime();
  }
  else{
      currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay) + "  " + timeClient.getFormattedTime();
}
  }
  DHT.read11(dhtpin);
  int hum = DHT.humidity;
  int temp = DHT.temperature;
  int moisv = analogRead(mois);
  int watlev = analogRead(watle);
  float mois1 = map(moisv,0,1024,0,100);
Firebase.push("Data/"+currentDate+"/Humidity",hum);
Firebase.push("Data/"+currentDate+"/Temperature",temp);
Firebase.push("Data/"+currentDate+"/Soil Moisture",mois1);
Firebase.push("Data/"+currentDate+"/Water Level",watlev);
}
