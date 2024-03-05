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

const word MAX_BRIGHTNESS = 1024; // In some devices is 255. Please see the documentation.
const unsigned long DEFAULT_BRIGHTEN_TIME_MS = 1 * 1000; // Interval for going from 0 the MaxBrightness. Fast up.
const unsigned long DEFAULT_FADE_TIME_MS = 30 * 1000; // Interval for going from MaxBrightness the 0. Slow down.
const unsigned long DEFAULT_RUNNING_DURATION_MS = 1 /*min*/ * 60 /*sec*/ * 1000; // A minute will continue the light with MaxBrightness.

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

	void setBrightenTime(unsigned long milliSecond);
	unsigned long getBrightenTime();

	void setFadeTime(unsigned long milliSecond);
	unsigned long getFadeTime();

	void setRunningDuration(unsigned long milliSecond);
	unsigned long getRunningDuration();
private:
	void processBrightness(bool lightOn);
	void setNextOperationTime(unsigned long additionalTime);
	bool isItTimeForNextOperation();
};

#endif	// FluentLight_H
