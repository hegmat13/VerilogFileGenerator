#include "FDS.h"

#include <assert.h>
#include <string>
#include <iostream>

using namespace std;

FDS::FDS(int inLatency) :
	cost(NUM_OPS*inLatency, 0)
{
	latency = inLatency;
}

FDS::~FDS()
{

}

void FDS::scheduleFDS(vector<CircuitComp> &circuitCompVector)
{
	checkIfStatements(circuitCompVector);
	assignPredecessors(circuitCompVector);
	assignSuccessors(circuitCompVector);
	assignASAPTimes(circuitCompVector);
	assignALAPTimes(circuitCompVector);

	// TODO: As of right now I believe this is all assuming single cycle, do I care about cycles? Don't think I do.
	for (vector<int>::size_type i = 0; i != circuitCompVector.size(); i++)
	{
		//computeInterval(circuitCompVector.at(i));
		computeRange(circuitCompVector.at(i));
		computeProbability(circuitCompVector.at(i));
	}
	
	computeCost(circuitCompVector);
	//computeList(); // TODO: not sure we even need list
	
	// for each type of operation
	// we have 4 operations
	for( int i = 0; i < (NUM_OPS+1); i++)
	{
		// loop all operations of type k
		// will need some way of knowing how to find operations of type k
		// maybe use list here?
		for(vector<int>::size_type k = 0; k != circuitCompVector.size(); k++)
		{
			if (!circuitCompVector.at(k).ignoreOp)
			{
				if (circuitCompVector.at(k).enumType == i)
				{
					// 300 is my version of infinity for now
					unsigned int deltaKbest = 300;
					unsigned int deltaKnew = 0;
					// in this case a step is a time interval
					unsigned int bestStep = 0;

					// loop through all operations of k who have range >=2
					// this means operations with ASAP==ALAP are fixed
					// TODO: changing this to an if statement for now...not sure if we need to loop again once something has been scheduled
					if (circuitCompVector.at(k).range >= 2)
					{
						// loop through the each step in the current interval of the current node
						for (int intervalCnt = circuitCompVector.at(k).timeframe[0]; intervalCnt <= circuitCompVector.at(k).timeframe[1]; intervalCnt++)
						{
							vector<CircuitComp> cpyCircuitCompVector = circuitCompVector;
							// temporarily schedule current operation in current step
							fill(cpyCircuitCompVector.at(k).probability.begin(), cpyCircuitCompVector.at(k).probability.end(), 0);
							cpyCircuitCompVector.at(k).probability.at((intervalCnt - 1)) = 1;

							// temporarily update schedule of dependent operations...???
							// need to updated the time[0] here
							cpyCircuitCompVector.at(k).timeframe[0] = intervalCnt;
							updateDependents(cpyCircuitCompVector, cpyCircuitCompVector.at(k), intervalCnt - 1);

							// compute cost of based on this step = deltaKnew
							computeCost(cpyCircuitCompVector);
							deltaKnew = cost[NUM_OPS*i + (intervalCnt - 1)];
							// if new cost is better than the current best cost, update best
							if (deltaKnew < deltaKbest)
							{
								deltaKbest = deltaKnew;
								bestStep = intervalCnt;
							}
						}

						// schedule current operation with bestStep
						circuitCompVector.at(k).timeframe[0] = bestStep;
						// Update dependent operations from the new schedule
						listOfUpdatedSuccessors.push_back(circuitCompVector.at(k));
						updateDependents(circuitCompVector, circuitCompVector.at(k), bestStep - 1);
						// update all operations with new interval, range, probability, list, cost
						// TODO: Is this the entire circuit that gets updated? 
						for (vector<int>::size_type i = 0; i != circuitCompVector.size(); i++)
						{
							// may need to change this to something else
							// I don't think we actually need to update the intervals, they should be updated
							//computeInterval(circuitCompVector.at(i));
							computeRange(circuitCompVector.at(i));
							updateProbability(circuitCompVector.at(i));
						}
						computeCost(circuitCompVector);
						//computeList();
					}
				}
			}
		}
	}
	
	for (vector<int>::size_type i = 0; i != circuitCompVector.size(); i++)
	{
		circuitCompVector.at(i).timeframe[1] = (circuitCompVector.at(i).timeframe[0] + circuitCompVector.at(i).cycleTime);
	}

	updateIfStatements(circuitCompVector);
}

