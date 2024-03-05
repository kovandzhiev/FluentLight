/*!
 * @file FluentLight.cpp
 *
 * This is a library to control LED light fluently
 *
 * This is an Arduino library which allows you to manage LED light
 *
 * Written by Plamen Kovandzhiev kovandjiev@gmail.com
 * It is licensed under the MIT License, see LICENSE.txt.
 */

#include "FluentLight.h"

//#define DEBUG

byte _pin;
word _maxBrightness;
word _brightness;
word _tempBrightness;
unsigned long _brightenTime;
unsigned long _fadeTime;
unsigned long _runningDuration;
enum State { Off, Brighten, On, Fade };
State _state;
unsigned long _nextOperationTime;

FluentLight::FluentLight(byte pin) {
    _pin = pin;
    _tempBrightness = 0;
    _state = Off;
	_maxBrightness = MAX_BRIGHTNESS;
	_brightenTime = DEFAULT_BRIGHTEN_TIME_MS;
	_fadeTime = DEFAULT_FADE_TIME_MS;
	_runningDuration = DEFAULT_RUNNING_DURATION_MS;
}

void FluentLight::begin() {
    pinMode(_pin, OUTPUT);

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
            _state = Brighten;
        }

        return;
    }

    if (_state == Brighten) {
        if (_brightness >= _maxBrightness) {
#ifdef DEBUG
	Serial.println("FluentLight set state to \'On\'");
#endif
            _state = On;
            setNextOperationTime(_runningDuration);
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
        _state = Fade;

        return;
    }
    
    if (_state == Fade)
    {
        if (lightOn) {
#ifdef DEBUG
	Serial.println("FluentLight set state from \'Fade\' to \'Brighten\'");
#endif
            _state = Brighten;

            return;
        }
        
        if (_brightness == 0)
        {
#ifdef DEBUG
	Serial.println("FluentLight set state to \'Off\'");
#endif
            _state = Off;
            return;
        }
        
        setNextOperationTime(_fadeTime / _maxBrightness);
        _brightness--;
    }
}

void FluentLight::setMaxBrightness(word bright)
{
    _maxBrightness = bright;
}

word FluentLight::getMaxBrightness()
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
