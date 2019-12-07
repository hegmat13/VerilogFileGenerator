`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/22/2018 02:11:41 PM
// Design Name: 
// Module Name: circuit3
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


module circuit3(a, b, c,d, e, f, g, h,sa, Clk, Rst, avg);
input Clk, Rst;
parameter Int16 = 16;
	input [Int16-1:0] a, b, c, d, e, f, g, h;

output [Int16-1:0] avg;

parameter Int8 = 8;
	input [Int8-1:0] sa;
	
parameter Int32 = 32;
	wire [Int32-1:0] wire_l00, wire_l01, wire_l02, wire_l03, wire_l10, wire_l11, wire_l2, wire_l2div2, wire_l2div4, wire_l2div8;


	ADD    #(.DATAWIDTH(Int32)) add_1(a, b, wire_l00); // l00 = a + b
	ADD    #(.DATAWIDTH(Int32)) add_2(c, d, wire_l01); // l01 = c + d
	ADD    #(.DATAWIDTH(Int32)) add_3(e, f, wire_l02); // l02 = e + f
	ADD    #(.DATAWIDTH(Int32)) add_4(g, h, wire_l03); // l03 = g + h
	ADD    #(.DATAWIDTH(Int32)) add_5(wire_l00, wire_l01, wire_l10); // l10 = l00 + l01
	ADD    #(.DATAWIDTH(Int32)) add_6(wire_l02, wire_l03, wire_l11); // l11 = l02 + l03
	ADD    #(.DATAWIDTH(Int32)) add_7(wire_l10, wire_l11, wire_l2);  // l2 = l10 + l11
	SHR    #(.DATAWIDTH(Int32)) shr_1(wire_l2, sa, wire_l2div2); //l2div2 = l2 >> sa
	SHR    #(.DATAWIDTH(Int32)) shr_2(wire_l2div2, sa, wire_l2div4); //l2div4 = l2div2 >> sa 
	SHR    #(.DATAWIDTH(Int32)) shr_3(wire_l2div4, sa, wire_l2div8); //l2div8 = l2div4 >> sa
	REG    #(.DATAWIDTH(Int32)) reg_1(wire_l2div8, Clk, Rst, avg); // avg= l2div8 
endmodule
