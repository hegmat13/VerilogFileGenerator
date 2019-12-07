`timescale 1ns / 1ps
//////////////////////////////////////////////////////////
module circuitN(a, b, c, z, x);
input signed [31:0] a;
input signed [31:0] b;
input signed [31:0] c;
output signed [31:0] z;
output signed [31:0] x;
wire signed [31:0] d;
wire signed [31:0] e;
wire signed [31:0] f;
wire signed [31:0] g;
wire signed [31:0] h;
wire signed dLTe;
wire signed dEQe;
wire signed [31:0] zwire;
wire signed [31:0] xwire;
SADD #(32) SAdd0(a, b, d);
SADD #(32) SAdd1(a, c, e);
SSUB #(32) SSub2(a, b, f);
SCOMP #(32) SComp3(d, e, 0, 0, dEQe);
SCOMP #(32) SComp4(d, e, 0, dLTe, 0);
SMUX2x1 #(32) SMux5(d, e, dLTe, g);
SMUX2x1 #(32) SMux6(g, f, dEQe, h);
SSHL #(32) SShl7(g, dLTe, xwire);
SSHR #(32) SShr8(h, dEQe, zwire);
SREG #(32) SReg9(xwire, 0, 0, x);
SREG #(32) SReg10(zwire, 0, 0, z);
endmodule