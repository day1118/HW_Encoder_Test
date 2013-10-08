/*
  Hockey Warrior - Encoder Test
  Tests the Encoder Sensors & prints the results. Can also run the motors.
 */
 
 #include "config.h"

int ENC_Left;
int ENC_Right;

int ENC_COUNT_Left = 0;
int ENC_COUNT_Right = 0;

int ENC_TEMPSTATE_Left = 0;
int ENC_TEMPSTATE_Right = 0;
int ENC_STATE_Left = 0;
int ENC_STATE_Right = 0;

int averageCount = 1;

int mean = 600;
int STD = 200;

void setup() {
	// Set IR pins as outputs
  pinMode(ENCODER_LEFT_PIN, OUTPUT);
  pinMode(ENCODER_RIGHT_PIN, OUTPUT);

  Serial.begin(115200);      // open the serial port at 9600 bps:

  DEBUG_PRINT("Starting...");
}

void loop() {
	// Read each sensor in a loop
  ENC_Left = readSensor(ENCODER_LEFT_PIN, averageCount);
  ENC_Right = readSensor(ENCODER_RIGHT_PIN, averageCount);
  
  ENC_TEMPSTATE_Left = calcState(ENC_Left, ENC_STATE_Left);
  ENC_TEMPSTATE_Right = calcState(ENC_Right, ENC_STATE_Right);
  
  if(ENC_STATE_Left != ENC_TEMPSTATE_Left)
    ENC_COUNT_Left++;

  if(ENC_STATE_Right != ENC_TEMPSTATE_Right)
    ENC_COUNT_Right++;

  ENC_STATE_Left = ENC_TEMPSTATE_Left;
  ENC_STATE_Right = ENC_TEMPSTATE_Right;

  PLOT("ENC_Left", ENC_Left);
  PLOT("ENC_Right", ENC_Right);

  PLOT("ENC_STATE_Left", ENC_STATE_Left);
  PLOT("ENC_STATE_Right", ENC_STATE_Right);

  PLOT("ENC_COUNT_Left", ENC_COUNT_Left);
  PLOT("ENC_COUNT_Right", ENC_COUNT_Right);
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
    return curValue < (mean + STD);
  }
}
