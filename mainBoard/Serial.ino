//===========================//
//                           //
//   HAIMS - Serial.ino      //
//                           //
//===========================//

void SerialInit()
{
  Serial.begin(9600);
}

void recvIRData(char* buf, unsigned int* data)
{
  clearBuffer(buf);
  clearData(data);
  data_cnt = 0;
  buf_cnt = 0;
  
  Serial.flush();
  while(1){
    if(Serial.available()){
      char c = Serial.read();

      if( c == 44 ){                  // ASCII code : ","
        buf[buf_cnt] = '\0';
        data[data_cnt++] = atoi(buf);
        buf_cnt = 0;
      }
      else if( c == 13 ){             // ASCII code : CR
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
