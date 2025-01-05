/*!
 * @file FluentLight.h
 *
 * This is a library to control LED light fluently
 *
 * This is an Arduino library which allows you to manage LED light
 *
 * Written by Plamen Kovandzhiev kovandjiev@gmail.com
 * It is licensed under the MIT License, see LICENSE.txt.
 * 
 * Version: 1.0.1
 * Last changed: 01.01.2025
 * Added set pwmRange and pwmFrequency in constructor
 * During On setMaxBrightness dynamically
*/
 
#ifndef FluentLight_H
#define FluentLight_H

#include "Arduino.h"

const int MAX_BRIGHTNESS = 255; // PWM range. Default is 255. Please see the documentation.
const unsigned long DEFAULT_BRIGHTEN_TIME_MS = 1 * 1000; // Interval for going from 0 the MaxBrightness. Fast up.
const unsigned long DEFAULT_FADE_TIME_MS = 30 * 1000; // Interval for going from MaxBrightness the 0. Slow down.
const unsigned long DEFAULT_RUNNING_DURATION_MS = 1 /*min*/ * 60 /*sec*/ * 1000; // A minute will continue the light with MaxBrightness.
const int DEFAULT_PWM_RANGE = 255; // Default PWM range is 255
const int DEFAULT_PWM_FREQUENCY = 1000; // Default PWM frequency is 1 kHz

class FluentLight {
public:
	enum class State { Off, Brighten, On, Fade };

	typedef void (*onStateChangedEvent_t) (State state);

	/// @brief Crate an instance of FluentLight
	/// @param ledPin Light control pin
	/// @param pwmRange PWM range. Default is 255
	/// @param pwmFreq PWM frequency. Default it 1 kHz. Valid values are from 100Hz to 40000Hz
	FluentLight(byte ledPin, int pwmRange = DEFAULT_PWM_RANGE, int pwmFreq = DEFAULT_PWM_FREQUENCY);

	/// @brief Start FluentLight
	void begin();

	/// @brief Process FluentLight logic. This method should be added in loop()
	void process();
	
	void turnOn();

	void setMaxBrightness(int bright);
	int getMaxBrightness();

	void setBrightenTime(unsigned long milliSecond);
	unsigned long getBrightenTime();

	void setFadeTime(unsigned long milliSecond);
	unsigned long getFadeTime();

	void setRunningDuration(unsigned long milliSecond);
	unsigned long getRunningDuration();

	/// @brief Call back function of turnOn state changed event
	onStateChangedEvent_t onStateChanged;
private:
	void updateBrightness(bool lightOn);
	void scheduleNextOp(unsigned long additionalTime);
	bool isNextOpDue();
	void changeState(State state);
};

#endif	// FluentLight_H
