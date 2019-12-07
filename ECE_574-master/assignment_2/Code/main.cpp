/**************************************************************************************************/

/*
 * File: main.cpp
 * Author: Your Name
 * NetID: Your NetID
 * Date:
 *
 * Description: Main file for dpgen
 *
 */

/**************************************************************************************************/

#include <cstdlib>
#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include "parsing.h"

using namespace std;

int main(int argc, char *argv[]) {
    
	int counter;  
    if(argc==2)
	{
        cout << endl << "Number of arguments incorrect.  Please call program as: /"dpgen inputfile outputfile/"" << endl;
	}
    
	else
	{
		Run(argv[1], argv[2]);
	}
	
    return EXIT_SUCCESS;
}




