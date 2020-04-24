/*! \file util.h
 *  \brief Contains a variety of miscellaneous math functions to allow for cleaner code.
 */

#pragma once

/*!
 * \brief A type-independent way to get the sign of a number. Returns -1, 0, or 1.
 */
template < typename T >
T signum(const T& value) {
  return (value > T{0}) - (value < T{0});
}

/*!
 * \brief Represents all directions in the maze.
 * 
 * These directions do NOT actually correspond to real cardinal directions.
 * They are all relative to the robot's starting position,
 * which is in the South-Western corner, facing East. 
 * 
 */
enum Direction : byte {
  East =    (1 << 0),
  North =   (1 << 1),
  West =    (1 << 2),
  South =   (1 << 3)
};

/*! \brief Returns a value between [0.0, 1.0], based on how far between 'start' and 'target' the input 'current' is. */
double accelRamp(double start, double target, double current) {
  if (target == start) {
    return 0.5;
  }

  return (current - start) / (target - start);
}

/*! \brief Like accelRamp, but reversed (i.e. starts at 1.0 and decreases to 0.0). */
double decelRamp(double start, double target, double current) {
  return 1.0 - accelRamp(start, target, current);
}

/*! \brief Combines accelRamp and decelRamp, switching from one to the other halfway between start and target */
double bothRamp(double start, double target, double current) {
  if (accelRamp(start, target, current) <= 0.5) {
    return accelRamp(start, (start + target) / 2.0, current);
  } else {
    return decelRamp((start + target) / 2.0, target, current);
  }
}

/*!
 * \brief Returns the signed difference between two angles.
 * 
 * Returns the shortest angle change required to get from start to target.
 * For example, if start is 20.0, and target is 350.0, the function returns -30.0,
 * as that is the least angle change required to get from the starting angle to the
 * target angle.
 */
double getAngleDiff(double start, double target) {
  double temp = target - start;
  temp = (temp + 180.0);
  temp -= 360.0 * floor(temp / 360.0);
  temp -= 180.0;
  return temp;
}
