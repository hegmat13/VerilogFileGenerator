`timescale 1ns / 1ps
//////////////////////////////////////////////////////////
module circuitN(a, b, c, z, x);
input signed [63:0] a;
input signed [63:0] b;
input signed [63:0] c;
output signed [31:0] z;
output signed [31:0] x;
wire signed [63:0] d;
wire signed [63:0] e;
wire signed [63:0] f;
wire signed [63:0] g;
wire signed [63:0] h;
wire signed dLTe;
wire signed dEQe;
wire signed [63:0] xrin;
wire signed [63:0] zrin;
wire signed [63:0] greg;
wire signed [63:0] hreg;
SADD #(64) SAdd0(a, b, d);
SADD #(64) SAdd1(a, c, e);
SSUB #(64) SSub2(a, b, f);
SCOMP #(64) SComp3(d, e, 0, 0, dEQe);
SCOMP #(64) SComp4(d, e, 0, dLTe, 0);
SMUX2x1 #(64) SMux5(d, e, dLTe, g);
SMUX2x1 #(64) SMux6(g, f, dEQe, h);
SREG #(64) SReg7(g, 0, 0, greg);
SREG #(64) SReg8(h, 0, 0, hreg);
SSHL #(64) SShl9(hreg, dLTe, xrin);
SSHR #(64) SShr10(greg, dEQe, zrin);
SREG #(32) SReg11(xrin[31:0], 0, 0, x);
SREG #(32) SReg12(zrin[31:0], 0, 0, z);
endmodule