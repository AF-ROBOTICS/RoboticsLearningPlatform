/**
 * @file      Classifier.h
 * @brief     Provide functions that are used to Classify states of walls
 * @details   Uses wall sensors to classify state of robot
 * @version   V1.0
 * @author    Beyer
 * @copyright 
 * @warning   AS-IS
 * @note      
 * @date      November 11, 2019
 ******************************************************************************/

#include <stdint.h> 

enum scenario {
    Error = 0,
    LeftTooClose = 1,
    RightTooClose = 2,
    CenterTooClose = 4,
    Straight = 8,
    LeftTurn = 9,
    RightTurn = 10,
    TeeJoint = 11,
    LeftJoint = 12,
    RightJoint = 13,
    CrossRoad = 14,
    Blocked = 15
};
typedef enum scenario scenario_t;

#define SIDEMAX 475    // largest side distance to wall in mm
#define SIDEMIN 75    // smallest side distance to wall in mm
#define CENTEROPEN 500 // distance to wall between open/blocked
#define CENTERMIN 75  // min distance to wall in the front
#define IRMIN 50    // min possible reading of IR sensor
#define IRMAX 800   // max possible reading of IR sensor

/* Classify
* Utilize three distance values from the left, center, and right
* distance sensors to determine and classify the situation into one
* of many scenarios (enumerated by scenario)
*
* Input
*   int32_t Left: distance measured by left sensor
*   int32_t Center: distance measured by center sensor
*   int32_t Right: distance measured by right sensor
*
* Output
*   scenario_t: value representing the scenario the robot
*       currently detects (e.g. RightTurn, LeftTurn, etc.)
*/
scenario_t Classify(uint16_t Left, uint16_t Center, uint16_t Right){

  scenario_t result=Error;

  // Error condition, sensor has reading less than 50 mm or greater than 800 mm
  if (Left > IRMAX || Left < IRMIN || Right > IRMAX || Right < IRMIN || Center > IRMAX || Center < IRMIN){
      return result;
  }

  // Danger conditions - if any sensor is too close as defined by SIDEMIN and CENTERMIN
  if(Left < SIDEMIN){
      result = LeftTooClose;
  }
  if(Right < SIDEMIN){
      result = RightTooClose;
  }
  if(Center < CENTERMIN){
      result = CenterTooClose;
  }
  if (result > 0){
      return result;
  }

    if(result > 0){
        return result;  //return danger codes of any above combination
    }

    // No error or danger condition
    // Center not open
    if(Center < CENTEROPEN){
        if(Left < SIDEMAX && Right < SIDEMAX){ // left & right are closed
            result = Blocked;

        }else if (Left < SIDEMAX){ //Right is open
            result = RightTurn;

        }else if (Right < SIDEMAX){ //left is open
            result = LeftTurn;

        }else{ //both left and right open
            result = TeeJoint;
        }

    //Center is open
    }else{
        if(Left < SIDEMAX && Right < SIDEMAX){ // left & right are closed
            result = Straight;

        }else if (Left < SIDEMAX){ //right is open
            result = RightJoint;

        }else if(Right < SIDEMAX){ //left is open
            result = LeftJoint;

        }else{
            result = CrossRoad;
        }
    }

    return result;
}
