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

byte _ledPin;
int _pwmRange;
int _pwmFreq;
int _maxBrightness;
int _currentBrightness;
int _prevBrightness;
unsigned long _brightenTime;
unsigned long _fadeTime;
unsigned long _runningDuration;
FluentLight::State _state;
unsigned long _nextOpTime;

FluentLight::FluentLight(byte ledPin, int pwmRange, int pwmFreq) {
    _ledPin = ledPin;
    _pwmRange = pwmRange;
    _pwmFreq = pwmFreq;
    _prevBrightness = 0;
    _state = Off;
	_maxBrightness = MAX_BRIGHTNESS;
	_brightenTime = DEFAULT_BRIGHTEN_TIME_MS;
	_fadeTime = DEFAULT_FADE_TIME_MS;
	_runningDuration = DEFAULT_RUNNING_DURATION_MS;
}

void FluentLight::begin() {
    pinMode(_ledPin, OUTPUT);

    // PWM range. Default is 255
	analogWriteRange(_pwmRange);
	// PWM frequency. Default it 1 kHz. Valid values are from 100Hz to 40000Hz
	analogWriteFreq(_pwmFreq);

    _prevBrightness = 0;
    _currentBrightness = 0;
    analogWrite(_ledPin, _currentBrightness);
}

void FluentLight::process() {
    if (_state == Off) {
        return;
    }

    updateBrightness(false);
    
    if (_prevBrightness != _currentBrightness)
    {
#ifdef DEBUG
	Serial.print("FluentLight brightness: ");
	Serial.println(_currentBrightness);
#endif
        analogWrite(_ledPin, _currentBrightness);
        _prevBrightness = _currentBrightness;
    }
}

void FluentLight::turnOn() {
    updateBrightness(true);
}

void FluentLight::scheduleNextOp(unsigned long additionalTime) {
    _nextOpTime = millis() + additionalTime;
}

bool FluentLight::isNextOpDue() {
    return millis() > _nextOpTime;
}

void FluentLight::updateBrightness(bool lightOn) {
    // Waiting for the next operation
    if (!isNextOpDue() && !lightOn) {
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
        if (_currentBrightness >= _maxBrightness) {
#ifdef DEBUG
	Serial.println("FluentLight set state to \'On\'");
#endif
            scheduleNextOp(_runningDuration);
            changeState(On);
            return;
        }
        
        scheduleNextOp(_brightenTime / _maxBrightness);
        _currentBrightness++;

        return;
    }
    
    if (_state == On) {
        if(lightOn) {
#ifdef DEBUG
	        Serial.println("FluentLight extend running duration");
#endif
            scheduleNextOp(_runningDuration);
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
        
        if (_currentBrightness == 0)
        {
#ifdef DEBUG
	Serial.println("FluentLight set state to \'Off\'");
#endif
            changeState(Off);
            return;
        }
        
        scheduleNextOp(_fadeTime / _maxBrightness);
        _currentBrightness--;
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
            _currentBrightness = _maxBrightness;
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