void FDS::computeInterval(CircuitComp &circuitComp)
{
	/*** Compute the time frames ***/
    //circuitComp.timeframe[0] = calculateASAPtime(j, vectorType componentVector); 
    //circuitComp.timeframe[1] = calculateALAPtime(j, vectorType componentVector);
}

void FDS::computeRange(CircuitComp &circuitComp)
{
	circuitComp.range = circuitComp.timeframe[1] - circuitComp.timeframe[0] + 1; //width = ALAP - ASAP + 1
}

void FDS::computeProbability(CircuitComp &circuitComp)
{	
	//assign probabilities of this element being scheduled for each time interval
    for(int i = 1; i <= latency; i++)
    {
		if (i >= circuitComp.timeframe[0] && i <= circuitComp.timeframe[1])
		{
			double prob = 1.0 / circuitComp.range;
			circuitComp.probability.push_back(prob);
		}
		else
			circuitComp.probability.push_back(0);  
    }
}

void FDS::updateProbability(CircuitComp &circuitComp)
{
	//assign probabilities of this element being scheduled for each time interval
	for (int i = 1; i <= latency; i++)
	{
		if (i >= circuitComp.timeframe[0] && i <= circuitComp.timeframe[1])
		{
			double prob = 1.0 / circuitComp.range;
			circuitComp.probability.at(i-1) = prob;
		}
		else
			circuitComp.probability.at(i-1) = 0;
	}
}

void FDS::computeCost(vector<CircuitComp> &circuitCompVector)
{
	//unsigned int cost[NUM_OPS][latency];
	
	fill(cost.begin(), cost.end(), 0);

	// loop through the vector of comps
	for (vector<int>::size_type i = 0; i != circuitCompVector.size(); i++)
	{
		if (!circuitCompVector.at(i).ignoreOp)
		{
			int step = 0;
			int arrStep = 0;
			if (circuitCompVector.at(i).enumType == MUL)
			{
				step = circuitCompVector.at(i).timeframe[0];
				arrStep = step - 1;
				while (step <= circuitCompVector.at(i).timeframe[1])
				{
					cost[NUM_OPS*MUL + arrStep] += circuitCompVector.at(i).probability.at(arrStep);
					step++;
				}
			}
			else if (circuitCompVector.at(i).enumType == DIV_MOD)
			{
				step = circuitCompVector.at(i).timeframe[0];
				arrStep = step - 1;
				while (step <= circuitCompVector.at(i).timeframe[1])
				{
					cost[NUM_OPS*DIV_MOD + arrStep] += circuitCompVector.at(i).probability.at(arrStep);
					step++;
				}
			}
			else if (circuitCompVector.at(i).enumType == ADD_SUB)
			{
				step = circuitCompVector.at(i).timeframe[0];
				arrStep = step - 1;
				while (step <= circuitCompVector.at(i).timeframe[1])
				{
					cost[NUM_OPS*ADD_SUB + arrStep] += circuitCompVector.at(i).probability.at(arrStep);
					step++;
				}
			}
			else if (circuitCompVector.at(i).enumType == LOGIC)
			{
				step = circuitCompVector.at(i).timeframe[0];
				arrStep = step - 1;
				while (step <= circuitCompVector.at(i).timeframe[1])
				{
					cost[NUM_OPS*LOGIC + arrStep] += circuitCompVector.at(i).probability.at(arrStep);
					step++;
				}

			}
		}
	}
}

