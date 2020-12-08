/*
 * dcmot, a class used to operate a DC motor connected to a TB6612FNG driver by 
 * using STM32Duino hardware timer support.
 * 
 * Copyright (C) 2020 Julian Friedrich
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>. 
 *
 * You can file issues at https://github.com/fjulian79/libdcmot/issues
 */

#include <dcmot/dcmot.hpp>

#include <string.h>

DcMot::DcMot() : 
      pTimer(0)
    , A(0)
    , B(0)
    , Channel(0)
    , ARR(0)
    , Resolution(0)
{
    /* Nothing to do */
}

DcMot::DcMot(DcMotParams_t *pParms, uint16_t res)
{
    init(pParms, res);
}

void DcMot::init(DcMotParams_t *pParms, uint16_t res)
{
    pTimer = pParms->pTim;
    A = pParms->A;
    B = pParms->B;
    Resolution = res;
    Channel = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(pParms->Pwm),
            PinMap_PWM));
    pTimer->setMode(Channel, TIMER_OUTPUT_COMPARE_PWM1,
            digitalPinToPinName(pParms->Pwm));
    pTimer->setCaptureCompare(Channel, 0);
    pTimer->resumeChannel(Channel);
    
    pinMode(A, OUTPUT);
    pinMode(B, OUTPUT);

    limit(100);
}

void DcMot::set(int32_t val)
{
    uint32_t ccval = 0;

    if(pTimer == 0)
    {
        return;
    }

    val = constrain(val, -Resolution, Resolution);

    if (val == 0)
    {
        digitalWrite(A, LOW);
        digitalWrite(B, LOW);
    }
    else if(val < 0)
    {
        val = -val;
        digitalWrite(A, HIGH);
        digitalWrite(B, LOW);
    } 
    else
    {
        digitalWrite(A, LOW);
        digitalWrite(B, HIGH);
    }

    ccval = (ARR * val) / Resolution;
    pTimer->setCaptureCompare(Channel, ccval);
}

void DcMot::stop(void)
{
    set(0);
}

void DcMot::ebreak(uint32_t val)
{
    uint32_t ccval = (ARR * val) / Resolution;
    
    if(pTimer == 0)
    {
        return;
    }

    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    pTimer->setCaptureCompare(Channel, ccval);
}

void DcMot::limit(uint8_t percent)
{
    if (percent > 100)
    {
        percent = 100;
    } 

    ARR = ((pTimer->getOverflow() + 1) * percent) / 100;
}
