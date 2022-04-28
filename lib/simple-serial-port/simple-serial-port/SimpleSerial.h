#pragma once

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <string.h>
#include <chrono>
#include <thread>
#include <time.h>
#include <fstream>

using namespace std;

enum DEFAULT_BAUD_RATES {
	B300 = CBR_300,
	B1200 = CBR_1200,
	B2400 = CBR_2400,
	B4800 = CBR_4800,
	B9600 = CBR_9600,
	B19200 = CBR_19200,
	B57600 = CBR_57600,
	B115200 = CBR_115200,
	B128000 = CBR_128000,
	B256000 = CBR_256000,
};

class SimpleSerial
{

private:
	HANDLE io_handler_;
	COMSTAT status_;
	DWORD errors_;

	string syntax_name_;
	char front_delimiter_;
	char end_delimiter_;
	void CustomSyntax(string syntax_type);	

	//
	uint64_t errorCode;
	//

public:
	SimpleSerial(char* com_port, DWORD COM_BAUD_RATE);

	string ReadSerialPort(double reply_wait_time, string syntax_type);
	bool WriteSerialPort(char *data_sent);
	bool CloseSerialPort();
	~SimpleSerial();
	bool connected_;

	//
	uint64_t getErrorCode() { return errorCode; }
	//

	//Error Codes:
	//1: Initialization of SimpleSerial Object failed because the BaudRate is invalid
	//2: Initialization of SimpleSerial Object failed because nothing is connected to the specified COM Port

};

