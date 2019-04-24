
#include "vex.h"
#include "vision.h"
#include <cmath>

using namespace vex;

//////////////////////////// robot config /////////////////////////////

vex::brain Brain;
vex::controller Controller;
vex::competition Competition = vex::competition();

vex::motor right1(vex::PORT1, false);
vex::motor right2(vex::PORT2, false);
vex::motor right3(vex::PORT3, true);
vex::motor left1(vex::PORT9, true);
vex::motor left2(vex::PORT10, true);
vex::motor left3(vex::PORT8, false);

vex::motor catapult = vex::motor(vex::PORT11, true);
vex::motor intake = vex::motor(vex::PORT12, true);

vex::limit catapultLimit = vex::limit(Brain.ThreeWirePort.E);

vex::gyro Gyro = vex::gyro(Brain.ThreeWirePort.H);

///////////////////////////////////////////////////////////////////////

bool firing = false;
bool pressed = false;

int topTarget = 23; // 195 using mid/top? flag as target
int midTarget = 95; // 277 using mid //was 104
int xTarget = 78;
int xTarget2 = 157;

/////////////////////////////////// user control helper methods
///////////////////////////////////////////

void drive(double lpower, double rpower) {
  rpower *= 12.0 / 127;
  lpower *= 12.0 / 127;

  right1.spin(vex::directionType::fwd, rpower, vex::voltageUnits::volt);
  right2.spin(vex::directionType::fwd, rpower, vex::voltageUnits::volt);
  right3.spin(vex::directionType::fwd, rpower, vex::voltageUnits::volt);

  left1.spin(vex::directionType::fwd, lpower, vex::voltageUnits::volt);
  left2.spin(vex::directionType::fwd, lpower, vex::voltageUnits::volt);
  left3.spin(vex::directionType::fwd, lpower, vex::voltageUnits::volt);
}

void drive2(double lpower, double rpower) {
  right1.spin(vex::directionType::fwd, rpower, vex::velocityUnits::pct);
  right2.spin(vex::directionType::fwd, rpower, vex::velocityUnits::pct);
  right3.spin(vex::directionType::fwd, rpower, vex::velocityUnits::pct);

  left1.spin(vex::directionType::fwd, lpower, vex::velocityUnits::pct);
  left2.spin(vex::directionType::fwd, lpower, vex::velocityUnits::pct);
  left3.spin(vex::directionType::fwd, lpower, vex::velocityUnits::pct);
}

void intakeControl() {
  if (Controller.ButtonL1.pressing()) {
    intake.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
  } else if (Controller.ButtonL2.pressing()) {
    intake.spin(vex::directionType::fwd, -100, vex::velocityUnits::pct);
  } else {
    intake.stop();
  }
}

void catapultTest() {
  catapult.spin(vex::directionType::fwd, Controller.ButtonX.pressing() ? 10 : 0,
                vex::velocityUnits::pct);
}

void firingControl() {
  if (!firing) {
    firing = true;
  }
}

int catapultControl() {
  while (1) {
    Controller.ButtonR1.pressed(firingControl);

    if (firing && !catapultLimit.pressing()) {
      firing = false;
    }

    if (!catapultLimit.pressing()) {
      pressed = false;
    }

    // moves catapult until it hits the switch or if it is firing
    if (firing || !catapultLimit.pressing()) {
      catapult.spin(vex::directionType::fwd, 70, vex::velocityUnits::pct);
    }

    // when the catapult hits the switch it will move a bit more allowing for
    // the ratchet to catch
    if (!pressed && catapultLimit.pressing()) {
      catapult.spin(vex::directionType::fwd, 10, vex::velocityUnits::pct);
      vex::task::sleep(100); // 100 for catching on 2nd tooth
      catapult.stop();
      pressed = true;
    }
    vex::task::sleep(10);
  }
  return (0);
}

//////////////////////////////////////////// auton helper methods
////////////////////////////////////////////////

