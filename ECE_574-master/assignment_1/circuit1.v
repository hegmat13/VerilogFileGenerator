`timescale 1ns / 1ns
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/22/2018 12:14:38 PM
// Design Name: 
// Module Name: circuit1
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module circuit1(a, b, c, Clk, Rst, z, x);

	parameter Int8 = 8;
	parameter Int16 = 16;
	input [Int8-1:0] a, b, c;
	input Clk, Rst;

	output [Int8-1:0] z;
	output [Int16-1:0] x;

	wire [Int8-1:0] d, e;
	wire [Int16-1:0] f, g;  
	wire [Int16-1:0] xwire;

	ADD    #(.DATAWIDTH(Int8)) add_1(a, b, d); // d = a + b
	ADD    #(.DATAWIDTH(Int8)) add_2(a, c, e); // e = a + c
	COMP   #(.DATAWIDTH(Int16)) comp_1(d, e, g, 0, 0); // g = d > e
	MUX2x1 #(.DATAWIDTH(Int16)) mux2x1_1(d, e, g, z); // z = g ? d : e 
	MUL    #(.DATAWIDTH(Int16)) mul_1 (a, c, f); // f = a * c
    SUB    #(.DATAWIDTH(Int16)) sub_1(f, d, xwire); // xwire = f - d 
	REG    #(.DATAWIDTH(Int16)) reg_1(xwire, Clk, Rst, x); // x = xwire



endmodule
