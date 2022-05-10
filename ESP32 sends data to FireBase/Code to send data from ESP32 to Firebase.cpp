
#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

// Token
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

// WiFi credentials
#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "Password"

// For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

// API Key
#define API_KEY "FireBase API code"

// RTDB URL
#define DATABASE_URL "DataBase"

// User Email and password that already registred in Firebase project
#define USER_EMAIL "Email"
#define USER_PASSWORD "Password"

// Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
boolean signupOK = false;
unsigned long count = 0;

void setup()
{

  Serial.begin(115200);
  pinMode(23, OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // Wifi Setup
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

  // Assign the api key
  config.api_key = API_KEY;

// Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Assign the RTDB URL
  config.database_url = DATABASE_URL;

// Assign the callback function for the long running token generation task
// Token Connection
  config.token_status_callback = tokenStatusCallback; // From Library

  Firebase.begin(&config, &auth);

  // Pass False when reconnection conrolled manually
  Firebase.reconnectWiFi(true);

  Firebase.setDoubleDigits(5);

  // if (Firebase.signUp(&config, &auth, "", ""))
  // {
  //   Serial.println("OK");
  //   signupOK = true;
  // }
  // else
  // {
  //   Serial.printf("%s\n", config.signer.signupError.message.c_str());
  // }
}

void loop()
{

  // Firebase.ready() called repeatedly to handle authentication tasks.

  if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();

    // if (n == 1)
    // {
    //   Serial.println("Led On");
    //   digitalWrite(23, HIGH);
    //   delay(100);
    // }
    // else if (n == 0)
    // {
    //   Serial.println("Led Off");
    //   digitalWrite(23, LOW);
    //   delay(100);
    // }

    if (Firebase.RTDB.setInt(&fbdo, "test/int", count))
    {
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
      count++;
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
