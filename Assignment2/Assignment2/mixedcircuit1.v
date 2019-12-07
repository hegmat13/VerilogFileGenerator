`timescale 1ns / 1ps
//////////////////////////////////////////////////////////
module circuitN(a, b, c);
input signed [7:0] a;
input [7:0] b;
output signed [7:0] c;
wire signed [7:0] cwire;
SADD #(8) SAdd0(a, b, cwire);
SREG #(8) SReg1(cwire, 0, 0, c);
endmodule