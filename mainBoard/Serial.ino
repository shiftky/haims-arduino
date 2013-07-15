#define  BUF_SIZE    20

int cmdBufCnt = 0;
char cmdBuf[BUF_SIZE];

void SerialInit()
{
  Serial.begin(9600);
}

void clearCmdBuffer(char* arr)
{
  for(int i=0; i < sizeof(arr); i++){
    cmdBuf[i] = '\0';
  }
}

int recvCmd()
{
  clearCmdBuffer(cmdBuf);
  cmdBufCnt = 0;
  
  while(1){
    if(Serial.available()){
      char c = Serial.read();
      
      if( c >= '0' && c <= '9' ) {
        cmdBuf[cmdBufCnt++] = c;
      }
      else if( c == 10 ){
        cmdBuf[cmdBufCnt] = '\0';
        return cmdBuf[0] != '\0' ? atoi(cmdBuf) : 999;
      }
    }
  }
}