void FDS::updateDependents(vector<CircuitComp> circuitCompVector, CircuitComp &circuitComp, int updateTimeFrame)
{
	//TODO: Does this need to be recursive? Can it move past its ALAP time? Will it always move? What if its dependent can't move?
	int index = -1;

	// need to figure out where the current node lives in the vector
	// assuming result is a unique identifier
	for (vector<int>::size_type i = 0; i != circuitCompVector.size(); i++)
	{
		if (!circuitCompVector.at(i).ignoreOp)
		{
			if (circuitCompVector.at(i).result.getCircuitSignalName() == circuitComp.result.getCircuitSignalName())
			{
				index = i;
			}
		}
	}

	int found = 0;
	for (vector<int>::size_type i = 0; i != listOfUpdatedSuccessors.size(); i++)
	{
		if (listOfUpdatedSuccessors.empty())
			break;
		if (listOfUpdatedSuccessors.at(i).result.getCircuitSignalName() == circuitCompVector.at(index).result.getCircuitSignalName())
		{
			listOfUpdatedSuccessors.erase(listOfUpdatedSuccessors.begin()+i);
			i--;
		}
	}

	// loop through the entire vector finding which operations have the current operation as a predecessor
	for (vector<int>::size_type i = 0; i != circuitCompVector.size(); i++)
	{
		if (!circuitCompVector.at(i).ignoreOp)
		{
			for (vector<int>::size_type j = 0; j != circuitCompVector.at(i).predecessor.size(); j++)
			{
				// if we found the current in the predecessors
				if (circuitCompVector.at(i).predecessor.at(j) == index)
				{
					// make sure that it actually needs to move, if ASAP time is less than or equal to current ASAP time
					if (circuitCompVector.at(i).timeframe[0] <= circuitComp.timeframe[0])
					{
						circuitCompVector.at(i).timeframe[0] += updateTimeFrame;
						listOfUpdatedSuccessors.push_back(circuitCompVector.at(i));

						// if we get an ASAP updated time that's greater than ALAP something is wrong
						if (circuitCompVector.at(i).timeframe[0] > circuitCompVector.at(i).timeframe[1])
						{
							circuitCompVector.at(i).timeframe[0] = circuitCompVector.at(i).timeframe[1];
						}
					}
				}
			}
		}
	}

	if (!listOfUpdatedSuccessors.empty())
	{
		updateDependents(circuitCompVector, listOfUpdatedSuccessors.front(), updateTimeFrame);
	}
}

//Pushes back an index value to an elements predecessor and successor after checking the dependency exists. else pushes back -1
//TODO: add vector predecessor[] to component Vector
void FDS::assignSuccessors(vector<CircuitComp> &circuitCompVector)
{
	for (vector<int>::size_type i = 0; i != circuitCompVector.size(); i++) //loop for all elements in the vector
	{
		if (!circuitCompVector.at(i).ignoreOp)
		{
			for (vector<int>::size_type j = 0; j != circuitCompVector.size(); j++) //loop for one element (i) checked against all elements (j)
			{
				if (!circuitCompVector.at(j).ignoreOp)
				{
					int index = 0;
					if (i != j) //don't compare a component to itself
					{
						for (vector<int>::size_type k = 0; k < circuitCompVector.at(j).operands.size(); k++) //TODO get numberofOperands, will this loop make the function generic?
						{
							if (circuitCompVector.at(i).result.getCircuitSignalName() == circuitCompVector.at(j).operands.at(k).getCircuitSignalName()) //check if result is found in the operands of the current element
							{
								circuitCompVector.at(i).successor.push_back(j); //TODO change to push back vector in vector
							}
						}
					} //end k loop
				}
			} //end j loop
		}
	} //end i loop
}

