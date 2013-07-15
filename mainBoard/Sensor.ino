/* digital pin */
#define  SW1    14
#define  SW2    15
#define  LED_G  16
#define  LED_R  17

/* analog pin */
#define  PHOTO_TR  0
#define  TEMP_IC   1

void SensorInit()
{
  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
}

void getCurrentTemp()
{
  int sensor_val = analogRead(TEMP_IC);
  int voltage = map(sensor_val, 0, 1023, 0, 5000);
  int temp = map(voltage, 900, 4800, -35, 100);
  
  Serial.println(temp);
}

void getCurrentIllumination()
{
  int sensor_val = analogRead(PHOTO_TR);
  float voltage = ((long)sensor_val * 5000) / 1024;
  float microamp = (voltage * 1000) / 1000;
  float lx = microamp / (290 / 100);
  
  Serial.println(lx);
}
