/*vex-vision-config:begin*/
#include "v5.h"
#include "v5_vcs.h"
#include <cmath>

vex::vision::signature SIG_1 = vex::vision::signature (1, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_2 = vex::vision::signature (2, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_3 = vex::vision::signature (3, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_4 = vex::vision::signature (4, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_5 = vex::vision::signature (5, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_6 = vex::vision::signature (6, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_7 = vex::vision::signature (7, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision Vision = vex::vision (vex::PORT13, 50, SIG_1, SIG_2, SIG_3, SIG_4, SIG_5, SIG_6, SIG_7);
/*vex-vision-config:end*/

vex::vision::object getTopFlag() { //inconsistent
    Vision.takeSnapshot(SIG_1, 3);
    return Vision.objects[2];
}

vex::vision::object getMidFlag() { //inconsistent
    Vision.takeSnapshot(SIG_1, 2);
    return Vision.objects[1];
}

vex::vision::object getLowFlag() { //consistent
    Vision.takeSnapshot(SIG_1);
    return Vision.largestObject;
}



