# ECE474_Assignment2

Alejandro Alvarez (aalvarez1)
Ian Hooks (ihooks)
Mubarak Hassan (mubarakhassan)
Matt Heger (heger)

ECE 474

The program uses command line arguments to read an input file, parse the text into vectors of inputs, outputs, wires, registers and equations, then based off the infomration in those vectora we outputed verilog command equivalents and wrote the verilog commands to a .v file.

One issue we had was with the error checking, wiht the commented out part in the isvalid function it can find which variable is extra in the equations but in the 3rd circuit it doesn't find the c and prints it out saying that is missing. So we left that part commented out.
