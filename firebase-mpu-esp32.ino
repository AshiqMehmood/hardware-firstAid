#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
//Provide the token generation process info.
#include "addons/TokenHelper.h"

//from firebase console
#define API_KEY ""
#define FIREBASE_PROJECT_ID "first-aid-44aaf"

//define user creds
#define USER_EMAIL "project060422@gmail.com";
#define USER_PASSWORD "mainproject";

//unique id of device
#define DEVICE_ID "#SOS-999"

//wifi credentials
#define WIFI_SSID "Router"
#define WIFI_PWD "123456789"

FirebaseData fbdo;
FirebaseConfig config;
FirebaseAuth auth;

Adafruit_MPU6050 mpu;

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
  Serial.print("Firebase Client v");
  Serial.print(FIREBASE_CLIENT_VERSION);

   if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }
  delay(100);

  // put your setup code here, to run once:
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.token_status_callback = tokenStatusCallback;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  
}

void loop() {
  // put your main code here, to run repeatedly:
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
  /* Print out the values */
  delay(800);
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  if(a.acceleration.x > 6) {
     Serial.print("Fall Detected !!");
    
  //if(Firebase.ready() && (millis() - dataMills > 60000 || dataMills == 0)) {
    //dataMills = millis();
    // run every minute
    if(!taskCompleted) {
      FirebaseJson json;

      String documentPath = "testing/SOS999/triggers";
      String reading = "9988";
      String status1 = "pending";
      String deviceId = DEVICE_ID;

      json.set("fields/sensorValue/stringValue", String(reading).c_str());
      json.set("fields/shouldAlert/stringValue", String(status1).c_str());
      json.set("fields/deviceId/stringValue", String(deviceId).c_str());
      
          //add data 
      if(Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), json.raw())) {
        Serial.println("OK !");
        taskCompleted=true;
        //Serial.print(fbdo.payload().c_str());
      }
      else
        Serial.println(fbdo.errorReason());  

    }
    //String fetchDocPath = "devices/#SOS-999/triggers/sensor-reading-1";
    //get Document
  }
  //}

}
