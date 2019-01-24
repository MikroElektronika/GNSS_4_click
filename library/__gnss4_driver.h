/*
    __gnss4_driver.h

-----------------------------------------------------------------------------

  This file is part of mikroSDK.

  Copyright (c) 2017, MikroElektonika - http://www.mikroe.com

  All rights reserved.

----------------------------------------------------------------------------- */

/**
@file   __gnss4_driver.h
@brief    GNSS4 Driver
@mainpage GNSS4 Click
@{

@image html libstock_fb_view.jpg

@}

@defgroup   GNSS4
@brief      GNSS4 Click Driver
@{

| Global Library Prefix | **GNSS4** |
|:---------------------:|:-----------------:|
| Version               | **1.0.0**    |
| Date                  | **jan 2019.**      |
| Developer             | **Katarina Perendic**     |

Command parser core configured for AT command based GSM devices.

*/
/* -------------------------------------------------------------------------- */

#include "stdint.h"
#include "stdbool.h"

#ifndef _GNSS4_H_
#define _GNSS4_H_

/**
 * @macro T_GNSS4_P 
 * @brief Global Abstract Macro 
 */
#define T_GNSS4_P    const uint8_t*

/** @defgroup GNSS4_AT_CMDS AT Command Types */                /** @{ */

extern const uint8_t _GNSS4_UNKNOWN ;                                   /**< Unknown command type */
extern const uint8_t _GNSS4_TEST    ;                                   /**< Test command type */
extern const uint8_t _GNSS4_SET     ;                                   /**< Set command type */
extern const uint8_t _GNSS4_GET     ;                                   /**< Get command type */
extern const uint8_t _GNSS4_URC     ;                                   /**< URC */
extern const uint8_t _GNSS4_EXEC    ;                                   /**< Exec command type */
                                                                       /** @} */
/** @defgroup GNSS4_AT_EVARG GNSS4 Event Arguments */   /** @{ */

extern const uint8_t _GNSS4_EVARG_START_T    ;                          /**< Event argument 0 */
extern const uint8_t _GNSS4_EVARG_END_T      ;                          /**< Event argument 1 */
extern const uint8_t _GNSS4_EVARG_EVENT_T    ;                          /**< Event argument 2 */
                          
extern const uint8_t _GNSS4_EVENT_RESPONSE   ;                          /**< Response */
extern const uint8_t _GNSS4_EVENT_TIMEOUT    ;                          /**< Timeout */
extern const uint8_t _GNSS4_EVENT_BUFFER_OUT ;                          /**< Buffer warning */
extern const uint8_t _GNSS4_EVENT_CALLBACK   ;                          /** Callback */
                                                                       /** @} */
/**
 * @typedef T_gnss4_handler
 * @brief Handler Prototype
 */
typedef void ( *T_gnss4_handler )( char *buffer, uint8_t *evArgs );

