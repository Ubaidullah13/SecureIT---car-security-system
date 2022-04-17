/*************************************************************

  Blynk using a LED widget on your phone!

  App project setup:
    LED widget on V1
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
// #define BLYNK_TEMPLATE_ID "TMPLaaRHj-7a"
// #define BLYNK_DEVICE_NAME "Test"
#define BLYNK_AUTH_TOKEN "Your Auth Token"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Your SSID";
char pass[] = "Your Password";

int led1 = 23;

// WidgetLED led(V2);

// BlynkTimer timer;

// // V1 LED Widget is blinking
// void blinkLedWidget()
// {
//   if (led.getValue())
//   {
//     led.off();
//     Serial.println("LED on V1: off");
//     digitalWrite(23, LOW);
//   }
//   else
//   {
//     led.on();
//     Serial.println("LED on V1: on");
//     digitalWrite(23, HIGH);
//   }
// }

void setup()
{
  // Debug console
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);
  pinMode(led1, OUTPUT);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  //timer.setInterval(1000L, blinkLedWidget);
}

void loop()
{
  Blynk.run();
  //timer.run();
}