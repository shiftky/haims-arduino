/* digital pin */
#define  PIN_IR_IN   0
#define  PIN_LED_OUT 9

// AEHA format
#define SIGNAL_LEADER_HIGH_TIME 8
#define SIGNAL_LEADER_LOW_TIME  4
#define SIGNAL_TIMEOUT 4000

#define DATA_LEN 512
byte buf[DATA_LEN];

void IRRecvInit()
{
  pinMode(PIN_IR_IN, INPUT);
  pinMode(PIN_LED_OUT, OUTPUT);
  
  clearBuf(DATA_LEN);
}

void clearBuf(int range)
{
  for (int i = 0; i < range; i++)
    buf[i] = 0;
}

// see pulseIn() code. (hardware/arduino/cores/arduino/wiring_pulse.c)
unsigned long switchIn(uint8_t pin, uint8_t state, unsigned long timeout)
{
  uint8_t bit = digitalPinToBitMask(pin);
  uint8_t port = digitalPinToPort(pin);
  uint8_t stateMask = (state ? bit : 0);
  unsigned long width = 0;
  
  unsigned long numloops = 0;
  unsigned long maxloops = microsecondsToClockCycles(timeout) / 16;
  
  // wait for the signal to start
  while ((*portInputRegister(port) & bit) != stateMask)
    if (numloops++ == maxloops)
      return 0;
  
  // wait for the signal to stop
  while ((*portInputRegister(port) & bit) == stateMask) {
    if (numloops++ == maxloops)
      return 0;
    width++;
  }

  return clockCyclesToMicroseconds(width * 21 + 16); 
}

int receiveIR()
{
  // IR High Signal -> Pin is LOW  (0)
  // IR Low Signal  -> Pin is HIGH (1)  
  digitalWrite(PIN_LED_OUT, LOW);
  Serial.print("ready\n");
  
  // wait signal
  while (digitalRead(PIN_IR_IN) != 0);
  
  int count = 0;
  unsigned long t = 0;
  unsigned long tHigh, tLow;
  
  // leader
  tHigh = switchIn(PIN_IR_IN, LOW, SIGNAL_TIMEOUT);
  tLow = switchIn(PIN_IR_IN, HIGH, SIGNAL_TIMEOUT);
  digitalWrite(PIN_LED_OUT, HIGH);
  
  // calc TimeUnit from leader.
  t = (tHigh + tLow) / (SIGNAL_LEADER_HIGH_TIME + SIGNAL_LEADER_LOW_TIME);
  t *= 2;
  
  // receive data bits.
  for (int i = 0; i < DATA_LEN; i++) {
    byte in = 0;
    for (int j = 0; j < 8; j++) {
      tHigh = switchIn(PIN_IR_IN, LOW, SIGNAL_TIMEOUT);
      if (tHigh == 0) goto ERROR_RECEIVE;
      tLow = switchIn(PIN_IR_IN, HIGH, SIGNAL_TIMEOUT);
      if (tLow == 0) goto END_RECEIVE;
      
      in |= (tLow > t) ? in |= (0x1 << j) : 0;
    }
    buf[i] = in;
    count++;
  }

ERROR_RECEIVE:
  Serial.println("error");

END_RECEIVE:
  Serial.print("T=");
  Serial.println(t / 2);
  for (int i = 0; i < count; i++) {
    Serial.print(buf[i], HEX);
    Serial.write(',');
  }
  Serial.write('\n');

  clearBuf(count);
  digitalWrite(PIN_LED_OUT, LOW);
}

