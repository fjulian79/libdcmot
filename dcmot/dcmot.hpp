/*
 * dcmot, a class used to operate a DC motor connected to a TB6612FNG driver.
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

#ifndef DCMOT_HPP_
#define DCMOT_HPP_

#include <Arduino.h>
#include <stdint.h>

/**
 * @brief Definition of the DcMot parameter structure.
 * 
 */
typedef struct 
{
    /**
     * @brief Pointer to the timer instance to use.
     * 
     * The timer has to be be initalized sperately, see README.md
     */
    HardwareTimer *pTim;

    /**
     * @brief The pin number connected to the A driver pin.
     */
    uint32_t A;
    
    /**
     * @brief The pin number connected to the B driver pin.
     */
    uint32_t B;
    
    /**
     * @brief The pin number connected to the PWM driver pin.
     */
    uint32_t Pwm;

}DcMotParams_t;

class DcMot
{
    public:

        /**
         * @brief Construct a new DcMot object
         * 
         * Attention, the object has to be initilaized before it can be used, 
         * see init(...);
         */
        DcMot();

        /**
         * @brief Construct a new DcMot object
         * 
         * @param pParams   Pointer to the parameters.
         * @param res       Optional user resuloution, defaults to 100.
         *                  See README.md for details.
         */
        DcMot(DcMotParams_t *pParms, uint16_t res = 100);

        /**
         * @brief Innitializes DcMot object
         * 
         * @param pParams   Pointer to the parameters.
         * @param res       Optional user resuloution, defaults to 100.
         *                  See README.md for details.
         */
        void init(DcMotParams_t *pParms, uint16_t res = 100);

        /**
         * @brief Sets the motor speed including the direction.
         * 
         * The given value will be limmited by the configured resolution.
         * 
         * @param val   The new speed value.
         *              If the value is zero the motor is idle.
         *              If the value is positive the motor spins forward.
         *              If the vlaue is negative the motor spins backward
         */
        void set(int32_t val);

        /**
         * @brief Stops the motor.
         * 
         * Equal to set(0)
         */
        void stop(void);

        /**
         * @brief Enables the eddy break.
         * 
         * The given value will be limmited by the configured resolution.
         * 
         * @param val   The amount of break to apply. The value is limmited
         *              by the configured resolution.
         */
        void ebreak(uint32_t val);

    private:

        /**
         * @brief Pointer to the used timer instance.
         */
        HardwareTimer *pTimer;
        
        /**
         * @brief The pin number connected to the A driver pin. 
         */
        int A;

        /**
         * @brief The pin number connected to the B driver pin.
         */
        int B;

        /**
         * @brief The CC chanel used to generate the pwm signal.
         */
        uint8_t Channel;

        /**
         * @brief The The timer overflow value.
         * 
         * This is also the maximum value to be set at the cc channel.
         */
        uint32_t ARR;    

        /**
         * @brief The user space resultion to map to the cc channel.
         * 
         */
        int32_t Resolution;
};

#endif /* DCMOT_HPP_ */