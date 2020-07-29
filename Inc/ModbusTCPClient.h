/*
 * ModbusTCPClient.h
 *
 *  Created on: 29 апр. 2020 г.
 *      Author: Denis Paradiuk
 */

#ifndef INC_MODBUSTCPCLIENT_H_
#define INC_MODBUSTCPCLIENT_H_

#include "esp8266.h"

bool ReadModbusTCP(uint16_t deviceAdrr, uint16_t FunctionalCode,
		uint16_t StartingAdrr, uint16_t NumberOfVal, uint8_t* rxData, uint16_t rxDataSize);

bool WriteModbusTCP(uint16_t deviceAdrr, uint16_t FunctionalCode,
		uint16_t StartingAdrr, uint16_t Value);

#endif /* INC_MODBUSTCPCLIENT_H_ */
