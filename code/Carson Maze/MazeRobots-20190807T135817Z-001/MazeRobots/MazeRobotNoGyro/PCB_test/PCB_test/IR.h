/*! \file IR.h
 *  \brief Contains the IRSensor class, for making IR sensor use easier.
 */

constexpr const size_t IR_samples = 100;

/*!
 * \brief Allows for accessing the distance measurements of an IR sensor.
 * 
 * This sensor class averages samples from the IR sensor to get a smooth,
 * consistent measurement of distance, measured in millimeters.
 */
class IRSensor {
public:
  IRSensor(int inputPin) :
    pin(inputPin)
  {
    
  }

  /*!
   * \brief Returns the current distance in mm
   */
  double getDistance() {
    double sum = 0;
    for (int i = 0; i < IR_samples; ++i) {
      sum += analogRead(pin) * 5.0 / 1024.0;
    }
    sum /= IR_samples;
    
    return 128.191 / (sum + 0.310) - 45.935;
  }

private:
  int pin;
};
