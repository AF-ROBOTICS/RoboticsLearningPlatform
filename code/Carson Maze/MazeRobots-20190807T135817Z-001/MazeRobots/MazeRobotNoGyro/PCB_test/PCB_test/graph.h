/*! \file graph.h
 *  \brief Contains the structure of the maze and functions to alter the robot's relation to the grid.
 */

#include "util.h"

constexpr const float wall_thickness = 12.0;
constexpr const float cell_width = 167.0 + wall_thickness;

/*!
 * The Cell class represents one grid square of the maze.
 * It stores the cell's available exits, the marks on the cell's edges
 * used by the pathfinding algorithm, and whether or not the cell has
 * been explored or not. Unexplored cells will have every exit marked as
 * blocked (all zeros).
 */
struct Cell {

  /// A bitfield representing which exits are open, based on bitwise-ORing Direction values.
  /// When a bit is zero, the corresponding direction is blocked, when one, it is open.
  Direction exits;

  /// Stores the number of marks made on each edge: 'East', 'North', 'West', and 'South' respectively.
  /// This could be stored in a single byte as the maximum number of marks on a cell is only 2, but
  /// the interface is easier to use in array form.
  byte marks[4];

  /// All cells start with explored as 'false', and are updated to 'true' when
  /// the robot has reached it and found the available exits.
  bool explored;

  /*! Adds a mark on the edge of the cell in a given Direction */
  void addMark(Direction d) {
    ++marks[__builtin_ctz(d) % 4];
  }

  /*! Returns the number of marks on the edge of the cell in a given Direction */
  int getMark(Direction d) {
    return marks[__builtin_ctz(d) % 4];
  }
};

// The maze is represented as a grid of cells, each cell having two properties:
// 1. Whether or not it is explored
// 2. The open entrances/exits to the cell
Cell maze[10][10] = { Cell{ 0, { 0 }, false } };

// The robot starts in the south-western corner, facing East
int row = 0, col = 0;
Direction dir = Direction::East;

// Updates the robot's position, assuming it drives one cell forward in the direction 'dir'
void moveIn(Direction dir) {
  switch (dir) {
  case Direction::East:
    ++col;
    break;
  case Direction::North:
    ++row;
    break;
  case Direction::West:
    --col;
    break;
  case Direction::South:
    --row;
    break;
  default:
    break;
  }
}

/*!
 * This gets the cardinal direction relative to the robot's current heading
 * offset =  0 => the direction the robot is facing
 * offset =  1 => the direction to the robot's left
 * offset = -1 => the direction to the robot's right
 * offset =  2 => the direction behind the robot
 * etc.
 */
Direction getRelativeHeading(int offset) {
  // The series of modulos is to convert the offset to a value in the range [0, 3]
  offset = ((offset % 4) + 4) % 4;

  return 1 << (((__builtin_ctz(dir)) + offset) % 4);
}
