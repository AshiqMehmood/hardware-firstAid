#include <WiFi.h>
#include <Firebase_ESP_Client.h>
//Provide the token generation process info.
#include "addons/TokenHelper.h"

//from firebase console
#define API_KEY ""
#define FIREBASE_PROJECT_ID "first-aid-44aaf"

//define user creds
#define USER_EMAIL "project060442@gmail.com";
#define USER_PASSWORD "mainproject";

//unique id of device
#define DEVICE_ID "#SOS-999"

//wifi credentials
#define WIFI_SSID "Router"
#define WIFI_PWD "123456789"

FirebaseData fbdo;
FirebaseConfig config;
FirebaseAuth auth;

bool taskCompleted = false;
unsigned long dataMills = 0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PWD);
  Serial.print("Connecting to wifi....");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("Connected with IP: ");
  Serial.print(WiFi.localIP());
  Serial.print();
  Serial.print("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  // put your setup code here, to run once:
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.token_status_callback = tokenStatusCallback;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWifi(true);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(Firebase.ready() && (millis() - dataMills > 60000 || dataMills == 0)) {
    dataMills = millis();
    // run only once
    if(!taskCompleted) {
      FirebaseJson json;

      String documentPath = "devices/#SOS-999/triggers/sensor-reading-1";
      String reading = "891";
      String status = "pending";

      json.set("fields/sensorValue/stringValue", reading);
      json.set("fields/shouldAlert/stringValue", status);
      
      Serial.print("Create document... ");
      //add data 
      if(Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), json.raw()))
        Serial.println("OK ! \n%s\n\n", fbdo.payload().c_str());
      else
        Serial.println(fbdo.errorReason());  

    }
    String fetchDocPath = "devices/#SOS-999/triggers/sensor-reading-1";
    //get Document
  }

}
