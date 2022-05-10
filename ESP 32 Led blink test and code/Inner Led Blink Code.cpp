#include <Arduino.h>

#define led 2

void setup() {
  pinMode(led, OUTPUT);
}

void loop() {
// Blink Fast
  for (int i = 0; i < 5; i++)
  {
      digitalWrite(led ,HIGH);

      delay(500);

      digitalWrite(led, LOW);

      delay(500);
  }
  
  // Blink Slow
  for (int i = 0; i < 5; i++)
  {
      digitalWrite(led ,HIGH);

      delay(1000);

      digitalWrite(led, LOW);

      delay(1000);
  }

}
