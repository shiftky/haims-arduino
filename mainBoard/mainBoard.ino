void setup()
{
  SensorInit();
  SerialInit();
  IRRecvInit();
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
      Serial.println("mode 0");
      break;
    
    case 1:
      Serial.println("mode 1");
      receiveIR();
      break;

    case 2:
      getCurrentIllumination();
      break;
    
    case 3:
      getCurrentTemp();
      break;
      
    default:
      break;
  }
}
