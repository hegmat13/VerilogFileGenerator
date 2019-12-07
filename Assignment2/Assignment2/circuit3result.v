`timescale 1ns / 1ps
//////////////////////////////////////////////////////////
module circuitN(a, b, c, d, e, f, g, h, sa, avg);
input signed [15:0] a;
input signed [15:0] b;
input signed [15:0] c;
input signed [15:0] d;
input signed [15:0] e;
input signed [15:0] f;
input signed [15:0] g;
input signed [15:0] h;
input signed [7:0] sa;
output signed [15:0] avg;
wire signed [31:0] l00;
wire signed [31:0] l01;
wire signed [31:0] l02;
wire signed [31:0] l03;
wire signed [31:0] l10;
wire signed [31:0] l11;
wire signed [31:0] l2;
wire signed [31:0] l2div2;
wire signed [31:0] l2div4;
wire signed [31:0] l2div8;
SADD #(32) SAdd0({{16{a[15]}}, a}, {{16{b[15]}}, b}, l00);
SADD #(32) SAdd1({{16{c[15]}}, c}, {{16{d[15]}}, d}, l01);
SADD #(32) SAdd2({{16{e[15]}}, e}, {{16{f[15]}}, f}, l02);
SADD #(32) SAdd3({{16{g[15]}}, g}, {{16{h[15]}}, h}, l03);
SADD #(32) SAdd4(l00, l01, l10);
SADD #(32) SAdd5(l02, l03, l11);
SADD #(32) SAdd6(l10, l11, l2);
SSHR #(32) SShr7(l2, {{24{sa[23]}}, sa}, l2div2);
SSHR #(32) SShr8(l2div2, {{24{sa[23]}}, sa}, l2div4);
SSHR #(32) SShr9(l2div4, {{24{sa[23]}}, sa}, l2div8);
SREG #(16) SReg10(l2div8[15:0], 0, 0, avg);
endmodule