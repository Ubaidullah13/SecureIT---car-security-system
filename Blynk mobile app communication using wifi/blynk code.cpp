#define BLYNK_AUTH_TOKEN "Your Auth Token"

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

// ALTERNATE METHOD
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
  
}

void loop()
{
  Blynk.run();
  //timer.run();
}
