/*
 * ModbusTCPClient.c
 *
 *  Created on: 29 апр. 2020 г.
 *      Author: Denis Paradiuk
 */

#include "ModbusTCPClient.h"

static uint16_t txIDRead = 0;
static uint16_t txIDWrite = 0;

bool ReadModbusTCP(uint16_t deviceAdrr, uint16_t FunctionalCode,
		uint16_t StartingAdrr, uint16_t NumberOfVal, uint8_t* rxData, uint16_t rxDataSize)
{
	if(rxDataSize == 0)
	{
		return false;
	}

	char SendModbusTCP[12];
	memset(SendModbusTCP, 0, sizeof(SendModbusTCP));
	uint16_t msgLen = 0x05 + NumberOfVal;

	txIDRead++;

	char tmp2[50];
	uint8_t buff[50];

	SendModbusTCP[0] = txIDRead >> 8;
	//SendModbusTCP[1] = 32;
	SendModbusTCP[1] = txIDRead;
	//SendModbusTCP[3] = 32;
	SendModbusTCP[2] = 0x00;
	//SendModbusTCP[5] = 32;
	SendModbusTCP[3] = 0x00;
	//SendModbusTCP[7] = 32;
	SendModbusTCP[4] = msgLen >> 8;
	//SendModbusTCP[9] = 32;
	SendModbusTCP[5] = msgLen;
	//SendModbusTCP[11] = 32;
	SendModbusTCP[6] = deviceAdrr;
	//SendModbusTCP[13] = 32;
	SendModbusTCP[7] = FunctionalCode;
	//SendModbusTCP[15] = 32;
	SendModbusTCP[8] = StartingAdrr >> 8;
	//SendModbusTCP[17] = 32;
	SendModbusTCP[9] = StartingAdrr - 1;
	//SendModbusTCP[19] = 32;
	SendModbusTCP[10] = NumberOfVal >> 8;
	//SendModbusTCP[21] = 32;
	SendModbusTCP[11] = NumberOfVal;

	esp8266_SendTCP(SendModbusTCP, sizeof(SendModbusTCP));

	HAL_Delay(1000);

	HAL_UART_Receive_IT(&huart2, tmp2, sizeof(tmp2));

	int size = getTCPbuff(buff);

 	if(size > 0)
	{
		if(rxDataSize >= size)
		{
			for(int i = 0; i < size; i += 2)
			{
				for(int j = 0; j < rxDataSize; j++)
				{
					rxData[j] = buff[size + i];
					break;
				}
			}
		}

		clearTCPbuff();
		return true;
	}
}

bool WriteModbusTCP(uint16_t deviceAdrr, uint16_t FunctionalCode,
		uint16_t StartingAdrr, uint16_t Value)
{
	char SendModbusTCP[12];
	memset(SendModbusTCP, 0, sizeof(SendModbusTCP));
	uint16_t msgLen = 0x05 + Value;

	txIDWrite++;

	char tmp2[50];
	uint8_t buff[50];

	SendModbusTCP[0] = txIDWrite >> 8;
	//SendModbusTCP[1] = 32;
	SendModbusTCP[1] = txIDWrite;
	//SendModbusTCP[3] = 32;
	SendModbusTCP[2] = 0x00;
	//SendModbusTCP[5] = 32;
	SendModbusTCP[3] = 0x00;
	//SendModbusTCP[7] = 32;
	SendModbusTCP[4] = msgLen >> 8;
	//SendModbusTCP[9] = 32;
	SendModbusTCP[5] = msgLen;
	//SendModbusTCP[11] = 32;
	SendModbusTCP[6] = deviceAdrr;
	//SendModbusTCP[13] = 32;
	SendModbusTCP[7] = FunctionalCode;
	//SendModbusTCP[15] = 32;
	SendModbusTCP[8] = StartingAdrr >> 8;
	//SendModbusTCP[17] = 32;
	SendModbusTCP[9] = StartingAdrr - 1;
	//SendModbusTCP[19] = 32;
	SendModbusTCP[10] = Value >> 8;
	//SendModbusTCP[21] = 32;
	SendModbusTCP[11] = Value;

	esp8266_SendTCP(SendModbusTCP, sizeof(SendModbusTCP));

	HAL_Delay(1000);

	HAL_UART_Receive_IT(&huart2, tmp2, sizeof(tmp2));

	int size = getTCPbuff(buff);

	if(size > 0)
	{
		if(size == strlen(buff) && size == strlen(SendModbusTCP))
		{
			return true;
		}
	}

		clearTCPbuff();
		return true;
}
