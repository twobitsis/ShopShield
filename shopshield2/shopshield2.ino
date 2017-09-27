



/*
  :Version: local Blynk and rewrite Beta 2.0
  :Author: twobitsis
  :Email: toborn@aol.com
  :Date: 06/02/2017
  :Revision: version#
  :License: Public Domain

  = Project: {ShopShield}

  07-15-17
  07-27-17
  09-13-17
*/

// Lib
#include <Arduino.h>
#include <WiFi101.h>
#include <BlynkSimpleWiFiShield101.h>
#include <Timer.h>
#include <DHT.h>

// Temp and Humidity
#define DHTPIN 12
#define DHTTYPE DHT11     // DHT 11 Type sensor
DHT dht(DHTPIN, DHTTYPE);

// login info
// Your network name and password.
//char auth[] = "612039ff0bfb44ae8711375ede8ab307";
//char auth[] = "041c0c7c1b054fad9cd79546dd9e1478";
//char auth[] = "041c0c7c1b054fad9cd79546dd9e1478";
char auth[] = "d3b4597285f94ce9ba1e230e55631bed";

char ssid1[] = "PikachuWay";  // Home
char pass1[] = "A512E316BA";

char ssid2[] = "PikachuWay_2GEXT"; // Shop
char pass2[] = "A512E316BA";

char ssid3[] = "Note"; // Samsung note pad
char pass3[] = "winter01";

char ssid4[] = "drtwo"; // Moto Phone
char pass4[] = "winter59";

char ssid5[] = "outlander"; // Offline Router
char pass5[] = "3032295507";

//

WidgetTerminal terminal(V1);
BLYNK_WRITE(V1);  // Terminal
BLYNK_WRITE(V2);  // Main screen trigger Button

int ledState = 0;
int shopTemp = 0;
int pinValue = 0;

Timer t1;
Timer t2;
Timer t3;

// declare functions
void netLogin();

//
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(true); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    terminal.println("Failed to read from DHT sensor!");
    terminal.flush();
}

  Blynk.virtualWrite(V5, t);
  Blynk.virtualWrite(V6, h);

  Serial.print("Temp:  ");
  Serial.println(t);
  Serial.print("Humidity: ");
  Serial.println(h);
  //
  terminal.print("Temp:  ");
  terminal.print(t);
  terminal.print("   Humidity: ");
  terminal.print(h);
  terminal.print("   Millis: ");
  terminal.println(millis() / 1000);
  terminal.flush();
  delay(500); // chill
}

void HB()
{
   // sent to 555 timer as a keep alive timer.
  analogWrite(A2, HIGH);
  delay(1000);
  analogWrite(A2, LOW);
  Serial.println(F(" HB Sent"));
  delay(500); // Chill
}

BLYNK_WRITE(V2)  // Main screen trigger Button
{
  pinValue = param.asInt(); // assigning incoming value from pin V2 to a variable
}


void button_PRESS()
{
  // Now lets check for a button press. (Shop Door Trigger)
  if (pinValue > 50)
  {
    pinValue = 0;
    Blynk.virtualWrite(V30, 100);
    Blynk.virtualWrite(V2, 100);
    digitalWrite(10, HIGH);
    delay(1500);
    digitalWrite(10, LOW);
    delay(5000);
    Blynk.virtualWrite(V30, 0);
    Blynk.virtualWrite(V2, 0);
  }

  //  float Door_statusV2 = (V2);
  //  if (Door_statusV2 > 50)
  //  {
  //    Blynk.virtualWrite(V30, 100);
  //    Blynk.virtualWrite(V2, 100);
  //    digitalWrite(10, HIGH);
  //    delay(1500);
  //    digitalWrite(10, LOW);
  //    delay(5000);
  //    Blynk.virtualWrite(V30, 0);
  //    Blynk.virtualWrite(V2, 0);
  //  }
}



