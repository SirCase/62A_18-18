
#include "..\..\Main()\include\Common\main.h"

void auton() {
     resetGyro();
  vex::task c(catapultControl);

  // spin intake inwards
  intake.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);

  // drive to cap to pick up ball
  encoderDrive(-2550, 69);
  wait(800);

  // drive back
  encoderDrive(2875, 69);

  intake.stop();

  // turn left
  gyroTurn(-90);

  // turnToFlag();

  // fire catapult
  firing = true;
  wait(1000);

  // turn right a little bit
  // turnToFlag();
  gyroTurn(2.1);

  // drive to low flag
  encoderDrive(2800, 80);
  timedDrive(600, 72);

  //drive back
  encoderDrive(-1690, 75);

  //turn right to face cap
  gyroTurn(90);

  //drive to flip cap
  intake.spin(vex::directionType::rev, 100, vex::velocityUnits::pct);
  encoderDrive(-1200, 69);

  stopAll();
}


int main() {
  Competition.autonomous(auton);
  Competition.drivercontrol(defaultUserControl);
}