void FDS::assignPredecessors(vector<CircuitComp> &circuitCompVector)
{
	for (vector<int>::size_type i = 0; i != circuitCompVector.size(); i++) //loop for all elements in the vector
	{
		if (!circuitCompVector.at(i).ignoreOp)
		{
			for (vector<int>::size_type j = 0; j != circuitCompVector.size(); j++) //loop for one element (i) checked against all elements (j)
			{
				if (!circuitCompVector.at(j).ignoreOp)
				{
					int index = 0;
					if (i != j) //don't compare a component to itself
					{
						for (vector<int>::size_type k = 0; k < circuitCompVector.at(i).operands.size(); k++) //TODO get numberofOperands, will this loop make the function generic?
						{
							if (circuitCompVector.at(j).result.getCircuitSignalName() == circuitCompVector.at(i).operands.at(k).getCircuitSignalName()) //check if result is found in the operands of the current element
							{
								circuitCompVector.at(i).predecessor.push_back(j); //TODO change to push back vector in vector
							}
						}
					} //end k loop
				}
			} //end j loop
		}
	} //end i loop
}

void FDS::assignASAPTimes(vector<CircuitComp> &circuitCompVector)
{
	int totalVisited = 0;
	for (vector<int>::size_type index = 0; index != circuitCompVector.size(); index++) //whole component vector
	{
		if (!circuitCompVector.at(index).ignoreOp)
		{
			//for loop and if that check for 0 predecessors
			if (circuitCompVector.at(index).predecessor.size() == 0)
			{
				circuitCompVector.at(index).timeframe[0] = 1; //empty //ASAP time[0] = 1
				circuitCompVector.at(index).visited = true;
				totalVisited++;
				for (int succ = 0; succ < circuitCompVector.at(index).successor.size(); succ++)
				{
					int tempAsapTime = circuitCompVector.at(index).timeframe[0] + circuitCompVector.at(index).cycleTime;

					//if (circuitCompVector.at(index).cycleTime > 1)
						//tempAsapTime++;

					if (circuitCompVector.at(circuitCompVector.at(index).successor.at(succ)).timeframe[0] < tempAsapTime)//no other predecessor has a longer (time[0] + cycleTime)
					{
						circuitCompVector.at(circuitCompVector.at(index).successor.at(succ)).timeframe[0] = tempAsapTime;
					}
				}
			}
		}
	}  
	//TODO add global totalVisited
	while (totalVisited != circuitCompVector.size())
	{
		int index = 0;
		bool skipElement = false;
		for (; index < circuitCompVector.size(); index++)//whole vector size
		{
			if (!circuitCompVector.at(index).ignoreOp)
			{
				skipElement = false;
				if (!circuitCompVector.at(index).visited)
				{
					for (int pred = 0; pred < circuitCompVector.at(index).predecessor.size(); pred++) //check all predecessors
					{
						if (!circuitCompVector.at(circuitCompVector.at(index).predecessor.at(pred)).visited) //has any single predecessor not been visited?
						{
							skipElement = true;
							break; //index will be incremented since we break out of the nearest for loop
						}
					}
					if (!skipElement) //if all the predecessors WERE visted, now find which predecessor has the longest time[0]+cycleTime
					{
						skipElement = false;

						//TODO make sure we aren't indexing into a predecessor that doesn't exist for the current compVector.at(index)
						//int tempAsapTime = circuitCompVector.at(circuitCompVector.at(index).predecessor.at(0)).timeframe[0]
						//					+ circuitCompVector.at(circuitCompVector.at(index).predecessor.at(0)).cycleTime;

						//if (circuitCompVector.at(circuitCompVector.at(index).predecessor.at(0)).cycleTime > 1)
						//{
							//tempAsapTime++;
						//}
						//find the longest ASAP start time (time[0]) of all the predecessors
						for (int predTCheck = 0; predTCheck < circuitCompVector.at(index).predecessor.size(); predTCheck++) //check all predecessors
						{
							int tempAsapTime = circuitCompVector.at(circuitCompVector.at(index).predecessor.at(predTCheck)).timeframe[0]
								+ circuitCompVector.at(circuitCompVector.at(index).predecessor.at(predTCheck)).cycleTime;

							if (tempAsapTime > circuitCompVector.at(index).timeframe[0])
							{

								circuitCompVector.at(index).timeframe[0] = tempAsapTime;
								//if (circuitCompVector.at(circuitCompVector.at(index).predecessor.at(predTCheck)).cycleTime > 1)
								//{
									//tempAsapTime++;
								//}
							}

						}
						circuitCompVector.at(index).visited = true; //this node was checked for all it's predecessors, it's done
						totalVisited++; //update total 
					}

				}
			}
			else
			{
				if (!circuitCompVector.at(index).visited)
					totalVisited++;

				circuitCompVector.at(index).visited = true;
			}
		}
	}
}

