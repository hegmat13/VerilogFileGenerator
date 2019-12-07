// dpgen.cpp : Defines the entry point for the application.
//

#include "dpgen.h"
#include <fstream>
#include "Process.h"
//#include <vector>
//#include "CircuitSignal.h"

using namespace std;

int main(int argc, char *argv[])
{
	const int NUM_ARGS = 3;
	int status = -1;
	ifstream netlistFileIn;
	ofstream verilogFileOut;
	Process process;

	// Check command line syntax
	if (argc != NUM_ARGS)
	{
		cout << "Usage: dpgen infile.txt outfile.v" << endl;
		return 1;
	}

	try
	{
		// Open command line files
		netlistFileIn.open(argv[1]);
		if (!(netlistFileIn.is_open()))
		{
			status = FAIL_OPEN;
			throw status;
		}

		verilogFileOut.open(argv[2]);
		if (!(verilogFileOut.is_open()))
		{
			status = FAIL_OPEN;
			throw status;
		}
		
        process.setVerilogFilename(argv[2]);

        // Convert tokens in netlist file to verilog tokens and write to
		// verilog file
		status = process.parseNetlist(netlistFileIn, verilogFileOut);
        if (status != SUCCESS)
        {
            throw status;
        }

		// Close files
		netlistFileIn.close();
		verilogFileOut.close();
	}
	catch (int exception_thrown)
	{
		if (exception_thrown == UNKNOWN_ERROR)
		{
			cerr << "Unknown Exception occurred" << endl;
		}
		else if (exception_thrown == FAIL_OPEN)
		{
			cerr << "File failed to open" << endl;
		}
        else if (exception_thrown == FAIL_OPERAND)
        {
            cerr << "Input/Output does not exist for component" << endl;
        }
        else if (exception_thrown == FAIL_OPERATOR)
        {
            cerr << "Invalid operation" << endl;
        }
		else if (exception_thrown == FAIL_FILE_TYPE)
        {
            cerr << "Invalid file type for output. Please use .v" << endl;
        }

        if (netlistFileIn.is_open())
            netlistFileIn.close();

        if (verilogFileOut.is_open())
            verilogFileOut.close();
    }
	
	return 0;
}
