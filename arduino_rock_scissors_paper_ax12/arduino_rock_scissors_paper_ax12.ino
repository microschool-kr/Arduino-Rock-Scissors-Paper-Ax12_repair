#include "SoftHalfDuplexSerial.h"
#include "DynamixelAx.h"

#define SCISSOR 1
#define ROCK 2
#define PAPER 14

#define NUM_MOTORS 3

#define UP_SPEED 200
#define DOWN_SPEED 130
#define UP_POSITION 195
#define DOWN_POSITION 500

softHalfDuplexSerial port(8); // data pin 8
dxlAx dxlCom(&port);

int count = 0;
int old_count = 0;
int distance = 0;
int max_distance = 12;
long randNumber;
int motors[NUM_MOTORS] = {SCISSOR, ROCK, PAPER};
int counts[NUM_MOTORS] = {0, 0, 0};

void setup() {
  Serial.begin(57600);
  randomSeed(analogRead(0));
  //pinMode(A0, INPUT);
  dxlCom.begin(57600);
  downAllMotors();
//  upAllMotors();
//  downMotor(SCISSOR);
//  downMotor(ROCK);
//  downMotor(PAPER);
  Serial.println("ready for start");
}

void loop() {
  int volt = map(analogRead(A0), 0, 1023, 0, 5000);
  distance = (27.61 / (volt - 0.1696)) * 1000;
  //Serial.println(distance);
  //Serial.println(analogRead(A0));

  if (distance <= max_distance) count = 1;
  else count = 0;

  if (count != old_count){
    if (count == 1 && old_count == 0)
    {
      randNumber = random(0, 3);
      //Serial.println(randNumber);
      if (++counts[randNumber] == 4)
      {
        int a = (randNumber + 1) % 3;
        int b = (randNumber + 2) % 3;
        if (counts[a] == counts[b])
        {
          int arr[2] = {a, b};
          randNumber = arr[random(0, 2)];
        }
        else
        {
          randNumber = counts[a] < counts[b] ? a : b;
        }
        counts[0] = counts[1] = counts[2] = 0;
      }
      Serial.println("detected!");
      delay(100);
      upMotor(motors[randNumber]);
      
      delay(1000);
    }
    else if (count == 0 && old_count == 1)
    {
      Serial.println("down start");
      delay(100);
      downAllMotors();
    }
  }
  old_count = count;
  
}

void downMotor(byte id)
{
  dxlCom.setGoalPositionAtSpeed(id, DOWN_POSITION, DOWN_SPEED);
  while(!dxlCom.dxlDataReady());        // waiting the answer of servo
  dxlCom.readDxlError();
  dxlCom.readDxlResult();
  bool isMoving = true;
  while (isMoving)
  {
    unsigned short error = DXL_ERR_SUCCESS;
    while(dxlCom.isBusy()); // waiting the status return delay time (for testing if it is moving)
    dxlCom.isMoving(id);
    while(!dxlCom.dxlDataReady());        // waiting the answer of servo
    error = dxlCom.readDxlError();
    isMoving = dxlCom.readDxlResult();
  }
  Serial.println("down over");
}

void downAllMotors()
{
  for (int i = 0; i < NUM_MOTORS; i++)
  {
    dxlCom.setGoalPositionAtSpeed(motors[i], DOWN_POSITION, DOWN_SPEED);
    while(!dxlCom.dxlDataReady());        // waiting the answer of servo
    dxlCom.readDxlError();
    dxlCom.readDxlResult();
  }
  bool isMoving = true;
  while (isMoving)
  {
    unsigned short error = DXL_ERR_SUCCESS;
    isMoving = false;
    while(dxlCom.isBusy()); // waiting the status return delay time (for testing if it is moving)
    for (int i = 0; i < NUM_MOTORS; i++)
    {
      dxlCom.isMoving(motors[i]);
      while(!dxlCom.dxlDataReady());        // waiting the answer of servo
      error = dxlCom.readDxlError();
      isMoving |= dxlCom.readDxlResult();
    }
  }
  
  Serial.println("down_All over");
}

void upMotor(byte id)
{
  dxlCom.setGoalPositionAtSpeed(id, UP_POSITION, UP_SPEED);
  while(!dxlCom.dxlDataReady());        // waiting the answer of servo
  dxlCom.readDxlError();
  dxlCom.readDxlResult();
  bool isMoving = true;
  while (isMoving)
  {
    unsigned short error = DXL_ERR_SUCCESS;
    while(dxlCom.isBusy()); // waiting the status return delay time (for testing if it is moving)
    dxlCom.isMoving(id);
    while(!dxlCom.dxlDataReady());        // waiting the answer of servo
    error = dxlCom.readDxlError();
    isMoving = dxlCom.readDxlResult();
  }
  Serial.println("up over");
}

void upAllMotors()
{
  for (int i = 0; i < NUM_MOTORS; i++)
  {
    dxlCom.setGoalPositionAtSpeed(motors[i], UP_POSITION, UP_SPEED);
    while(!dxlCom.dxlDataReady());        // waiting the answer of servo
    dxlCom.readDxlError();
    dxlCom.readDxlResult();
  }
  bool isMoving = true;
  while (isMoving)
  {
    unsigned short error = DXL_ERR_SUCCESS;
    isMoving = false;
    while(dxlCom.isBusy()); // waiting the status return delay time (for testing if it is moving)
    for (int i = 0; i < NUM_MOTORS; i++)
    {
      dxlCom.isMoving(motors[i]);
      while(!dxlCom.dxlDataReady());        // waiting the answer of servo
      error = dxlCom.readDxlError();
      isMoving |= dxlCom.readDxlResult();
    }
  }
}
