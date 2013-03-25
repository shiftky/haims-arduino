#include <IRremote.h>

/* digital pin */
#define  IR_IN  0
#define  SW1    14
#define  SW2    15
#define  LED_G  16
#define  LED_R  17

/* analog pin */
#define  PHOTO_TR  0
#define  TEMP_IC   1

#define  BUF_SIZE    20
#define  DATA_SIZE   128

int buf_cnt = 0;
int data_cnt = 0;
char buf[BUF_SIZE];
unsigned int data[DATA_SIZE];

void setup()
{
  pinMode(IR_IN, INPUT);
  
  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  
  Serial.begin(9600);
}

void loop()
{
  switch(recvCmd()){
    case 0:
      Serial.println("mode 0");
      recvIRData(buf, data);
      for(int i=0; i < data_cnt; i++){
        Serial.println(data[i]);
      }
      break;
    
    case 1:
      Serial.println("mode 1");
      break;

    case 2:
      Serial.println("mode 2");
      break;
    
    case 10:
      Serial.println("mode 10");
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
