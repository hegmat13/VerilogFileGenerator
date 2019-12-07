double Process::addDependentDelaysOneOP(vector<CircuitComp> &circuitCompVector, string op1) //add passing in vector
{
	double addedDelay = 0;
	
	for (int i = 0; i != circuitCompVector.size(); i++)
	{
		if (op1 == circuitCompVector.at(i).result) //if the current operand was ever a result then this component needs to "wait" for that instantiation
		{
			//set the added delay based on the current component instantiations' data width and type
			addedDelay += delays.stringToDelay(circuitCompVector.at(i).type, circuitCompVector.at(i).width);
		}
	}
	
	return addedDelay;
}

//NOTE: Also use for COMP since we only need to look at two operands
double Process::addDependentDelaysTwoOP(vector<CircuitComp> &circuitCompVector, string op1, string op2)
{
	double addedDelay = 0;
	
	for (int i = 0; i != circuitCompVector.size(); i++) //check Comp Vector if there's a dependency on op1
	{
		if (op1 == circuitCompVector.at(i).result) //if the current operand was ever a result then this component needs to "wait" for that instantiation
		{
			//set the added delay based on the current component instantiations' data width and type
			addedDelay += delays.stringToDelay(circuitCompVector.at(i).type, circuitCompVector.at(i).width);
		}
	}
	
	for (int i = 0; i != circuitCompVector.size(); i++) //check Comp Vector if there's a dependency on op2
	{
		if (op2 == circuitCompVector.at(i).result) //if the current operand was ever a result then this component needs to "wait" for that instantiation
		{
			//set the added delay based on the current component instantiations' data width and type
			addedDelay += delays.stringToDelay(circuitCompVector.at(i).type, circuitCompVector.at(i).width);
		}
	}
	
	return addedDelay;
}

//loops through the Comp vector to find the element with the largest delay time and returns that time
double Process::CalculateCiritcalPath(vector<CircuitComp> &circuitCompVector)
{
	double maxTime = 0;
	for (int i = 0; i != circuitCompVector.size(); i++) 
	{
		if(circuitCompVector.at(i).delay > maxTime)
			maxTime = circuitCompVector.at(i).delay;
	}
	
	return maxTime;
	
}