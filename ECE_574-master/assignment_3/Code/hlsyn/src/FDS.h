#include "Process.h"

#include <vector>

#ifndef FDS_H
#define FDS_H

using namespace std;

enum 
{
	MUL,
	DIV_MOD,
	ADD_SUB,
	LOGIC,
	NUM_OPS
};

class FDS
{
	private:
		void computeInterval(CircuitComp &circuitComp);
		void computeRange(CircuitComp &circuitComp);
		void computeProbability(CircuitComp &circuitComp);
		void computeCost(vector<CircuitComp> &circuitCompVector);
		void updateDependents(vector<CircuitComp> circuitCompVector, CircuitComp &circuitComp, int updateTimeFrame);
		void assignSuccessors(vector<CircuitComp> &componentVector);
		void assignPredecessors(vector<CircuitComp> &circuitCompVector);
		void assignASAPTimes(vector<CircuitComp> &circuitCompVector);
		void assignALAPTimes(vector<CircuitComp> &circuitCompVector);
		void updateProbability(CircuitComp &circuitComp);
		void checkIfStatements(vector<CircuitComp> &circuitCompVector);
		void updateIfStatements(vector<CircuitComp> &circuitCompVector);

		int latency;
		vector<double> cost;
		vector<CircuitComp> listOfUpdatedSuccessors;
	
	public:
		void scheduleFDS(vector<CircuitComp> &circuitCompVector);
		FDS(int inLatency);
		~FDS();
};
#endif