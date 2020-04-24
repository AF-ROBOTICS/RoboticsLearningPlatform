/*! \file PCB_test.ino
 *  \brief The main file for the project.
 * 
 * This file contains the setup() and loop() functions, as well as a few functions related to
 * advanced robot control and pathfinding.
 */

/*
    Filename: PCB_test.ino
    Author: Capt Steven Beyer & 
    Created: 6 Mar 2019
    Description: Arduino Sketch that allows an encoder and IR-sensor equipped
      robot to navigate through the maze using the Trémaux algorithm. The IR
      sensors are used at the edge of every cell to determine the open exits,
      and to align the robot to the grid. The encoders are used to travel
      precisely the correct distance between cells.

    Required Files:
      Libraries : Encoder
      Packages  : none
      Files     : TB6612FNG.h, drive.h, IR.h, graph.h, util.h
*/

#include "TB6612FNG.h"
#include "drive.h"
#include "IR.h"
#include "graph.h"

const double base_speed = 0.1;
const double bonus_speed = 0.25;

bool returning = false;

// create an instance of the robot class from drive.h
Robot robot;

IRSensor lSensor(A0);
IRSensor mSensor(A1);
IRSensor rSensor(A2);

/*
 * \brief Drives the robot forward while also aligning to walls
 * 
 * This function uses the left and right IR sensors to keep the robot
 * in the center of a cell as it drives forward. Speed is in the range [-1.0, 1.0].
 */
void driveIRAdjust(double spd) {
    const double min_threshold = 30.0;
    const double max_threshold = 80.0;
    const double gain = 0.0015;
    
    const double lSensorDist = lSensor.getDistance();
    const double rSensorDist = rSensor.getDistance();
    if (min_threshold < lSensorDist && lSensorDist < max_threshold) {
      if (min_threshold < rSensorDist && rSensorDist < max_threshold) {
        const double error = lSensorDist - rSensorDist;
        robot.drivePID(spd - error * gain, spd + error * gain);
      }
      else {
        const double error = lSensorDist - 60.0;
        robot.drivePID(spd - error * gain, spd + error * gain);
      }
    }
    else if (min_threshold < rSensorDist && rSensorDist < max_threshold) {
      const double error = rSensor.getDistance() - 60.0;
      robot.drivePID(spd + error * gain, spd - error * gain);
    }
    else {
      robot.drivePID(spd, spd);
    }
}

void driveDistance(double mm) {
  long lStart = robot.readLEnc(), rStart = robot.readREnc();
  while (min(robot.readLEnc() - lStart, robot.readREnc() - rStart) < mm * edges_per_mm) {
    driveIRAdjust(bothRamp(lStart, lStart + mm * edges_per_mm, robot.readLEnc()) * bonus_speed + base_speed);
  }
  robot.drive(0, 0);
}

/*!
 * \brief Determines the next direction to be traveled in based on th Trémaux algorithm. s
 */
Direction nextMove() {
  int leftMarks     = maze[row][col].getMark(getRelativeHeading( 1));
  int forwardMarks  = maze[row][col].getMark(getRelativeHeading( 0));
  int rightMarks    = maze[row][col].getMark(getRelativeHeading(-1));

  leftMarks    += ((maze[row][col].exits & getRelativeHeading( 1)) == 0) * 2;
  forwardMarks += ((maze[row][col].exits & getRelativeHeading( 0)) == 0) * 2;
  rightMarks   += ((maze[row][col].exits & getRelativeHeading(-1)) == 0) * 2;

  if (leftMarks >= 2 && forwardMarks >= 2 && rightMarks >= 2) {
    return getRelativeHeading(2);
  }

  if (returning) {
    if (leftMarks != 1 && forwardMarks != 1 && rightMarks != 1) {
      return getRelativeHeading(2);
    }

    if (rightMarks == 1) {
      return getRelativeHeading(-1);
    }
    if (forwardMarks == 1) {
      return getRelativeHeading(0);
    }
    if (leftMarks == 1) {
      return getRelativeHeading(1);
    }
  }
  
  int minDir = -1;
  int minVal = rightMarks;

  if (forwardMarks <= minVal) {
    minDir = 0;
    minVal = forwardMarks;
  }

  if (leftMarks <= minVal) {
    minDir = 1;
    minVal = leftMarks;
  }

  return getRelativeHeading(minDir);
}

/*!
 * \brief Moves the robot into the next cell, updates the cell, and readies the robot for its next action
 * 
 * This function moves the robot from the center of one cell to the edge of the cell it is currently facing.
 * It then updates the new cell and decides which direction to go next, stopping or returning once the robot
 * reaches the center of the new cell.
 * 
 * \param lStart The starting value of the left encoder to be measured from
 * \param rStart The starting value of the right encoder to be measured from
 * \param needAcceleration 'true' if the function is called from a dead stop, 'false' if the function is called while the robot is already moving.
 * \return The new direction for the robot to travel in as determined by the pathfinding algorithm.
 */
