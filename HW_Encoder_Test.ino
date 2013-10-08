/*
  Hockey Warrior - Encoder Test
  Tests the Encoder Sensors & prints the results. Can also run the motors.
 */
 
 #include "config.h"

int ENC_LEFT_On;
int ENC_LEFT_Off;
int ENC_LEFT_Diff;

int ENC_RIGHT_On;
int ENC_RIGHT_Off;
int ENC_RIGHT_Diff;

int ENC_LEFT_Count = 0;
int ENC_RIGHT_Count = 0;

int ENC_TEMPSTATE_Left = 0;
int ENC_TEMPRIGHT_State = 0;
int ENC_LEFT_State = 0;
int ENC_RIGHT_State = 0;

int averageCount = 1;

int mean = 400;
int STD = 100;

int delayTime = 2;

void setup() {
	// Set IR pins as outputs
  pinMode(ENCODER_POWER_PIN, OUTPUT);
  //pinMode(ENCODER_LEFT_PIN, OUTPUT);
  //pinMode(ENCODER_RIGHT_PIN, OUTPUT);

  Serial.begin(115200);      // open the serial port at 9600 bps:

  DEBUG_PRINT("Starting...");
}

void loop() {
	// Read each sensor in a loop
  digitalWrite(ENCODER_POWER_PIN, HIGH);
  delay(delayTime);
  ENC_LEFT_On = readSensor(ENCODER_LEFT_PIN, averageCount);
  ENC_RIGHT_On= readSensor(ENCODER_RIGHT_PIN, averageCount);

  digitalWrite(ENCODER_POWER_PIN, LOW);
  delay(delayTime);
  ENC_LEFT_Off = readSensor(ENCODER_LEFT_PIN, averageCount);
  ENC_RIGHT_Off= readSensor(ENCODER_RIGHT_PIN, averageCount);

  ENC_LEFT_Diff = ENC_LEFT_On - ENC_LEFT_Off;
  ENC_RIGHT_Diff = ENC_RIGHT_On - ENC_RIGHT_Off;
  
  ENC_TEMPSTATE_Left = calcState(ENC_LEFT_Diff, ENC_LEFT_State);
  ENC_TEMPRIGHT_State = calcState(ENC_RIGHT_Diff, ENC_RIGHT_State);
  
  if(ENC_LEFT_State != ENC_TEMPSTATE_Left)
    ENC_LEFT_Count++;

  if(ENC_RIGHT_State != ENC_TEMPRIGHT_State)
    ENC_RIGHT_Count++;

  ENC_LEFT_State = ENC_TEMPSTATE_Left;
  ENC_RIGHT_State = ENC_TEMPRIGHT_State;

  PLOT("ENC_LEFT_Off", ENC_LEFT_Off);
  PLOT("ENC_RIGHT_Off", ENC_RIGHT_Off);

  PLOT("ENC_LEFT_On", ENC_LEFT_On);
  PLOT("ENC_RIGHT_On", ENC_RIGHT_On);

  PLOT("ENC_LEFT_Diff", ENC_LEFT_Diff);
  PLOT("ENC_RIGHT_Diff", ENC_RIGHT_Diff);

  PLOT("ENC_LEFT_State", ENC_LEFT_State);
  PLOT("ENC_RIGHT_State", ENC_RIGHT_State);

  PLOT("ENC_LEFT_Count", ENC_LEFT_Count);
  PLOT("ENC_RIGHT_Count", ENC_RIGHT_Count);

  delay(10);
}


int readSensor(int pin, int averageCount)
{
  int value = 0;
  int i = 0;
  for(i = 0; i < averageCount; i++)
      value += analogRead(pin);
  return value/averageCount;
}

int calcState(int curValue, int curState)
{
  if(curState == 1)
  {
    return curValue > (mean - STD);
  }
  else
  {
    return curValue > (mean + STD);
  }
}
