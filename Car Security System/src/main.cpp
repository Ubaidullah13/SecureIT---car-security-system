#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>
#include <IOXhop_FirebaseESP32.h>
#endif

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

// Wifi credentials
#define WIFI_SSID "I-14/1"
#define WIFI_PASSWORD "I141@NewCity"

// Database API key
#define API_KEY "AIzaSyAso44Edq1o-gxeRiiTygxmR3k5vPpG384"

// Database URL
#define DATABASE_URL "esp32-b7a00-default-rtdb.firebaseio.com"

// Registered User Email and Password
#define USER_EMAIL "ubaid.99es@gmail.com"
#define USER_PASSWORD "TestEsp32"

// PINS
#define LOCK 23
#define DOOR 22
#define ENGINE 21
#define RELAY 19

// Signal Variable
int secureSignal = 0;
int lockSignal = 0;
int doorSignal = 0;
int engineSignal = 0;
int resetSignal = 0;

// Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
// unsigned long count = 0;
// int intValue;

// int engineCount = 0;
// int doorCount = 0;
// int LockCount = 0;
void setup()
{

  Serial.begin(115200);

  // Input pins -> Signal form Component towrds ESP
  pinMode(LOCK, INPUT);
  pinMode(DOOR, INPUT);
  pinMode(ENGINE, INPUT);

  // Output Pins -> Signal from ESP towards Component
  pinMode(RELAY, OUTPUT);

  //  pinMode(2, OUTPUT);

  // Setting Wifi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");

  // Waiting For connection
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  // Api key (Important to link with database)
  config.api_key = API_KEY;

  // Sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // The Database URL */
  config.database_url = DATABASE_URL;

  // Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // addons/TokenHelper.h

  Firebase.begin(&config, &auth);

  // Comment or pass false value when WiFi reconnection will control manually or third party library
  Firebase.reconnectWiFi(true);

  Firebase.setDoubleDigits(5);
}

void loop()
{
  // Getting Firebase Ready and delay
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 500 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis(); // For delay

    // Secure Lock -> when this is 1, car will be fully locked and engine stoped until reset with app
    Firebase.RTDB.getInt(&fbdo, "/SecureLock/status");
    secureSignal = fbdo.intData();

    if (secureSignal == 1)
    {
      // digitalWrite(LOCK, LOW);
      // digitalWrite(DOOR, LOW);
      digitalWrite(RELAY, HIGH);
      if (Firebase.RTDB.setInt(&fbdo, "Engine/status", 1))
      {
        Serial.println("Fully Secured");
        Serial.println("");
      }
      else
      {
        Serial.println("FAILED -> REASON: " + fbdo.errorReason());
      }
    }
    else
    {
      // Read Lock Signal
      lockSignal = digitalRead(LOCK);
      if (lockSignal == LOW) // Car Locked
      {
        Serial.println("Car Locked");

        // Read Door Signal
        doorSignal = digitalRead(DOOR);
        // Door Opened
        if (doorSignal == HIGH)
        {
          Serial.println("Illegal Door Opening");
          digitalWrite(RELAY, HIGH);
          if (Firebase.RTDB.setInt(&fbdo, "Engine/status", 1))
          {
            Serial.println("Owner Notified");
            Serial.println("");
          }
          else
          {
            Serial.println("FAILED -> REASON: " + fbdo.errorReason());
          }
        }

        // Door Closed
        else
        {
          Serial.println("Door Closed");

          // Read Engine Signal
          engineSignal = digitalRead(ENGINE);
          // Engine starts Illegal
          if (engineSignal == HIGH)
          {
            Serial.println("Alert! Engine: ON");
            digitalWrite(RELAY, HIGH);
            if (Firebase.RTDB.setInt(&fbdo, "Engine/status", 1))
            {
              Serial.println("Owner Notified");
              Serial.println("");
            }
            else
            {
              Serial.println("FAILED -> REASON: " + fbdo.errorReason());
            }
          }

          // Engine OFF
          else
          {
            Serial.println("Engine: OFF");
            Serial.println("");
          }
        }
      }

      // Car Unlocked
      else
      {
        Serial.println("Car UnLocked");
        Serial.println("");
      }
    }

    // Reset Back to Normal
    if (Firebase.RTDB.getInt(&fbdo, "/Reset/status"))
    {
      resetSignal = fbdo.intData();
      if (resetSignal == HIGH)
      {
        digitalWrite(RELAY, LOW);
        if (Firebase.RTDB.setInt(&fbdo, "Engine/status", 0))
        {
          Serial.println("Engine Reset");
          Serial.println("");
        }
        else
        {
          Serial.println("FAILED -> REASON: " + fbdo.errorReason());
        }
      }
    }
  }
}

// // Write Data
// if (Firebase.RTDB.setInt(&fbdo, "test/int", n))
// {
//   Serial.println("PASSED");
//   n++;
// }
// else
// {
//   Serial.println("FAILED -> REASON: " + fbdo.errorReason());
// }

// // Read Data
// if (Firebase.RTDB.getInt(&fbdo, "/test/int"))
// {
//   read_data = fbdo.intData();
//   if (read_data == 0)
//   {
//     Serial.print("Data is: ");
//     Serial.print(read_data);
//   }
// }