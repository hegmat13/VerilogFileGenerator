`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
//  input Int64 a, b, c
//
//  output Int32 z, x
//
//  wire Int64 d, e, f, g, h 
//  wire Int1 dLTe, dEQe 
//  wire Int64 xrin, zrin
//  register Int64 greg, hreg
//
//  d = a + b
//  e = a + c
//  f = a - b  
//  dEQe = d == e
//  dLTe = d < e
//  g = dLTe ? d : e 
//  h = dEQe ? g : f 
//  greg = g
//  hreg = h
//  xrin = hreg << dLTe
//  zrin = greg >> dEQe
//  x = xrin
//  z = zrin
// 
//////////////////////////////////////////////////////////////////////////////////

module circuit4(a, b, c, Clk, Rst, z, x);

    input [63:0] a, b, c;
    input Clk, Rst;
    output [31:0] z, x;

    wire [63:0] d, e, f, g, h, xrin, zrin;
    wire dLTe, dEQe;
    wire gt, lt, eq;
    wire [63:0] greg, hreg;

    
    ADD #(64) Add12(a, b, d);
    ADD #(64) Add13(a, c, e);
    SUB #(64) Sub3(a, b, f);
    COMP #(64) Comp4(d, e, gt, lt, dEQe);
    COMP #(64) Comp5(d, e, gt, dLTe, eq);
    MUX2x1 #(64) Mux4(d, e, dLTe, g);
    MUX2x1 #(64) Mux5(g, f, dEQe, h);
    REG #(64) Reg5(g, Clk, 0, greg);
    REG #(64) Reg6(h, Clk, 0, hreg);
    SHL #(64) Shl2(hreg, dLTe, xrin);
    SHR #(64) Shr5(greg, dEQe, zrin);
    REG #(64) Reg7(xrin, Clk, 0, x);
    REG #(64) Reg8(zrin, Clk, 0, z);

endmodule
