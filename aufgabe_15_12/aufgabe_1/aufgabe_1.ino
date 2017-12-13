#include <Wire.h>
#include <Zumo32U4.h>


Zumo32U4LCD lcd;
Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors;
const int speed=200;
Zumo32U4Encoders encoders;
const char encoderErrorLeft[] PROGMEM = "!<c2";
const char encoderErrorRight[] PROGMEM = "!<e2";
//const int one_meter=10400;//100:1
const int one_meter=7800;//75:1
int left_count=0;
int right_count=0;


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
    	if(max(countsLeft-left_count,countsRight-right_count)>= one_meter){
			motors.setSpeeds(0,0);
			lcd.clear();
	    	lcd.print(F("Press A"));
	    	lcd.gotoXY(0, 1);
	    	lcd.print(max(countsRight,countsLeft));
    		left_count=countsLeft;
    		right_count=countsRight;
    		buttonA.waitForButton();
    		motors.setSpeeds(speed, speed);
    	}
    }

}
