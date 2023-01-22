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

byte _pin;
word _maxBrightness;
word _brightness;
word _tempBrightness;
unsigned long _lightOnDuration;
unsigned long _lightOffDuration;
unsigned long _onDuration;
enum State { Off, LightOn, On, LightOff };
State _state;
unsigned long _nextOperationTime;
unsigned long _operationInterval;

FluentLight::FluentLight(byte pin) {
    _pin = pin;
    _tempBrightness = 0;
    _state = Off;
	_maxBrightness = MAX_BRIGHTNESS;
	_lightOnDuration = LIGHT_ON_DURATION_MS;
	_lightOffDuration = LIGHT_OFF_DURATION_MS;
	_onDuration = ON_DURATION_MS;
}

void FluentLight::begin() {
    pinMode(_pin, OUTPUT);
    _tempBrightness = 0;
    _brightness = 0;
    analogWrite(_pin, _brightness);
}

void FluentLight::process() {
    processBrightness();
    
    if (_tempBrightness != _brightness)
    {
        analogWrite(_pin, _brightness);
        _tempBrightness = _brightness;
    }
}

void FluentLight::on() {
    processBrightness(true);
}

void FluentLight::processBrightness(bool startLight = false) {
    if (_state == Off && !startLight) {
        return;
    }

    unsigned long milliSec = millis();

    // Extend on duration
    if (_state == On && startLight)
    {
        _nextOperationTime = milliSec + _onDuration;
        return;
    }

    // Waiting for the next operation
    if (_nextOperationTime > milliSec)
	{
		return;
	}

    if (_state == Off && startLight)
    {
        _state = LightOn;
        _operationInterval = _lightOnDuration / _maxBrightness;
    }
    
    if (_state == LightOn)
    {
        if (_brightness >= _maxBrightness)
        {
            _state = On;
            // Set full on duration
            _nextOperationTime = milliSec + _onDuration;
            return;
        }
        
        _brightness++;
    }
    
    // It can come only all full on duration has been went
    if (_state == On)
    {
        _state = LightOff;
        // Calculate light off operation interval
        _operationInterval = _lightOffDuration / _maxBrightness;
    }
    
    if (_state == LightOff)
    {
        if (startLight)
        {
            _state = LightOn;
            _brightness++;
            _operationInterval = _lightOnDuration / _maxBrightness;
            _nextOperationTime = milliSec + _operationInterval;
            return;
        }
        
        if (_brightness == 0)
        {
            _state = Off;
            return;
        }
        
        _brightness--;
    }

    _nextOperationTime = milliSec + _operationInterval;
}

void FluentLight::setMaxBrightness(word bright)
{
    _maxBrightness = bright;
}

word FluentLight::getMaxBrightness()
{
    return _maxBrightness;
}

void FluentLight::setLightOnDuration(unsigned long milliSecond)
{
    _lightOnDuration = milliSecond;
}

unsigned long FluentLight::getLightOnDuration()
{
    return _lightOnDuration;
}

void FluentLight::setLightOffDuration(unsigned long milliSecond)
{
    _lightOffDuration = milliSecond;
}

unsigned long FluentLight::getLightOffDuration()
{
    return _lightOffDuration;
}

void FluentLight::setOnDuration(unsigned long milliSecond)
{
    _onDuration = milliSecond;
}

unsigned long FluentLight::getOnDuration()
{
    return _onDuration;
}
