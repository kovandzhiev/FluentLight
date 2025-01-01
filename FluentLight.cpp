/*!
 * @file FluentLight.cpp
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

#include "FluentLight.h"

// #define DEBUG

byte _pin;
int _pwmRange;
int _pwmFrequency;
int _maxBrightness;
int _brightness;
int _tempBrightness;
unsigned long _brightenTime;
unsigned long _fadeTime;
unsigned long _runningDuration;
FluentLight::State _state;
unsigned long _nextOperationTime;

FluentLight::FluentLight(byte pin) {
    FluentLight(pin, DEFAULT_PWM_RANGE, DEFAULT_PWM_FREQUENCY);
}

FluentLight::FluentLight(byte pin, int pwmRange, int pwmFrequency) {
    _pin = pin;
    _pwmRange = pwmRange;
    _pwmFrequency = pwmFrequency;
    _tempBrightness = 0;
    _state = Off;
	_maxBrightness = MAX_BRIGHTNESS;
	_brightenTime = DEFAULT_BRIGHTEN_TIME_MS;
	_fadeTime = DEFAULT_FADE_TIME_MS;
	_runningDuration = DEFAULT_RUNNING_DURATION_MS;
}

void FluentLight::begin() {
    pinMode(_pin, OUTPUT);

    // PWM range. Default is 255
	analogWriteRange(_pwmRange);
	// PWM frequency. Default it 1 kHz. Valid values are from 100Hz to 40000Hz
	analogWriteFreq(_pwmFrequency);

    _tempBrightness = 0;
    _brightness = 0;
    analogWrite(_pin, _brightness);
}

void FluentLight::process() {
    if (_state == Off) {
        return;
    }

    processBrightness(false);
    
    if (_tempBrightness != _brightness)
    {
#ifdef DEBUG
	Serial.print("FluentLight brightness: ");
	Serial.println(_brightness);
#endif
        analogWrite(_pin, _brightness);
        _tempBrightness = _brightness;
    }
}

void FluentLight::on() {
    processBrightness(true);
}

void FluentLight::setNextOperationTime(unsigned long additionalTime) {
    _nextOperationTime = millis() + additionalTime;
}

bool FluentLight::isItTimeForNextOperation() {
    return millis() > _nextOperationTime;
}

void FluentLight::processBrightness(bool lightOn) {
    // Waiting for the next operation
    if (!isItTimeForNextOperation() && !lightOn) {
		return;
	}

    if (_state == Off) {
        if (lightOn) {
#ifdef DEBUG
	        Serial.println("FluentLight set state to \'Brighten\'");
#endif
            changeState(Brighten);
        }

        return;
    }

    if (_state == Brighten) {
        if (_brightness >= _maxBrightness) {
#ifdef DEBUG
	Serial.println("FluentLight set state to \'On\'");
#endif
            setNextOperationTime(_runningDuration);
            changeState(On);
            return;
        }
        
        setNextOperationTime(_brightenTime / _maxBrightness);
        _brightness++;

        return;
    }
    
    if (_state == On) {
        if(lightOn) {
#ifdef DEBUG
	        Serial.println("FluentLight extend running duration");
#endif
            setNextOperationTime(_runningDuration);
            return;
        }

#ifdef DEBUG
	Serial.println("FluentLight set state to \'Fade\'");
#endif
        changeState(Fade);
        return;
    }
    
    if (_state == Fade)
    {
        if (lightOn) {
#ifdef DEBUG
	Serial.println("FluentLight set state from \'Fade\' to \'Brighten\'");
#endif
            changeState(Brighten);
            return;
        }
        
        if (_brightness == 0)
        {
#ifdef DEBUG
	Serial.println("FluentLight set state to \'Off\'");
#endif
            changeState(Off);
            return;
        }
        
        setNextOperationTime(_fadeTime / _maxBrightness);
        _brightness--;
    }
}

void FluentLight::setMaxBrightness(int bright)
{
    if (_maxBrightness != bright)
    {
        _maxBrightness = bright;
        // Only if state is On the brightness will be changed
        if (_state == On)
        {
            _brightness = _maxBrightness;
        }
    }
}

int FluentLight::getMaxBrightness()
{
    return _maxBrightness;
}

void FluentLight::setBrightenTime(unsigned long milliSecond)
{
    _brightenTime = milliSecond;
}

unsigned long FluentLight::getBrightenTime()
{
    return _brightenTime;
}

void FluentLight::setFadeTime(unsigned long milliSecond)
{
    _fadeTime = milliSecond;
}

unsigned long FluentLight::getFadeTime()
{
    return _fadeTime;
}

void FluentLight::setRunningDuration(unsigned long milliSecond)
{
    _runningDuration = milliSecond;
}

unsigned long FluentLight::getRunningDuration()
{
    return _runningDuration;
}

void FluentLight::changeState(State state)
{
    _state = state;
    if (onStateChanged)
    {
        onStateChanged(_state);
    }
}
