#include "vex.h"
#include <cmath>

//Define motor ports, change as necessary for wiring
#define LEFT1PORT vex::PORT1
#define LEFT2PORT vex::PORT2
#define RIGHT1PORT vex::PORT3
#define RIGHT2PORT vex::PORT4
#define LEFTLIFTPORT vex::PORT5
#define RIGHTLIFTPORT vex::PORT6
#define FOURBARPORT vex::PORT7

using namespace vex;

vex::brain Brain;
vex::controller Controller;
vex::competition Competition = vex::competition();

vex::motor right1(RIGHT1PORT, false);
vex::motor right2(RIGHT2PORT, false);
vex::motor left1(LEFT1PORT, true);
vex::motor left2(LEFT2PORT, true);

vex::motor leftLift(LEFTLIFTPORT, false);
vex::motor rightLift(RIGHTLIFTPORT, true);

vex::motor fourBar(FOURBARPORT, false);


//State check variables used in later functions
bool pressing = false;
bool intakeReady = true;
int fourBarState = 0;


//Change these values as necessary, these are lift and 4-bar limits
//4 bar half is the hold but not open point on the 4-bar
const double liftLowerLimit = 100;
const double liftUpperLimit = 1000;
const double fourBarHalf = 1000;
const double fourBarFull = 2000;

//Standard voltage drive, can scale as needed with the first multiplier
void drive(double lpower, double rpower) {
  rpower *= 12.0 / 127;
  lpower *= 12.0 / 127;
  
  right1.spin(vex::directionType::fwd, rpower, vex::voltageUnits::volt);
  right2.spin(vex::directionType::fwd, rpower, vex::voltageUnits::volt);

  left1.spin(vex::directionType::rev, lpower, vex::voltageUnits::volt);
  left2.spin(vex::directionType::rev, lpower, vex::voltageUnits::volt);
}
//Moves the lift, make sure limits are set in correct positions
//Can later be added to macros 
void liftControl(){
   if (Controller.ButtonR2.pressing() && leftLift.value() <= liftLowerLimit + 7){
     leftLift.rotateTo(0.0, vex::rotationUnits::raw);
     rightLift.rotateTo(0.0, vex::rotationUnits::raw);
     leftLift.rotateTo(liftLowerLimit, vex::rotationUnits::raw);
     rightLift.rotateTo(liftLowerLimit, vex::rotationUnits::raw);
   }
   else if (Controller.ButtonR1.pressing() && leftLift.value() < liftUpperLimit) {
    leftLift.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
    rightLift.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
  } else if (Controller.ButtonR2.pressing() && leftLift.value() > liftLowerLimit) {
    leftLift.spin(vex::directionType::rev, 100, vex::velocityUnits::pct);
    rightLift.spin(vex::directionType::rev, 100, vex::velocityUnits::pct);
  } else {
    leftLift.stop();
    rightLift.stop();
  }
}
//Moves the 4-bar, make sure states are set in correct positions
//Can later be added to macros 
void fourBarControl(){
  if (!pressing){
    if (Controller.ButtonL1.pressing()) {
      pressing = true;
      if (fourBarState < 2){
        fourBarState++;
      }
  }
  else if (Controller.ButtonL2.pressing()) {
    pressing = true;
    if (fourBarState > 0){
      fourBarState--;
    }
   }
  }

  if (fourBarState == 0){
    fourBar.startRotateTo(0.0, vex::rotationUnits::raw);
  }
  else if (fourBarState == 1){
    fourBar.startRotateTo(fourBarHalf, vex::rotationUnits::raw);
  }
  else{
    fourBar.startRotateTo(fourBarFull, vex::rotationUnits::raw);
  }
  if (!(Controller.ButtonL1.pressing() || Controller.ButtonL2.pressing())){
    pressing = false;
  }
}

//As of now unused, but can be added for autonomous control
void wait(int msec) { vex::task::sleep(msec); }

//This is just an initialization function to clean up the main function
//It shouldn't need to be called more than once at the start of the program
void setupMotors(){
  left1.setStopping(vex::brakeType::coast);
  right1.setStopping(vex::brakeType::coast);
  left2.setStopping(vex::brakeType::coast);
  right2.setStopping(vex::brakeType::coast);
  leftLift.setStopping(vex::brakeType::hold);
  rightLift.setStopping(vex::brakeType::hold);
  fourBar.setStopping(vex::brakeType::hold);


  left1.resetRotation();
  left2.resetRotation();
  right1.resetRotation();
  right2.resetRotation();
  leftLift.resetRotation();
  rightLift.resetRotation();
  fourBar.resetRotation();
}
//Main control loop
void defaultUserControl() {
  setupMotors();
  while (1) {    
    drive(Controller.Axis3.value(), Controller.Axis2.value());
    liftControl();
    fourBarControl();
    vex::task::sleep(10);
  }
}