void setup()
{
  WiFi.setPins(8, 7, 4, 2);
  Serial.begin(9600);
  //
  dht.begin();  // Warm up temp and Humitity Sensor
  //
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  pinMode(A2, OUTPUT); // HB
  delay(500); // Chill
  //
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(13, HIGH);
  //
  delay(500); // Chill
  netLogin();
  Serial.println("initial first wifi login");
  //
  delay(500);
  //
  t1.every(1000, my1Events);
  t2.every(5000, my5Events);
  t3.every(10000, my10Events);
}

// Main Continous loop
void loop()
{
  t1.update();
  t2.update();
  t3.update();
  Blynk.run();
}


//  main math stub run every 1 seconds AFTER succesful login into internet then Blynk
void my1Events()
{
  if (WiFi.status() != WL_CONNECTED) {
    delay(500);
    netLogin();
  }

  //  so user knows system status
  if (ledState == 0)
  {
    ledState = 255;
  } else
  { ledState = 0;
  }

  // Blink the "led" and update the time so user knows were live
  Blynk.virtualWrite(V3, ledState);
  Blynk.virtualWrite(V50, (millis() / 1000));

  // Check door sensors
  float sensorGDvalue = digitalRead(5);
  if (sensorGDvalue == HIGH)
  {
    Blynk.virtualWrite(V10, 100);
    Blynk.virtualWrite(V20, 0);
  } else {
    Blynk.virtualWrite(V10, 0);
    Blynk.virtualWrite(V20, 100);
  }

  float sensorMDvalue = digitalRead(6);
  if (sensorMDvalue == HIGH)
  {
    Blynk.virtualWrite(V11, 100);
    Blynk.virtualWrite(V21, 0);
  } else {
    Blynk.virtualWrite(V11, 0);
    Blynk.virtualWrite(V21, 100);
  }

  float sensorLUXvalue = digitalRead(12);
  if (sensorLUXvalue == HIGH)
  {
    Blynk.virtualWrite(V12, 100);
    Blynk.virtualWrite(V22, 100);
  } else {
    Blynk.virtualWrite(V12, 0);
    Blynk.virtualWrite(V22, 0);
  }

  button_PRESS();
}

//  main math stub run every 5 seconds AFTER succesful login into internet then Blynk to reset Watchdog HB
void my5Events()
{
  HB();
}

    //  main math stub run every 10 seconds AFTER succesful login into internet then Blynk
    void my10Events()
    {
      // voltage divider for battery.   Figure battery voltage and post here
      float sensorValue = analogRead(A7);
      sensorValue = (sensorValue * 2);
      float voltage = (sensorValue * 3.3) / 1024.0;
      voltage = ((voltage / 4.25 ) * 100);
      Blynk.virtualWrite(V4, voltage);
      sendSensor();
      delay(500); // Chill
    }



    //  Attempt to login into the world
    void netLogin()
    {
      delay(500);
      int x = 0;
      digitalWrite(13, HIGH);
      Serial.println("begining setup");

      WiFi.begin(ssid5, pass5);
      while (WiFi.status() != WL_CONNECTED) {
        Serial.println(" ");
        Serial.println(".");
        Serial.println("unable to connect to wifi");
        digitalWrite(13, LOW);
        delay(1000);
        digitalWrite(13, HIGH);
        delay(1000);
        x++;
        if (x > 10) {
          Serial.println("=====================================");
          Serial.println("Critical error connecting to wifi");
          x = 0;
        }
      }

      digitalWrite(13, LOW);
      delay(1000);
      digitalWrite(13, HIGH);

      Serial.println("Attempting to login to BLYNK");
      delay(1000);

      Blynk.config (auth, IPAddress(192, 168, 3, 100), 8442);

      while (Blynk.connect() == false)
      {
        Serial.println("Unable to login to Blynk, check ip...:");
        delay(1000);
      }

      terminal.println(("Blynk v" BLYNK_VERSION ": Device started"));
      terminal.println("-------------");
      //
      Serial.println(("Blynk v" BLYNK_VERSION ": Device started"));
      Serial.println("-------------");
      //
      delay(1000);
      digitalWrite(13, LOW);
      terminal.flush();
      delay(500);
    }

