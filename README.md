# ECE474_Assignment2

Alejandro Alvarez (aalvarez1)
Ian Hooks (Ihooks)
Mubarak Hassan (mubarakhassan)
Matt Heger (heger)

ECE 474

The program uses command line arguments to read an input file, parse the text into verilog commands and writes the verilog commands to a .v file.

The program has one issue:
When writing the output file, the bitwidths for the wires will be evaluated at 0 and cause the function to find the max bitwidth to go with a non-wire bitwidth or 0 if both inputs are wires.