Direction exploreEdge(long lStart, long rStart, bool needAcceleration) {
  // Initial acceleration phase, lasts until the robot reaches the cell edge
  while (min(robot.readLEnc() - lStart, robot.readREnc() - rStart) < cell_width / 2.0 * edges_per_mm) {
    if (needAcceleration) {
      const double progress = accelRamp(lStart, lStart + cell_width / 2.0 * edges_per_mm, robot.readLEnc());
      driveIRAdjust(progress * bonus_speed + base_speed);
    }
    else {
      driveIRAdjust(bonus_speed + base_speed);
    }
  }

  maze[row][col].addMark(getRelativeHeading(0));
  moveIn(dir);
  maze[row][col].addMark(getRelativeHeading(2));
  
  updateCell();
  Direction result = nextMove();
  

  // Use a wall to track deceleration/distance rather than the encoder
  if (!(maze[row][col].exits & getRelativeHeading(0))) {
    const double startDist = mSensor.getDistance();
    while (mSensor.getDistance() > 33.0) {
      const double progress = decelRamp(startDist, 33.0, mSensor.getDistance());
      driveIRAdjust(progress * bonus_speed + base_speed);
    }
  }
  else {
    while (min(robot.readLEnc() - lStart, robot.readREnc() - rStart) < cell_width * edges_per_mm) {
      if (result == getRelativeHeading(0)) {
        // Continue forward and exit when center of the cell reached. Note we do not decelerate or stop.
        driveIRAdjust(bonus_speed + base_speed);
      }
      else {
        // Begin decelerating in preparation for a turn
        const double progress = decelRamp(lStart + cell_width * edges_per_mm / 2.0, lStart + cell_width * edges_per_mm, robot.readLEnc());
        driveIRAdjust(progress * bonus_speed + base_speed);
      }
    }

  }

  // Stop if not continuing forward
  if (result != getRelativeHeading(0)) {
    robot.drive(0, 0);
  }

  return result;
}

/*!
 * \brief Stops the robot's movement and waits for the motor driver to be powered.
 */
void waitForPower() {
  while (analogRead(A5) < 500) {
    robot.drive(0, 0);
    delay(100); 
  }
}

/*
 * \brief Updates the available exits to a cell using the IR sensors.
 * 
 * Marks the cell as explored and updates its exits.
 * This function should be called when the robot is at the edge of a cell, not when it has already reached the center.
 */
void updateCell() {
  if (maze[row][col].explored) {
    return;
  }

  maze[row][col].exits |= getRelativeHeading(2);
  
  if (lSensor.getDistance() > 200.0) {
    maze[row][col].exits |= getRelativeHeading(1);
  }
  if (mSensor.getDistance() > 250.0) {
    maze[row][col].exits |= getRelativeHeading(0);
  }
  if (rSensor.getDistance() > 200.0) {
    maze[row][col].exits |= getRelativeHeading(-1);
  }

  maze[row][col].explored = true;
}

bool accel = true;
long lastLEnc = 0;
long lastREnc = 0;

void setup() {
  robot.init();
  Serial.begin(115200);
  delay(500);
  
  waitForPower();

  robot.calibrateGyro();
  robot.drive(0, 0);
  delay(100);

  lastLEnc = robot.readLEnc();
  lastREnc = robot.readREnc();
}

int turnCounter = 0;
int turnType = 0;

/*!
 * Based on the next direction to go in and the current heading of the robot,
 * determines what kind of turn must be taken and updates a counter.
 * 
 * \return The number of consecutive turns in the same direction taken so far
 */
int countConsecutiveTurns(Direction nextDir) {
  if (nextDir == getRelativeHeading(turnType) && turnType != 0) {
    // If going in the next direction would require the same turn we've been doing, increment turn counter
    ++turnCounter;
  }
  else {
    if (nextDir == getRelativeHeading(-1)) {
      // Start counting right turns
      turnType = -1;
      turnCounter = 1;
    }
    else if (result == getRelativeHeading(1)) {
      // Start counting left turns
      turnType = 1;
      turnCounter = 1;
    }
    else {
      // Reset the counter because we've been doing neither kind of turn
      turnType = 0;
      turnCounter = 0;
    }
  }
  
  return turnCounter;
}

void loop() {
  waitForPower();

  Direction result = exploreEdge(lastLEnc, lastREnc, accel);

  returning |= countConsecutiveTurns(result) >= 3;
  
  if (result == getRelativeHeading(0)) {
    lastLEnc += cell_width * edges_per_mm;
    lastREnc += cell_width * edges_per_mm;
    accel = false;
  }
  else {
    if (result == getRelativeHeading(1)) {
      dir = getRelativeHeading(1);
    }
    else if (result == getRelativeHeading(-1)) {
      dir = getRelativeHeading(-1);
    }
    else {
      dir = getRelativeHeading(2);
    }

    robot.turnAbsoluteAngle(result);
    
    lastLEnc = robot.readLEnc();
    lastREnc = robot.readREnc();
    accel = true;
  }
}
