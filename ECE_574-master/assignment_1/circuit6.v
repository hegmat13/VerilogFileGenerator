`timescale 1ns / 1ns
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/22/2018 02:55:13 PM
// Design Name: 
// Module Name: circuit6
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


module circuit6(a, b, c, zero,Clk, Rst,z);
input Clk, Rst;
parameter Int64 = 64;
	input [Int64-1:0] a, b, c, zero;
	output [Int64-1:0] z;
	wire [Int64-1:0] e, f, g, zwire;
	wire gEQz;
	
	DEC #(.DATAWIDTH(Int64)) dec_1(a, e); // e = a - 1
	INC #(.DATAWIDTH(Int64)) inc_1(c, f); // f = c + 1   
	MOD #(.DATAWIDTH(Int64)) mod_1(a, c, g); // g = a % c
	COMP #(.DATAWIDTH(Int64)) comp_1(g, zero, 0,0, gEQz); // gEQz = g == zero
	MUX2x1 #(.DATAWIDTH(Int64)) mux2x1_1(e, f, gEQz, zwire); // zwire = gEQz ? e : f 
	REG    #(.DATAWIDTH(Int64)) reg_1(zwire, Clk, Rst, z); // z=zwire
endmodule 
	
	

