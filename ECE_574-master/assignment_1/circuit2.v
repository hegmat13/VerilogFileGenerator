`timescale 1ns / 1ns
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/19/2018 10:30:10 PM
// Design Name: 
// Module Name: circuit2
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

module circuit2(a, b, c, Clk, Rst, z, x);

    input Clk, Rst;
	parameter Int32 = 32;
	input [Int32-1:0] a, b, c;

	output [Int32-1:0] z, x;

	wire [Int32-1:0] d, e, f, g, h;
	wire dLTe, dEQe;  
	wire [Int32-1:0] zwire, xwire;

	ADD    #(.DATAWIDTH(Int32)) add_1(a, b, d); // d = a + b
	ADD    #(.DATAWIDTH(Int32)) add_2(a, c, e); // e = a + c
	SUB    #(.DATAWIDTH(Int32)) sub_1(a, b, f); // f = a - b  
	COMP   #(.DATAWIDTH(Int32)) comp_1(d, e, 0, dLTe, dEQe); // dEQe = d == e // dLTe = d < e
	MUX2x1 #(.DATAWIDTH(Int32)) mux2x1_1(d, e, dLTe, g); // g = dLTe ? d : e 
	MUX2x1 #(.DATAWIDTH(Int32)) mux2x1_2(g, f, dEQe, h); // h = dEQe ? g : f 
	SHL    #(.DATAWIDTH(Int32)) shl_1(g, dLTe, xwire); // xwire = g << dLTe
	SHR    #(.DATAWIDTH(Int32)) shr_2(h, dEQe, zwire); // zwire = h >> dEQe
	REG    #(.DATAWIDTH(Int32)) reg_1(xwire, Clk, Rst, x); // x = xwire
	REG    #(.DATAWIDTH(Int32)) reg_2(zwire, Clk, Rst, z); // z = zwire
	
endmodule