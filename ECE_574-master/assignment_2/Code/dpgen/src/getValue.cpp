#include <iostream>
#include <string>
#include "getValue.h"

using namespace std;

/*
enum operationDelay {REG, ADD, SUB, MUL, COMP, MUX, SHR, SHL, DIV, MOD, INC, DEC};

enum operationResultWidth {ONE, TWO, EIGHT, SIXTEEN, THIRTYTWO, SIXTYFOUR };*/

float pathTimes[12][6] = {
						{2.616, 2.644, 2.879, 3.061, 3.602, 3.966},
						{2.704, 3.713, 4.924, 5.638, 7.270, 9.566},
						{3.024, 3.412, 4.890, 5.569, 7.253, 9.566},
						{2.438, 3.651, 7.453, 7.811, 12.395, 15.354},
						{3.031, 3.934, 5.949, 6.256, 7.264, 8.416},
						{4.083, 4.115, 4.815, 5.623, 8.079, 8.766},
						{3.644, 4.007, 5.178, 6.460, 8.819, 11.095},
						{3.614, 3.980, 5.152, 6.549, 8.565, 11.220},
						{0.619, 2.144, 15.439, 33.093, 86.312, 243.233},
						{0.758, 2.149, 16.078, 35.563, 88.142, 250.583},
						{1.792, 2.218, 3.111, 3.471, 4.347, 6.200},
						{1.792, 2.218, 3.108, 3.701, 4.685, 6.503}};

GetValue::GetValue()
{
}

GetValue::~GetValue()
{
}

double GetValue::stringToDelay(std::string datatype, std::string datawidth)
{
	int firstIndex = -1;
	int secondIndex = -1;

	if (datatype == "REG")
	{
		firstIndex = 0;
	}

	else if ((datatype == "ADD") || (datatype == "SADD"))
	{
		firstIndex = 1;
	}

	else if ((datatype == "SUB") || (datatype == "SSUB"))
	{
		firstIndex = 2;
	}

	else if ((datatype == "MUL") || (datatype == "SMUL"))
	{
		firstIndex = 3;
	}

	else if ((datatype == "COMP") || (datatype == "SCOMP"))
	{
		firstIndex = 4;
	}

	else if (datatype == "MUX" || (datatype == "SMUX"))
	{
		firstIndex = 5;
	}

	else if ((datatype == "SHR") || (datatype == "SSHR"))
	{
		firstIndex = 6;
	}

	else if ((datatype == "SHL") || (datatype == "SSHL"))
	{
		firstIndex = 7;
	}

	else if ((datatype == "DIV") || (datatype == "SDIV"))
	{
		firstIndex = 8;
	}

	else if ((datatype == "MOD") || (datatype == "SMOD"))
	{
		firstIndex = 9;
	}

	else if ((datatype == "INC" ) || (datatype == "SINC"))
	{
		firstIndex = 10;
	}

	else if ((datatype == "DEC") || (datatype == "SDEC"))
	{
		firstIndex = 11;
	}

	else
	{
		cout << "Failed to find first index: " << datatype << endl;
	}

	if (datawidth == "1")
	{
		secondIndex = 0;
	}

	else if (datawidth == "2")
	{
		secondIndex = 1;
	}

	else if (datawidth == "8")
	{
		secondIndex = 2;
	}

	else if (datawidth == "16")
	{
		secondIndex = 3;
	}

	else if (datawidth == "32")
	{
		secondIndex = 4;
	}

	else if (datawidth == "64")
	{
		secondIndex = 5;
	}

	else
	{
		cout << "Second index not found." << endl;
	}

	if ((firstIndex == -1) || (secondIndex == -1))
	{
		cout << "Error getting delay value";
		return -1;
	}

	else
	{
		return pathTimes[firstIndex][secondIndex];
	}

}
