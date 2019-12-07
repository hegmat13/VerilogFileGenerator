#include "CircuitSignal.h"

using namespace std;

CircuitSignal::CircuitSignal() :
	ifCondition(false),
	ifConditionLevel(0)
{

}

string CircuitSignal::getCircuitSignalName()
{
	return signal_name;
}

void CircuitSignal::setCircuitSignalName(string sig_name)
{
	signal_name = sig_name;
}

string CircuitSignal::getCircuitSignalType()
{
	return signal_type;
}

void CircuitSignal::setCircuitSignalType(string sig_type)
{
	signal_type = sig_type;
}

string CircuitSignal::getCircuitSignalDataType()
{
	return data_type;
}

void CircuitSignal::setCircuitSignalDataType(string sig_data_type)
{
	data_type = sig_data_type;
}

string CircuitSignal::getCircuitSignalWidth()
{
	return signal_width;
}

void CircuitSignal::setCircuitSignalWidth(string sig_width)
{
	signal_width = sig_width;
}

string CircuitSignal::getCircuitSignalPosition()
{
	return signal_position;
}

void CircuitSignal::setCircuitSignalPosition(string sig_pos)
{
	signal_position = sig_pos;
}

bool CircuitSignal::getCircuitSignalIfCondition()
{
	return ifCondition;
}

void CircuitSignal::setCircuitSignalIfCondition(bool sig_if_condition)
{
	ifCondition = sig_if_condition;
}

int CircuitSignal::getCircuitSignalIfConditionLevel()
{
	return ifConditionLevel;
}

void CircuitSignal::setCircuitSignalIfConditionLevel(int sig_if_condition_level)
{
	ifConditionLevel = sig_if_condition_level;
}

string CircuitSignal::getIfNestedWith()
{
	return nestedWith;
}

void CircuitSignal::setIfNestedWith(string nested)
{
	nestedWith = nested;
}