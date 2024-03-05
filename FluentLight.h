/*!
 * @file FluentLight.h
 *
 * This is a library to control LED light fluently
 *
 * This is an Arduino library which allows you to manage LED light
 *
 * Written by Plamen Kovandzhiev kovandjiev@gmail.com
 * It is licensed under the MIT License, see LICENSE.txt.
 */
 
#ifndef FluentLight_H
#define FluentLight_H

#include "Arduino.h"

const word MAX_BRIGHTNESS = 255;
const unsigned long LIGHT_ON_DURATION_MS = 1000; // Interval for going from 0 the MaxBrightness. Fast up.
const unsigned long LIGHT_OFF_DURATION_MS = 30 * 1000; // Interval for going from MaxBrightness the 0. Slow down.
const unsigned long ON_DURATION_MS = 1/*min*/ * 60 /*sec*/ * 1000; // A minute will continue the light with MaxBrightness.

class FluentLight {
public:
	/*!
	 * @brief Crate an instance of FluentLight
	 * @param pin Light control pin
	 */	
	FluentLight(byte pin);

	/*!
	 * @brief Start FluentLight
	 */
	void begin();

	/*!
	 * @brief Process FluentLight logic. This method should be added in loop()
	 */
	void process();
	
	void on();

	void setMaxBrightness(word bright);

	word getMaxBrightness();

	void setLightOnDuration(unsigned long milliSecond);

	unsigned long getLightOnDuration();

	void setLightOffDuration(unsigned long milliSecond);

	unsigned long getLightOffDuration();

	void setOnDuration(unsigned long milliSecond);

	unsigned long getOnDuration();
private:
	void processBrightness(bool startLight);
};

#endif	// FluentLight_H
