enum e_device
{
  FAN = 0,
  PUMP = 1,
  HEATER = 2
};

typedef struct _Device
{
  int pin;
  char onInst;
  char offInst;
} s_device;

s_device dev[3] = {{13, 'A', 'a'},   //FAN
                   {12, 'W', 'w'},   //PUMP
                   {10, 'H', 'h'}};  //HEATER

int inByte = 0;
int digits_read = 0;
int brightness = 0;
int lastBrightness = 0;
int lastChange = 0;
const int DIM_INTERVAL = 800;
const int LIGHT_PIN = 11;
long __map(long x, long in_min, long in_max, long out_min, long out_max)
{
      return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
void setup() {
  // put your setup code here, to run once:
  pinMode(dev[FAN].pin, OUTPUT);
  pinMode(dev[PUMP].pin, OUTPUT);
  pinMode(dev[HEATER].pin, OUTPUT);
  pinMode(LIGHT_PIN, OUTPUT);
  digitalWrite(dev[FAN].pin, LOW);
  digitalWrite(dev[PUMP].pin, LOW);
  digitalWrite(dev[HEATER].pin, LOW);
  analogWriteFreq(1000);
  analogWrite(LIGHT_PIN, 0);
  Serial.begin(9600);
  while(!Serial);
}

void loop() {
  static int tmp_brightness = 0;
  if(Serial.available() > 0)
  {
    inByte = Serial.read();
    if(inByte == 'L')
    {
      digits_read = 0;
      tmp_brightness = 0;
    }
    if(inByte >= '0' && inByte <='9')
    {
      tmp_brightness *= 10;
      tmp_brightness += inByte - '0';
      digits_read++;
      if(digits_read == 3)
      {
        lastBrightness = brightness;
        lastChange = millis();
        brightness = tmp_brightness;
        //analogWrite(LIGHT_PIN, brightness);
        //Serial.println(brightness);
      }
    }
    if(inByte == dev[FAN].onInst)  digitalWrite(dev[FAN].pin, HIGH);
    if(inByte == dev[FAN].offInst) digitalWrite(dev[FAN].pin, LOW);
    if(inByte == dev[PUMP].onInst)
    {
      digitalWrite(dev[PUMP].pin, HIGH);
      delay(1000);
      digitalWrite(dev[PUMP].pin, LOW);
    }
    if(inByte == dev[HEATER].onInst)  digitalWrite(dev[HEATER].pin, HIGH);
    if(inByte == dev[HEATER].offInst) digitalWrite(dev[HEATER].pin, LOW);
  }
  int remainDimming = millis() - lastChange;
  if(remainDimming > DIM_INTERVAL)
  {
    remainDimming = DIM_INTERVAL;
  }
  int brightness_dim = __map(remainDimming, 0, DIM_INTERVAL, lastBrightness, brightness);
  analogWrite(LIGHT_PIN, brightness_dim);
}
