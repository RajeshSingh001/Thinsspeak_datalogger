#include <ESP8266WiFi.h>
#include <dht11.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

dht11 DHT11;
float t, h, X, X1;
int TEMP;

char ssid[] = SECRET_SSID;   // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

String myStatus = "";

void setup() {
  Serial.begin(9600);  // Initialize serial
  pinMode(D1, INPUT);
  DHT11.attach(D1);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  WiFi.begin(ssid, pass); //WiFi connection
  Serial.println("Waiting for connection");
  while (WiFi.status() != WL_CONNECTED)
  { //Wait for the WiFI connection completion
    delay(500);
    Serial.println(".");
  }
  Serial.println("\nConnected.");
}

void loop() {

  if (WiFi.status() == WL_CONNECTED)
  {
    int chk = DHT11.read();
    int temp = DHT11.temperature;
    int hum = DHT11.humidity;
    // set the fields with the values
    ThingSpeak.setField(1, temp);
    ThingSpeak.setField(2, hum);

    //    String tempStatus = ("Temp Value: " + temp);
    //    String humStatus = ("Hum Value: " + hum);

    String myStatus = "Data Logged";
    // set the status
    //    ThingSpeak.setStatus(tempStatus);
    //    ThingSpeak.setStatus(humStatus);
    ThingSpeak.setStatus(myStatus);


    // write to the ThingSpeak channel
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if (x == 200) {
      Serial.println("Channel update successful.");
    }
    else {
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }

    delay(15000); // Wait 20 seconds to update the channel again
  }

  else
  {
    Serial.println("No Internet Connection");
  }
}
