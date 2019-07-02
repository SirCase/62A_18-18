
#include "..\..\Main()\include\Common\main.h"

void auton() {
  resetGyro();
  vex::task c(catapultControl);

  // spin intake inwards
  intake.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);

  // drive to cap to pick up ball
  encoderDrive(-2420, 69);
  wait(800);

  // drive back
  encoderDrive(1800, 69);

  intake.stop();

  // turn right
  gyroTurn(90);

  // turnToFlag();

  // fire catapult
  firing = true;
  wait(1000);
    
    //turn left a bit
    gyroTurn(-35);
    
    //drive forward a bit
    encoderDrive(1590, 80);
    
    //turn right to face flag
    gyroTurn(34.2);

  // drive to low flag
  encoderDrive(2400, 80);
    wait(100);
  //drive back
  encoderDrive(-1710, 80);

  //turn left to face cap
  gyroTurn(-90);

  //drive to flip cap
  intake.spin(vex::directionType::rev, 100, vex::velocityUnits::pct);
  encoderDrive(-1200, 85);

  stopAll();
  
}

int main() {
  Competition.autonomous(auton);
  Competition.drivercontrol(defaultUserControl);
}
