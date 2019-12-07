`timescale 1ns / 1ps
//////////////////////////////////////////////////////////
module circuitN(a, b, c);
input [7:0] a;
input [15:0] b;
output [31:0] c;
wire [31:0] cwire;
ADD #(32) Add0({24'b0, a}, {16'b0, b}, cwire);
REG #(32) Reg1(cwire, 0, 0, c);
endmodule