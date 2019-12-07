`timescale 1ns / 1ps
//////////////////////////////////////////////////////////
module circuitN(a, b, c);
input [31:0] a;
input [15:0] b;
output [7:0] c;
wire [7:0] cwire;
ADD #(8) Add0(a[7:0], b[7:0], cwire);
REG #(8) Reg1(cwire, 0, 0, c);
endmodule