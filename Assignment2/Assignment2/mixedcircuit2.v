`timescale 1ns / 1ps
//////////////////////////////////////////////////////////
module circuitN(a, b, c);
input signed [7:0] a;
input [15:0] b;
output signed [31:0] c;
wire signed [31:0] cwire;
SADD #(32) SAdd0({{24{a[23]}}, a}, {16'b0, b}, cwire);
SREG #(32) SReg1(cwire, 0, 0, c);
endmodule