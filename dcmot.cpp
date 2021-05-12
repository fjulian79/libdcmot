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
      pPwmTim(0)
    , Drv_A(0)
    , Drv_B(0)
    , PwmCh(0)
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
    pPwmTim = pParms->pPwmTim;
    Drv_A = pParms->Drv_A;
    Drv_B = pParms->Drv_B;
    Resolution = res;
    PwmCh = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(pParms->Drv_Pwm),
            PinMap_PWM));
    pPwmTim->setMode(PwmCh, TIMER_OUTPUT_COMPARE_PWM1,
            digitalPinToPinName(pParms->Drv_Pwm));
    pPwmTim->setCaptureCompare(PwmCh, 0);
    pPwmTim->resumeChannel(PwmCh);
    
    pinMode(Drv_A, OUTPUT);
    pinMode(Drv_B, OUTPUT);

    limit(100);
}

void DcMot::set(int32_t val)
{
    uint32_t ccval = 0;

    if(pPwmTim == 0)
    {
        return;
    }

    val = constrain(val, -Resolution, Resolution);

    if (val == 0)
    {
        digitalWrite(Drv_A, LOW);
        digitalWrite(Drv_B, LOW);
    }
    else if(val < 0)
    {
        val = -val;
        digitalWrite(Drv_A, HIGH);
        digitalWrite(Drv_B, LOW);
    } 
    else
    {
        digitalWrite(Drv_A, LOW);
        digitalWrite(Drv_B, HIGH);
    }

    ccval = (ARR * val) / Resolution;
    pPwmTim->setCaptureCompare(PwmCh, ccval);
}

void DcMot::stop(void)
{
    set(0);
}

void DcMot::ebreak(uint32_t val)
{
    uint32_t ccval = (ARR * val) / Resolution;
    
    if(pPwmTim == 0)
    {
        return;
    }

    digitalWrite(Drv_A, HIGH);
    digitalWrite(Drv_B, HIGH);
    pPwmTim->setCaptureCompare(PwmCh, ccval);
}

void DcMot::limit(uint8_t percent)
{
    if (percent > 100)
    {
        percent = 100;
    } 

    ARR = ((pPwmTim->getOverflow() + 1) * percent) / 100;
}