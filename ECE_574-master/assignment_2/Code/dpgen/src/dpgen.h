// dpgen.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>

// TODO: Reference additional headers your program requires here.
#include <string>

enum ErrorStatus
{
    UNKNOWN_ERROR = -1,
    FAIL_OPEN = -2,
    FAIL_OPERAND = -3,
    FAIL_OPERATOR = -4,
	FAIL_FILE_TYPE = -5,
    SUCCESS = 1
};