#ifdef __cplusplus
extern "C"{
#endif

/** @defgroup GNSS4_INIT Driver Init Functions */              /** @{ */

/**
 * @brief Driver Initialization
 * 
 * @param[in] gpioObj pointer to GPIO object
 * @param[in] uartObj pointer to UART object
 *
 * Default UART driver intializaton function.
 */
void gnss4_uartDriverInit(T_GNSS4_P gpioObj, T_GNSS4_P uartObj);

/**
 * @brief Core Initialization
 *
 * @param[in] defaultHdl        default handler
 * @param[in] defaultWdog       default watchdog
 *
 * Initialization should be executed before any other function. User can
 * execute this function later inside the application to reset AT Engine to
 * the default state.
 */
void gnss4_coreInit( T_gnss4_handler defaultHdl, uint32_t defaultWdog );

                                                                       /** @} */
/** @defgroup GNSS4_FUNC Driver Functions */                   /** @{ */

/** 
 * @brief Hardware Flow Control State
 *
 * @param[in] hfcState 
 *
 * Enables or disables Hardware Flow Control.
 */
void gnss4_hfcEnable( bool hfcState );

/**
 * @brief Enables or disables module power
 *
 * @param[in] powerState
 *
 * Turn ON or OFF the module.
 */
void gnss4_modulePower( bool powerState );

/**
 * @brief Receive
 *
 * @param[in] rxInput       char received from the module
 *
 * Function is used to populate response buffer with characters received from
 * the module. This function should be placed inside UART ISR function.It also
 * can be used inside function which constatnly poll the UART RX register.
 */
void gnss4_putc( char rxInput );

/**
 * @brief Engine Tick
 *
 * Function must be placed inside the Timer ISR function which will be called
 * every one millisecond.
 */
void gnss4_tick();

/**
 * @brief Save AT Command to Storage
 *
 * @param[in] pCmd          command string
 * @param[in] timeout       timeout for provided command
 * @param[in] pHandler      handler for provided command
 *
 * Saves handlers and timeout for the particular command.
 */
uint16_t gnss4_setHandler( char *pCmd, uint32_t timeout, T_gnss4_handler pHandler );

/**
 * @brief Simple AT Command
 *
 * @param[in] pCmd          pointer to command string
 *
 * Function should be used in case of simple AT commands which have no
 * additional arguments expected. Most of the AT Commands uses this function.
 */
void gnss4_cmdSingle( char *pCmd );

/**
 * @brief Doble AT Command
 *
 * @param[in] pCmd          pointer to command string
 * @param[in] pArg1         pointer to the string used as additional argument
 *
 * Function should be used with AT commands which expects additional data after
 * the module responses with specific character. In most cases special character
 * is ">" like in example of AT command for SMS sending.
 */
void gnss4_cmdDouble( char *pCmd, char *pArg1 );

/**
 * @brief Triple AT Command
 *
 * @param[in] pCmd          pointer to command string
 * @param[in] pArg1         pointer to the string used as additional argument
 * @param[in] pArg2         pointer to the second additional argument
 *
 * Function should be used with AT command which excepts two additional
 * parameters after the execution of the command. This kind of AT functions are
 * very rare.
 */
void gnss4_cmdTriple( char *pCmd, char *pArg1, char *pArg2 );

/**
 * @brief AT Engine State Machine
 *
 * This function should be placed inside the infinite while loop.
 */
void gnss4_process();

/**
 * @brief GPS Parser function
 *
 * @param[in] dataIn        All data response
 * @param[in] command       The name of the NMEA response ($GPGGA, $GPGLL or $GPRMC.... )
 * @param[in] dataPos       Number position of the data
 * @return Buffer with data
 * 
 * For parsing, use the this function to send the following form of arguments:
 * The name of the NMEA response that you want to parse, the position of the data that you need.
 * As a response - you will get a separate buffer with the requested data
 */
char *gnss4_parser(char *dataIn, char *command, uint8_t dataPos);
                                                                       /** @} */
#ifdef __cplusplus
} // extern "C"
#endif
#endif

/**
    @example Click_GNSS4_STM.c
    @example Click_GNSS4_TIVA.c
    @example Click_GNSS4_CEC.c
    @example Click_GNSS4_KINETIS.c
    @example Click_GNSS4_MSP.c
    @example Click_GNSS4_PIC.c
    @example Click_GNSS4_PIC32.c
    @example Click_GNSS4_DSPIC.c
    @example Click_GNSS4_AVR.c
    @example Click_GNSS4_FT90x.c
    @example Click_GNSS4_STM.mbas
    @example Click_GNSS4_TIVA.mbas
    @example Click_GNSS4_CEC.mbas
    @example Click_GNSS4_KINETIS.mbas
    @example Click_GNSS4_MSP.mbas
    @example Click_GNSS4_PIC.mbas
    @example Click_GNSS4_PIC32.mbas
    @example Click_GNSS4_DSPIC.mbas
    @example Click_GNSS4_AVR.mbas
    @example Click_GNSS4_FT90x.mbas
    @example Click_GNSS4_STM.mpas
    @example Click_GNSS4_TIVA.mpas
    @example Click_GNSS4_CEC.mpas
    @example Click_GNSS4_KINETIS.mpas
    @example Click_GNSS4_MSP.mpas
    @example Click_GNSS4_PIC.mpas
    @example Click_GNSS4_PIC32.mpas
    @example Click_GNSS4_DSPIC.mpas
    @example Click_GNSS4_AVR.mpas
    @example Click_GNSS4_FT90x.mpas
*/                                                                     /** @} */
/* -------------------------------------------------------------------------- */
/*
  __gnss4_driver.h

  Copyright (c) 2017, MikroElektonika - http://www.mikroe.com

  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

3. All advertising materials mentioning features or use of this software
   must display the following acknowledgement:
   This product includes software developed by the MikroElektonika.

4. Neither the name of the MikroElektonika nor the
   names of its contributors may be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY MIKROELEKTRONIKA ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL MIKROELEKTRONIKA BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

----------------------------------------------------------------------------- */