void FDS::assignALAPTimes(vector<CircuitComp> &circuitCompVector)
{
	int totalVisited = 0;
	for (vector<int>::size_type index = 0; index != circuitCompVector.size(); index++) //whole component vector
	{
		circuitCompVector.at(index).visited = false;

		if (!circuitCompVector.at(index).ignoreOp)
		{
			circuitCompVector.at(index).timeframe[1] = 300;

			//for loop and if that check for 0 predecessors
			if (circuitCompVector.at(index).successor.size() == 0)
			{
				circuitCompVector.at(index).timeframe[1] = latency; //empty //ASAP time[0] = 1
				circuitCompVector.at(index).visited = true;
				totalVisited++;
				for (int pred = 0; pred < circuitCompVector.at(index).predecessor.size(); pred++)
				{
					int tempAlapTime = circuitCompVector.at(index).timeframe[1] - circuitCompVector.at(circuitCompVector.at(index).predecessor.at(pred)).cycleTime;

					//if (circuitCompVector.at(index).cycleTime > 1)
						//tempAsapTime++;

					if (circuitCompVector.at(circuitCompVector.at(index).predecessor.at(pred)).timeframe[1] > tempAlapTime)//no other predecessor has a longer (time[0] + cycleTime)
					{
						circuitCompVector.at(circuitCompVector.at(index).predecessor.at(pred)).timeframe[1] = tempAlapTime;
						
						if (tempAlapTime + circuitCompVector.at(circuitCompVector.at(index).predecessor.at(pred)).cycleTime > latency)
						{
							cout<<"ERROR: Circuit cannot be scheduled within the given latency bounds"<<endl;
							assert(0);
						}
					}
				}
			}
		}
	}
	//TODO add global totalVisited
	while (totalVisited != circuitCompVector.size())
	{
		int index = 0;
		bool skipElement = false;
		for (; index < circuitCompVector.size(); index++)//whole vector size
		{
			if (!circuitCompVector.at(index).ignoreOp)
			{
				skipElement = false;
				if (!circuitCompVector.at(index).visited)
				{
					for (int succ = 0; succ < circuitCompVector.at(index).successor.size(); succ++) //check all predecessors
					{
						if (!circuitCompVector.at(circuitCompVector.at(index).successor.at(succ)).visited) //has any single predecessor not been visited?
						{
							skipElement = true;
							break; //index will be incremented since we break out of the nearest for loop
						}
					}
					if (!skipElement) //if all the predecessors WERE visted, now find which predecessor has the longest time[0]+cycleTime
					{
						skipElement = false;

						//TODO make sure we aren't indexing into a predecessor that doesn't exist for the current compVector.at(index)
						//int tempAsapTime = circuitCompVector.at(circuitCompVector.at(index).predecessor.at(0)).timeframe[0]
						//					+ circuitCompVector.at(circuitCompVector.at(index).predecessor.at(0)).cycleTime;

						//if (circuitCompVector.at(circuitCompVector.at(index).predecessor.at(0)).cycleTime > 1)
						//{
							//tempAsapTime++;
						//}
						//find the longest ASAP start time (time[0]) of all the predecessors
						for (int succTCheck = 0; succTCheck < circuitCompVector.at(index).successor.size(); succTCheck++) //check all predecessors
						{
							int tempAlapTime = circuitCompVector.at(circuitCompVector.at(index).successor.at(succTCheck)).timeframe[1]
								- circuitCompVector.at(index).cycleTime;

							if (tempAlapTime < circuitCompVector.at(index).timeframe[1])
							{

								circuitCompVector.at(index).timeframe[1] = tempAlapTime;
								if ((tempAlapTime + circuitCompVector.at(circuitCompVector.at(index).successor.at(succTCheck)).cycleTime > latency) || (tempAlapTime <= 0) )
								{
									cout<<"ERROR: Circuit cannot be scheduled within the given latency bounds"<<endl;
									assert(0);
								}
								//if (circuitCompVector.at(circuitCompVector.at(index).predecessor.at(predTCheck)).cycleTime > 1)
								//{
									//tempAsapTime++;
								//}
							}

						}
						circuitCompVector.at(index).visited = true; //this node was checked for all it's predecessors, it's done
						totalVisited++; //update total 
					}

				}
			}
			else
			{
				if (!circuitCompVector.at(index).visited)
					totalVisited++;

				circuitCompVector.at(index).visited = true;
			}
		}
	}
}

