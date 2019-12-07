#pragma once

#include <string>
using namespace std;

class CircuitSignal
{
	string signal_name;
	string signal_type;
	string data_type;
	string signal_width;
	string signal_position;
	bool ifCondition;
	int ifConditionLevel;
	string nestedWith;

public:
	CircuitSignal();
	~CircuitSignal(){};
	
	string getCircuitSignalName();
	void setCircuitSignalName(string sig_name);
	string getCircuitSignalType();
	void setCircuitSignalType(string sig_type);
	string getCircuitSignalDataType();
	void setCircuitSignalDataType(string sig_data_type);
	string getCircuitSignalWidth();
	void setCircuitSignalWidth(string sig_width);
	string getCircuitSignalPosition();
	void setCircuitSignalPosition(string sig_pos);
	bool getCircuitSignalIfCondition();
	void setCircuitSignalIfCondition(bool sig_if_condition);
	int getCircuitSignalIfConditionLevel();
	void setCircuitSignalIfConditionLevel(int sig_if_condition_level);
	string getIfNestedWith();
	void setIfNestedWith(string nested);
};

