#include "vex.h"
#include "robot-config.h"
#include <cmath>
bool firing = false;
bool pressed = false;
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
/////////////////////////////////// user control helper methods ////////////////////////////////////////

void voltageDrive(double lpower, double rpower) {
    rpower *= 12.0/127;
    lpower *= 12.0/127;
    
    right1.spin(vex::directionType::fwd, rpower, vex::voltageUnits::volt);
    right2.spin(vex::directionType::fwd, rpower, vex::voltageUnits::volt);
    right3.spin(vex::directionType::fwd, rpower, vex::voltageUnits::volt);
   
    left1.spin(vex::directionType::fwd, lpower, vex::voltageUnits::volt);
    left2.spin(vex::directionType::fwd, lpower, vex::voltageUnits::volt);
    left3.spin(vex::directionType::fwd, lpower, vex::voltageUnits::volt);
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
    catapult.spin(vex::directionType::fwd, Controller.ButtonX.pressing() ? 10 : 0, vex::velocityUnits::pct);
}

void firingControl() {
    if (!firing) {
        firing = true;
    } 
}

int catapultControl() {
    while(1) {
        Controller.ButtonR1.pressed(firingControl);
        
        if (firing && !catapultLimit.pressing()) {
            firing = false;
        }
        
        if (!catapultLimit.pressing()) {
            pressed = false;
        }
           
        //moves catapult until it hits the switch or if it is firing
        if (firing || !catapultLimit.pressing()) {
           catapult.spin(vex::directionType::fwd, 70, vex::velocityUnits::pct);
        }
        
        //when the catapult hits the switch it will move a bit more allowing for the ratchet to catch
        if (!pressed && catapultLimit.pressing()) {
            catapult.spin(vex::directionType::fwd, 10, vex::velocityUnits::pct);
            vex::task::sleep(100); //100 for catching on 2nd tooth
            catapult.stop();
            pressed = true;
        }      
    }
    return (0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void usercontrol() {
    vex::task c(catapultControl);
    while(1) { 
        voltageDrive(Controller.Axis3.value(), Controller.Axis2.value());
        intakeControl();
    }
}

int main() {
    Competition.drivercontrol(usercontrol); 
}