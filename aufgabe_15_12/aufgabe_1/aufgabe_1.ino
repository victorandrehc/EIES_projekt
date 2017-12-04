#include <Wire.h>
#include <Zumo32U4.h>


Zumo32U4LCD lcd;
Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors;
const int speed=200;
Zumo32U4Encoders encoders;
const char encoderErrorLeft[] PROGMEM = "!<c2";
const char encoderErrorRight[] PROGMEM = "!<e2";
const int one_meter=4000;


void setup() {
  // put your setup code here, to run once:
	lcd.clear();
	lcd.print(F("Press A"));
	buttonA.waitForButton();
	lcd.clear();
	lcd.gotoXY(0, 1);
	//motors.setLeftSpeed(speed);
	//motors.setRightSpeed(speed);
	motors.setSpeeds(speed, speed);

}

void loop() {
  // put your main code here, to run repeatedly:
	int16_t countsLeft = encoders.getCountsLeft();
	int16_t countsRight = encoders.getCountsRight();
	bool errorLeft = encoders.checkErrorLeft();
    bool errorRight = encoders.checkErrorRight();
    if(!errorLeft && !errorRight){
    	if(countsLeft>= one_meter || countsRight>= one_meter){
			motors.setSpeeds(0,0);
			lcd.clear();
	    	lcd.print(countsLeft);
	    	lcd.gotoXY(0, 1);
	    	lcd.print(countsRight);
    		while(1);
    	}
    }

}
