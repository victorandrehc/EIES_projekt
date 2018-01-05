/* This example uses the front proximity sensor on the Zumo 32U4
Front Sensor Array to locate an opponent robot or any other
reflective object. Using the motors to turn, it scans its
surroundings. If it senses an object, it turns on its yellow LED
and attempts to face towards that object. */

#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4Motors motors;
Zumo32U4ProximitySensors proxSensors;
Zumo32U4ButtonA buttonA;

// A sensors reading must be greater than or equal to this
// threshold in order for the program to consider that sensor as
// seeing an object.
const uint8_t sensorThreshold = 4;

// The maximum speed to drive the motors while turning.  400 is
// full speed.
const uint16_t turnSpeedMax = 400;

// The minimum speed to drive the motors while turning.  400 is
// full speed.
const uint16_t turnSpeedMin = 100;

// The amount to decrease the motor speed by during each cycle
// when an object is seen.
const uint16_t deceleration = 10;

// The amount to increase the speed by during each cycle when an
// object is not seen.
const uint16_t acceleration = 10;

#define LEFT 0
#define RIGHT 1

// Stores the last indication from the sensors about what
// direction to turn to face the object.  When no object is seen,
// this variable helps us make a good guess about which direction
// to turn.
bool senseDir = RIGHT;

// True if the robot is turning left (counter-clockwise).
bool turningLeft = false;

// True if the robot is turning right (clockwise).
bool turningRight = false;

// If the robot is turning, this is the speed it will use.
uint16_t turnSpeed = turnSpeedMax;

// The time, in milliseconds, when an object was last seen.
uint16_t lastTimeObjectSeen = 0;

void setup()
{
  proxSensors.initFrontSensor();

  // Wait for the user to press A before driving the motors.
  lcd.clear();
  lcd.print(F("Press A"));
  //buttonA.waitForButton();
  while(!buttonA.isPressed()){
    proxSensors.read();
    uint8_t leftValue = proxSensors.countsFrontWithLeftLeds();
    uint8_t rightValue = proxSensors.countsFrontWithRightLeds();
    lcd.clear();
    lcd.gotoXY(0, 0);
    lcd.print(F("Press A"));
    lcd.gotoXY(0, 1);
    lcd.print(leftValue);
    lcd.print(' ');
    lcd.print(rightValue);
  }
  lcd.clear();
}

void turnRight(int leftValue, int rightValue)
{
  int leftVelocity=constrain(100+(rightValue-leftValue)*50,0,400);
  int rightVelocity=constrain(100-(rightValue-leftValue)*50,0,400);
  motors.setSpeeds(leftVelocity,rightVelocity);
  turningLeft = false;
  turningRight = true;
}

void turnLeft(int leftValue, int rightValue)
{
  int leftVelocity=constrain(100-(leftValue-rightValue)*50,0,400);
  int rightVelocity=constrain(100+(leftValue-rightValue)*50,0,400);
  motors.setSpeeds(leftVelocity,rightVelocity);
  turningLeft = true;
  turningRight = false;
}

void stop()
{
  motors.setSpeeds(0, 0);
  turningLeft = false;
  turningRight = false;
}

void loop()
{
  // Read the front proximity sensor and gets its left value (the
  // amount of reflectance detected while using the left LEDs)
  // and right value.
  proxSensors.read();
  uint8_t leftValue = proxSensors.countsFrontWithLeftLeds();
  uint8_t rightValue = proxSensors.countsFrontWithRightLeds();

  // Determine if an object is visible or not.
  bool objectSeen = leftValue >= sensorThreshold || rightValue >= sensorThreshold;

  if (objectSeen)
  {
    // An object is visible, so we will start decelerating in
    // order to help the robot find the object without
    // overshooting or oscillating.
    turnSpeed -= deceleration;
  }
  else
  {
    // An object is not visible, so we will accelerate in order
    // to help find the object sooner.
    turnSpeed += acceleration;
  }

  // Constrain the turn speed so it is between turnSpeedMin and
  // turnSpeedMax.
  turnSpeed = constrain(turnSpeed, turnSpeedMin, turnSpeedMax);

  if (objectSeen)
  {
    // An object seen.
    ledYellow(1);
    lcd.clear();
    lcd.gotoXY(0, 1);
    lcd.print(F("OBS1"));

    lastTimeObjectSeen = millis();

    bool lastTurnRight = turnRight;
    if(!(leftValue>=6 && rightValue>=6)){
      if (leftValue < rightValue){
          lcd.clear();
          lcd.gotoXY(0, 0);
          lcd.print(F("OBSR1"));
          lcd.gotoXY(0, 1);
          lcd.print(leftValue);
          lcd.print(' ');
          lcd.print(rightValue);
          // The right value is greater, so the object is probably
          // closer to the robot's right LEDs, which means the robot
          // is not facing it directly.  Turn to the right to try to
          // make it more even.
          turnRight(leftValue,rightValue);
          senseDir = RIGHT;
      }
      else if (leftValue > rightValue){
          lcd.clear();
          lcd.gotoXY(0, 0);
          lcd.print(F("OBSL1"));
          lcd.gotoXY(0, 1);
          lcd.print(leftValue);
          lcd.print(' ');
          lcd.print(rightValue);
        // The left value is greater, so turn to the left.
        turnLeft(leftValue,rightValue);
        senseDir = LEFT;
      }else{
          lcd.clear();
          lcd.gotoXY(0, 0);
          lcd.print(F("OBSC1"));
          lcd.gotoXY(0, 1);
          lcd.print(leftValue);
          lcd.print(' ');
          lcd.print(rightValue);
          // The values are equal, so stop the motors.
          //stop();      
          motors.setSpeeds(100,100);
      }
    }else{
          lcd.clear();
          lcd.gotoXY(0, 0);
          lcd.print(F("OBN1"));
          lcd.gotoXY(0, 1);
          lcd.print(leftValue);
          lcd.print(' ');
          lcd.print(rightValue);
          //stop();
          motors.setSpeeds(00,00);    
    }
    
  }
  else
  {
    // No object is seen, so just keep turning in the direction
    // that we last sensed the object.
    ledYellow(0);
    lcd.clear();
    lcd.gotoXY(0, 0);
    lcd.print(F("OBUS1"));
    lcd.gotoXY(0, 1);
    lcd.print(leftValue);
    lcd.print(' ');
    lcd.print(rightValue);

    if (senseDir == RIGHT)
    {
      //turnRight();
      motors.setSpeeds(400, 100);
    }
    else
    {
      //turnLeft();
      motors.setSpeeds(100, 400);
    }
  
  }

  /*lcd.gotoXY(0, 0);
  lcd.print(leftValue);
  lcd.print(' ');
  lcd.print(rightValue);
  lcd.gotoXY(0, 1);
  lcd.print(turningRight ? 'R' : (turningLeft ? 'L' : ' '));
  lcd.print(' ');
  lcd.print(turnSpeed);
  lcd.print(' ');
  lcd.print(' ');*/
}
