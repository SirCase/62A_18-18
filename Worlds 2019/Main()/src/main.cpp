
#include "Common/main.h"
void auton() {
  resetGyro();
  gyroTurn(90);
}

int main() {
  Competition.drivercontrol(defaultUserControl);
  Competition.autonomous(auton);
}