void encoderDrive(double ticks, double pct) {
  left1.startRotateFor(ticks, vex::rotationUnits::raw, pct,
                       vex::velocityUnits::pct);
  right1.startRotateFor(ticks, vex::rotationUnits::raw, pct,
                        vex::velocityUnits::pct);
  left2.startRotateFor(ticks, vex::rotationUnits::raw, pct,
                       vex::velocityUnits::pct);
  right2.startRotateFor(ticks, vex::rotationUnits::raw, pct,
                        vex::velocityUnits::pct);
  left3.startRotateFor(ticks, vex::rotationUnits::raw, pct,
                       vex::velocityUnits::pct);
  right3.rotateFor(ticks, vex::rotationUnits::raw, pct,
                   vex::velocityUnits::pct);
}

void stopAll() {
  left1.stop();
  left2.stop();
  left3.stop();
  right1.stop();
  right2.stop();
  right3.stop();
  intake.stop();
}

void wait(int msec) { vex::task::sleep(msec); }

void timedDrive(int msec, double speed) {
  drive(speed, speed);
  wait(msec);
  stopAll();
}

void turn(double speed) { drive(speed, -speed); }

void turn2(double speed) { drive2(speed, -speed); }

void brakeAll() {
  left1.setStopping(vex::brakeType::brake);
  right1.setStopping(vex::brakeType::brake);
  left2.setStopping(vex::brakeType::brake);
  right2.setStopping(vex::brakeType::brake);
  left3.setStopping(vex::brakeType::brake);
  right3.setStopping(vex::brakeType::brake);
}

/////////////////////////////////////// gyroscope///////////////////////////////////////////////

void gyroTurn(double target) {
  target = Gyro.value(vex::analogUnits::range12bit) + target * 10 + target * 12 / 90;
  double error = target - Gyro.value(vex::analogUnits::range12bit);
  double totalError = 0;

  double kp = 0.06;
  double ki = 0.00;

  while (std::abs(error) > 3) {
    vex::task::sleep(10);
    error = target - Gyro.value(vex::analogUnits::range12bit);
    totalError += error;
    if (std::abs(error) > 40) {
      totalError = 0;
    }
    turn2(error * kp + totalError * ki);
    
  }
  brakeAll();
  Controller.Screen.print(error);
}

void resetGyro() {
  Gyro.startCalibration();
  wait(1300);
}

///////////////////////////////////////// vision sensor
//////////////////////////////////////////////////////

void turnToFlag(double target) {
  int value = 0;
  int error = 10;
  int totalError = 0;
  double P = -0.333;
  double I = -0.224;

  while (std::abs(error) > 5) {
    value = getLowFlag().centerY;
    if (value != 0) {
      error = target - value;
      totalError = totalError + error;
      if (std::abs(error) > 50) {
        totalError = 0;
      }
      turn(error * P + totalError * I);
      Controller.Screen.clearScreen();
      Controller.Screen.print(error);
    }
  }
}

void turnToFlag() { turnToFlag(xTarget); }

void turnToFlag2() { turnToFlag(xTarget2); }

void driveToFlag(int whichFlag) { // 1 = mid flag, 2 = top flag
  int value = 0;
  int error = 10;
  int totalError = 0;
  int target = (whichFlag == 1 ? midTarget : topTarget);
  double P = 1.7;
  double I = 0.08;

  while (std::abs(error) > 4) {
    value = getLowFlag().centerX;
    if (value != 0) {
      error = target - value;
      totalError = totalError + error;
      if (std::abs(error) > 15) {
        totalError = 0;
      }
      drive(error * P + totalError * I, error * P + totalError * I);
      Controller.Screen.clearScreen();
      Controller.Screen.print(error);
    }
  }
}

void aimAtFlag(int whichFlag) { // 1 = mid flag, 2 = top flag
  int yTarget = (whichFlag == 1 ? midTarget : topTarget);
  int xValue = 0;
  int yValue = 0;
  int xError = 10;
  int yError = 10;

  turnToFlag();
  driveToFlag(whichFlag);
  yValue = getLowFlag().centerX;
  xValue = getLowFlag().centerY;
  xError = xTarget - xValue;
  yError = yTarget - yValue;

  if (std::abs(xError) > 5 || std::abs(yError) > 5) {
    aimAtFlag(whichFlag);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void defaultUserControl() {
  vex::task c(catapultControl);
  resetGyro();
  gyroTurn(90);
  while (1) {
   
    drive(Controller.Axis3.value(), Controller.Axis2.value());
    intakeControl();
    vex::task::sleep(10);
  }
}
