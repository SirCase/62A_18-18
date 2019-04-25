
#include "..\..\Main()\include\Common\main.h"

void auton() {
  resetGyro();
  vex::task c(catapultControl);

  // spin intake inwards
  intake.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);

  // drive to cap to pick up ball
  encoderDrive(-2550, 50);
  wait(900);

  // drive back
  encoderDrive(1000, 50);
    wait(50);

  intake.stop();

  // turn right
  gyroTurn(53);
    wait(50);

  //drive to flip cap
  intake.spin(vex::directionType::rev, 100, vex::velocityUnits::pct);
  encoderDrive(-2200, 65);
    wait(50);
    
  //drive forward to get in line with platform
    encoderDrive(1069, 65);
    wait(50);
    
    //turn left to face platform
    gyroTurn(-147);
    wait(50);
    
    timedDrive(1420, -95);
    setHolding();
  stopAll();
}

int main() {
  Competition.autonomous(auton);
  Competition.drivercontrol(defaultUserControl);
}
