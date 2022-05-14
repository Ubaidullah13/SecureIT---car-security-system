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

/* 1. Define the WiFi credentials */
#define WIFI_SSID "I-14/1"
#define WIFI_PASSWORD "I141@NewCity"

// For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "AIzaSyAso44Edq1o-gxeRiiTygxmR3k5vPpG384"

/* 3. Define the RTDB URL */
#define DATABASE_URL "esp32-b7a00-default-rtdb.firebaseio.com" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that already registred or added in project */
#define USER_EMAIL "ubaid.99es@gmail.com"
#define USER_PASSWORD "TestEsp32"

// Project variables
#define Lock 22
#define Door 21
#define Relay 19
#define engine 23

int LockSignal = 0;
int DoorSignal = 0;
int EngineSignal = 0;

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;
int read_data;
unsigned long sendDataPrevMillis = 0;
unsigned long count = 0;
int intValue;

int engineCount = 0;
int doorCount = 0;
int LockCount = 0;
void setup()
{

  Serial.begin(115200);
  pinMode(Lock, INPUT);
  pinMode(Door, INPUT);
  pinMode(Relay, OUTPUT);
  pinMode(engine, INPUT);
  pinMode(2, OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
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

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  Firebase.begin(&config, &auth);

  // Comment or pass false value when WiFi reconnection will control by your code or third party library
  Firebase.reconnectWiFi(true);

  Firebase.setDoubleDigits(5);
}

void loop()
{
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 100 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();

    if (Firebase.RTDB.getInt(&fbdo, "/test/int"))

    {

      if (fbdo.dataType() == "int")
      {

        read_data = fbdo.intData();
        if (read_data == 0)
        {
          Serial.print("FALSE");
          Serial.println(read_data); // print the data received from the Firebase database
          digitalWrite(2, LOW);
        }
        else
        {
          Serial.print("TRUE");
          digitalWrite(2, HIGH);
        }
      }
    }

    else

    {

      Serial.println(fbdo.errorReason()); // print he error (if any)
    }
  }

  // Firebase.ready() should be called repeatedly to handle authentication tasks.
  // millis() -> Returns the number of milliseconds passed since the Arduino board began running the current program. This number will overflow (go back to zero), after approximately 50 days.
  // if (Firebase.ready() && (millis() - sendDataPrevMillis > 100 || sendDataPrevMillis == 0))
  // {
  //   sendDataPrevMillis = millis();

  //   LockSignal = digitalRead(Lock);

  //   EngineSignal = digitalRead(engine);

  //   if (LockSignal == LOW)  // If car is locked
  //   {
  //     //LockCount = 0;
  //     Serial.println("Car Locked");
  //     DoorSignal = digitalRead(Door);
  //     if (Firebase.RTDB.setInt(&fbdo, "Lock/int", LockSignal))
  //     {
  //       Serial.println("PASSED");
  //       Serial.println("PATH: " + fbdo.dataPath());
  //       Serial.println("TYPE: " + fbdo.dataType());
  //     }
  //     else
  //     {
  //       Serial.println("FAILED");
  //       Serial.println("REASON: " + fbdo.errorReason());
  //     }

  //     if (DoorSignal == HIGH)
  //     {
  //       doorCount = 1;
  //       Serial.println("Illegal Door Opening");
  //       digitalWrite(Relay, HIGH);
  //       if (Firebase.RTDB.setInt(&fbdo, "Door/int", doorCount))
  //       {
  //         Serial.println("PASSED");
  //         Serial.println("PATH: " + fbdo.dataPath());
  //         Serial.println("TYPE: " + fbdo.dataType());
  //       }
  //       else
  //       {
  //         Serial.println("FAILED");
  //         Serial.println("REASON: " + fbdo.errorReason());
  //       }
  //     }
  //     else
  //     {
  //       doorCount = 0;
  //       Serial.println("Door Closed");
  //       if (Firebase.RTDB.setInt(&fbdo, "Door/int", doorCount))
  //       {
  //         Serial.println("PASSED");
  //         Serial.println("PATH: " + fbdo.dataPath());
  //         Serial.println("TYPE: " + fbdo.dataType());
  //       }
  //       else
  //       {
  //         Serial.println("FAILED");
  //         Serial.println("REASON: " + fbdo.errorReason());
  //       }
  //       if (EngineSignal == HIGH)
  //       {
  //         engineCount = 1;
  //         Serial.println("Illegal Engine On");
  //         digitalWrite(Relay, HIGH);
  //         // Firebase
  //         if (Firebase.RTDB.setInt(&fbdo, "EngineStarts/int", engineCount))
  //         {
  //           Serial.println("PASSED");
  //           Serial.println("PATH: " + fbdo.dataPath());
  //           Serial.println("TYPE: " + fbdo.dataType());
  //         }
  //         else
  //         {
  //           Serial.println("FAILED");
  //           Serial.println("REASON: " + fbdo.errorReason());
  //         }
  //       }
  //     }
  //   }
  //   else if (LockSignal == HIGH)
  //   {
  //     LockCount = 1;
  //     Serial.println("Car Unlocked");
  //     if (Firebase.RTDB.setInt(&fbdo, "Lock/int", LockCount))
  //     {
  //       Serial.println("PASSED");
  //       Serial.println("PATH: " + fbdo.dataPath());
  //       Serial.println("TYPE: " + fbdo.dataType());
  //     }
  //     else
  //     {
  //       Serial.println("FAILED");
  //       Serial.println("REASON: " + fbdo.errorReason());
  //     }
  //   }
  //   Serial.println();
  //   delay(1000);

  // if (Firebase.RTDB.setInt(&fbdo, "test/int", count))
  // {
  //   Serial.println("PASSED");
  //   Serial.println("PATH: " + fbdo.dataPath());
  //   Serial.println("TYPE: " + fbdo.dataType());
  //   count++;
  // }
  // else
  // {
  //   Serial.println("FAILED");
  //   Serial.println("REASON: " + fbdo.errorReason());
  // }
  //}
}