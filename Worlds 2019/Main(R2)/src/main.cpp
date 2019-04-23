
#include "..\..\Main()\include\Common\main.h"

void auton() {
  resetGyro();
  vex::task c(catapultControl);

  // spin intake inwards
  intake.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);

  // drive to cap to pick up ball
  encoderDrive(-2550, 50);
  wait(800);

  // drive back
  encoderDrive(1000, 50);

  intake.stop();

  // turn right
  gyroTurn(60);

  //drive to flip cap
  //intake.spin(vex::directionType::rev, 100, vex::velocityUnits::pct);
  //encoderDrive(-1500, 50);

  stopAll();
}

int main() {
  Competition.autonomous(auton);
  Competition.drivercontrol(defaultUserControl);
}
