`timescale 1ns / 1ps
//////////////////////////////////////////////////////////
module circuitN(a, b, c, z, x);
input signed [7:0] a;
input signed [7:0] b;
input signed [7:0] c;
output signed [7:0] z;
output signed [15:0] x;
wire signed [7:0] d;
wire signed [7:0] e;
wire signed [15:0] f;
wire signed [15:0] g;
wire signed [15:0] xwire;
SADD #(8) SAdd0(a, b, d);
SADD #(8) SAdd1(a, c, e);
SCOMP #(8) SComp2(d, e, g, 0, 0);
SMUX2x1 #(8) SMux3(d, e, g, z);
SMUL #(32) SMult4({{8{a[7]}}, a}, {{8{c[7]}}, c}, f);
SSUB #(16) SSub5(f, {{8{d[7]}}, d}, xwire);
SREG #(16) SReg6(xwire, 0, 0, x);
endmodule