void FDS::checkIfStatements(vector<CircuitComp> &circuitCompVector)
{
	for (vector<int>::size_type i = 0; i != circuitCompVector.size(); i++)
	{
		for (vector<int>::size_type j = 0; j != circuitCompVector.size(); j++)
		{
			if (i != j)
			{
				if (circuitCompVector.at(i).result.getCircuitSignalName() == circuitCompVector.at(j).result.getCircuitSignalName())
				{
					if (!circuitCompVector.at(i).ignoreOp || !circuitCompVector.at(j).ignoreOp)
					{
						if ((circuitCompVector.at(i).ifStatementLevel > circuitCompVector.at(j).ifStatementLevel) || (circuitCompVector.at(i).elseStatementLevel > circuitCompVector.at(j).elseStatementLevel))
						{
							circuitCompVector.at(j).ignoreOp = true;
						}
						else
						{
							circuitCompVector.at(i).ignoreOp = true;
						}
					}
				}
			}
		}
	}
}

void FDS::updateIfStatements(vector<CircuitComp> &circuitCompVector)
{
	for (vector<int>::size_type i = 0; i != circuitCompVector.size(); i++)
	{
		for (vector<int>::size_type j = 0; j != circuitCompVector.size(); j++)
		{
			if (i != j)
			{
				if (circuitCompVector.at(i).result.getCircuitSignalName() == circuitCompVector.at(j).result.getCircuitSignalName())
				{
					if (circuitCompVector.at(i).ignoreOp)
					{
						circuitCompVector.at(i).timeframe[0] = circuitCompVector.at(j).timeframe[0];
						circuitCompVector.at(i).timeframe[1] = circuitCompVector.at(j).timeframe[1];
					}
					else if (circuitCompVector.at(j).ignoreOp)
					{
						circuitCompVector.at(j).timeframe[0] = circuitCompVector.at(i).timeframe[0];
						circuitCompVector.at(j).timeframe[1] = circuitCompVector.at(i).timeframe[1];
					}
				}
			}
		}
	}
}

/* 
computeList()
{
	// list contains the operations of type k in each step
	component list[NUM_OPS][latency];
	
	// loop through the vector of comps
	for(vectorloop)
	{
		if(type = MUL)
		{
			step = comp.timeframe[0];
			while(step <= comp.timeframe[1])
			{
				list[MUL][step] = comp;
				step++;
			}
		}
		else if(type = DIV_MOD)
		{
			step = comp.timeframe[0];
			while(step <= comp.timeframe[1])
			{
				list[DIV_MOD][step] = comp;
				step++;
			}
		}
		else if(type = ADD_SUB)
		{
			step = comp.timeframe[0];
			while(step <= comp.timeframe[1])
			{
				list[ADD_SUB][step] = comp;
				step++;
			}
		}
		else if(type = LOGIC)
		{
			step = comp.timeframe[0];
			while(step <= comp.timeframe[1])
			{
				list[LOGIC][step] = comp;
				step++;
			}
			
		}
	}
} */