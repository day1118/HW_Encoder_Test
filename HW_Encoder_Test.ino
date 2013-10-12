/*
  Hockey Warrior - Encoder Test
  Tests the Encoder Sensors & prints the results. Can also run the motors.
 */
 
  #include "config.h"

  #define MOTORS_ENABLE

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

float scaleFactorInc = 1;
float scaleFactorDec = 8;

int motorSpeed = 220;
int ENC_LEFT_Motor_Speed = motorSpeed;
int ENC_RIGHT_Motor_Speed = motorSpeed;

void setup() {
	// Set IR pins as outputs
  pinMode(ENCODER_POWER_PIN, OUTPUT);
  
  #ifdef MOTORS_ENABLE
    pinMode(MOTOR_L_A_PIN, OUTPUT);
    pinMode(MOTOR_L_B_PIN, OUTPUT);  
    pinMode(MOTOR_L_ENABLE_PIN, OUTPUT);  

    pinMode(MOTOR_R_A_PIN, OUTPUT);
    pinMode(MOTOR_R_B_PIN, OUTPUT);  
    pinMode(MOTOR_R_ENABLE_PIN, OUTPUT);  

    pinMode(MOTOR_B_A_PIN, OUTPUT);
    pinMode(MOTOR_B_B_PIN, OUTPUT);  
    pinMode(MOTOR_B_ENABLE_PIN, OUTPUT);  

    digitalWrite(MOTOR_L_A_PIN, HIGH);
    digitalWrite(MOTOR_L_B_PIN, LOW);
    analogWrite(MOTOR_L_ENABLE_PIN, ENC_LEFT_Motor_Speed);

    digitalWrite(MOTOR_R_A_PIN, HIGH);
    digitalWrite(MOTOR_R_B_PIN, LOW);
    analogWrite(MOTOR_R_ENABLE_PIN, ENC_RIGHT_Motor_Speed);
  #endif
  
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

  int encDiff = abs(ENC_LEFT_Count - ENC_RIGHT_Count);
  if(ENC_RIGHT_Count > ENC_LEFT_Count)
  {
    ENC_LEFT_Motor_Speed = motorSpeed + (encDiff*scaleFactorInc);
    ENC_RIGHT_Motor_Speed = motorSpeed - (encDiff*scaleFactorDec);
  }
  else
  {
    ENC_LEFT_Motor_Speed = motorSpeed - (encDiff*scaleFactorDec);
    ENC_RIGHT_Motor_Speed = motorSpeed + (encDiff*scaleFactorInc);
  }

  ENC_LEFT_Motor_Speed = limit(ENC_LEFT_Motor_Speed);
  ENC_RIGHT_Motor_Speed = limit(ENC_RIGHT_Motor_Speed);

  analogWrite(MOTOR_L_ENABLE_PIN, ENC_LEFT_Motor_Speed);
  analogWrite(MOTOR_R_ENABLE_PIN, ENC_RIGHT_Motor_Speed);

  //PLOT("ENC_LEFT_Off", ENC_LEFT_Off);
  //PLOT("ENC_RIGHT_Off", ENC_RIGHT_Off);

  //PLOT("ENC_LEFT_On", ENC_LEFT_On);
  //PLOT("ENC_RIGHT_On", ENC_RIGHT_On);

  PLOT("ENC_LEFT_Diff", ENC_LEFT_Diff);
  PLOT("ENC_RIGHT_Diff", ENC_RIGHT_Diff);

  PLOT("ENC_LEFT_State", ENC_LEFT_State);
  PLOT("ENC_RIGHT_State", ENC_RIGHT_State);

  PLOT("ENC_LEFT_Count", ENC_LEFT_Count);
  PLOT("ENC_RIGHT_Count", ENC_RIGHT_Count);

  //PLOT("ENC_LEFT_Motor_Speed", ENC_LEFT_Motor_Speed);
  //PLOT("ENC_RIGHT_Motor_Speed", ENC_RIGHT_Motor_Speed);

  //PLOT("encDiff", encDiff)

  //delay(1);
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

int limit(int value)
{
  if(value > 255)
    return 255;
  else if(value < 0)
    return 0;
  else
    return value;
}