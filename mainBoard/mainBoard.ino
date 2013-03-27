#include <IRremote.h>

/* digital pin */
#define  IR_IN  0
#define  IR_LED 9
#define  SW1    14
#define  SW2    15
#define  LED_R  16
#define  LED_G  17

/* analog pin */
#define  PHOTO_TR  0
#define  TEMP_IC   1

#define  BUF_SIZE    20
#define  DATA_SIZE   128

int buf_cnt = 0;
int data_cnt = 0;
char buf[BUF_SIZE];
unsigned int data[DATA_SIZE];

IRrecv irrecv(IR_IN);
IRsend irsend;
decode_results ir_results;

void setup()
{
  pinMode(IR_IN, INPUT);
  pinMode(IR_LED, OUTPUT);
  digitalWrite(IR_LED, LOW);
  
  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_R, LOW);
  
  Serial.begin(9600);
  
  irrecv.enableIRIn();
}

void loop()
{
  /* Mode 0: IRSend
          1: IRRecv
          2: Get current illumination
          3: Get current temperature
  */

  switch(recvCmd()){
    case 0:
      recvIRData(buf, data);
      irsend.sendRaw(data, data_cnt, 38);
      Serial.println("send!");
      delay(100);
      break;
    
    case 1:
      delay(500);
      Serial.println();
      Serial.println("mode 1");
      break;
      
    case 2:
      delay(500);
      getCurrentIllumination();
      break;
    
    case 3:
      delay(500);
      getCurrentTemp();
      break;
      
    default:
      break;
  }
}

int recvCmd()
{
  clearBuffer(buf);
  buf_cnt = 0;
  
  while(1){
    if(Serial.available()){
      char c = Serial.read();
      
      if( c >= '0' && c <= '9' ) {
        buf[buf_cnt++] = c;
      }
      else if( c == 10 ){
        buf[buf_cnt] = '\0';
        return buf[0] != '\0' ? atoi(buf) : 999;
      }
    }
  }
}

void recvIRData(char* buf, unsigned int* data)
{
  clearBuffer(buf);
  clearData(data);
  data_cnt = 0;
  buf_cnt = 0;
  
  while(1){
    if(Serial.available()){
      char c = Serial.read();
      
      if( c == 44 ){
        buf[buf_cnt] = '\0';
        data[data_cnt++] = atoi(buf);
        buf_cnt = 0;
      }
      else if( c == 10 ){
        buf[buf_cnt] = '\0';
        data[data_cnt++] = atoi(buf);
        buf_cnt = 0;
        break; 
      }
      else{
        buf[buf_cnt++] = c;
      }
    }
  }
}

void clearBuffer(char* arr)
{
  for(int i=0; i < sizeof(arr); i++){
    buf[i] = '\0';
  }
}

void clearData(unsigned int* data)
{
  for(int i=0; i < sizeof(data); i++){
    buf[i] = 0;
  }
}

void getCurrentTemp()
{
  int sensor_val = analogRead(TEMP_IC);
  int voltage = map(sensor_val, 0, 1023, 0, 5000);
  int temp = map(voltage, 900, 4800, -30, 100);
  
  Serial.println();
  Serial.println(temp);
}

void getCurrentIllumination()
{
  int sensor_val = analogRead(PHOTO_TR);
  float voltage = ((long)sensor_val * 5000) / 1024;
  float microamp = (voltage * 1000) / 1000;
  float lx = microamp / (290 / 100);
  
  Serial.println();
  Serial.println(lx);
}
