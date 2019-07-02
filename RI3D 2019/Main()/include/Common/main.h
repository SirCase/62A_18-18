
#include "vex.h"
#include <cmath>
#define LEFT1PORT vex::PORT1
#define LEFT2PORT vex::PORT2
#define RIGHT1PORT vex::PORT3
#define RIGHT2PORT vex::PORT4
#define LEFTLIFTPORT vex::PORT5
#define RIGHTLIFTPORT vex::PORT6
#define INTAKEPORT vex::PORT7
#define FOURBARPORT vex::PORT8
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

vex::motor intake(INTAKEPORT, false);
vex::motor fourBar(FOURBARPORT, false);
/*
int intakeState = 0;
bool pressing = false;
*/
const int liftLimit = 1000;
void drive(double lpower, double rpower) {
  rpower *= 12.0 / 127;
  lpower *= 12.0 / 127;
  
  right1.spin(vex::directionType::fwd, rpower, vex::voltageUnits::volt);
  right2.spin(vex::directionType::fwd, rpower, vex::voltageUnits::volt);

  left1.spin(vex::directionType::fwd, lpower, vex::voltageUnits::volt);
  left2.spin(vex::directionType::fwd, lpower, vex::voltageUnits::volt);
}

void liftControl(){
   if (Controller.ButtonR1.pressing()) {
    leftLift.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
    rightLift.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
  } else if (Controller.ButtonR2.pressing() && motor()) {
    leftLift.spin(vex::directionType::fwd, -100, vex::velocityUnits::pct);
    rightLift.spin(vex::directionType::fwd, -100, vex::velocityUnits::pct);
  } else {
    leftLift.stop();
    rightLift.stop();
  }
}

/*void intakeControl(){
  if (!pressing){
    if (Controller.ButtonL1.pressing()) {
      pressing = true;
      if (intakeState == 1){
         intakeState = 0;
      }
    else{
      intakeState = 1;
    }
  }
  else if (Controller.ButtonL2.pressing()) {
    pressing = true;
    if (intakeState == -1){
      intakeState = 0;
    }
    else{
      intakeState = -1;
    }
   }
  }
  if (!(Controller.ButtonL1.pressing() || Controller.ButtonL2.pressing())){
    pressing = false;
  }
  if (intakeState == 1){
    intake.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
  }
  else if (intakeState == -1){
    intake.spin(vex::directionType::rev, 100, vex::velocityUnits::pct);
  }
  else{
    intake.stop();
  }
}*/
void wait(int msec) { vex::task::sleep(msec); }

void defaultUserControl() {
  left1.setStopping(vex::brakeType::coast);
  right1.setStopping(vex::brakeType::coast);
  left2.setStopping(vex::brakeType::coast);
  right2.setStopping(vex::brakeType::coast);
  while (1) {
    drive(Controller.Axis3.value(), Controller.Axis2.value());
    liftControl();
    //intakeControl();
    vex::task::sleep(10);
  }
}
