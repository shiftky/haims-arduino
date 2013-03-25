#include <IRremote.h>

#define  IR_IN  0
#define  SW1    14
#define  SW2    15
#define  LED_G  16
#define  LED_R  17

void setup()
{
  pinMode(IR_IN, INPUT);
  
  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
}

void loop()
{
  digitalWrite(LED_G, HIGH);
  digitalWrite(LED_R, LOW);
  delay(500);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_R, HIGH);
  delay(500);
}
