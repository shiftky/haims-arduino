//===========================//
//                           //
//   HAIMS - mainBoard.ino   //
//                           //
//===========================//
#include <IRremote.h>

//===========================//
//  Define                   //
//===========================//
/* digital pin */
#define  PIN_IR_IN   0
#define  PIN_LED_OUT 9
#define  LED_G  17
#define  LED_R  16
#define  SW1    14
#define  SW2    15

/* buffer */
#define BUF_SIZE 32
#define DATA_SIZE 512

//===========================//
//  Global variables         //
//===========================//
/* IRRemote */
IRsend irsend;
IRrecv irrecv(PIN_IR_IN);
decode_results results;

/* buffer */
int buf_cnt = 0;
int data_cnt = 0;
char buf[BUF_SIZE];
unsigned int data[DATA_SIZE];

//===========================//
//  Setup                    //
//===========================//
void setup()
{
  SensorInit();
  SerialInit();
  
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  
  pinMode(PIN_IR_IN, INPUT);
  pinMode(PIN_LED_OUT, OUTPUT);
  irrecv.enableIRIn();
}

//===========================//
//  Main Loop                //
//===========================//
void loop()
{
    /* Mode 0: IRSend
            1: Get current illumination
            2: Get current temperature
    */
    char c;
    if(Serial.available() > 0){
      c = Serial.read();
    }
    switch(c){
      case '0':
        digitalWrite(LED_R, HIGH);
        recvIRData(buf, data);
        irsend.sendRaw(data, data_cnt+1, 38);
        digitalWrite(LED_R, LOW);
        blink_led();
        break;
        
      case '1':
        getCurrentIllumination();
        break;
      
      case '2':
        getCurrentTemp();
        break;
        
      default:
        break;
    }
    
    if (digitalRead(SW1) == LOW && irrecv.decode(&results)) {
      IRDump(&results);
      irrecv.resume();
    }
}

void blink_led()
{
  for (int i=0; i<2; i++) {
    digitalWrite(LED_G, HIGH);
    delay(50);
    digitalWrite(LED_G, LOW);
    delay(50);
  }
}

