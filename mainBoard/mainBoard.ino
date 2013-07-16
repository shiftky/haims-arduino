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
        recvIRData(buf, data);
        irsend.sendRaw(data, data_cnt+1, 38);
        delay(200);
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
    
    if (irrecv.decode(&results)) {
      IRDump(&results);
      irrecv.resume();
    }
}
