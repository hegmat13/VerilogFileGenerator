`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
//  input Int8 a, b, c
//
//  output Int8 z
//  output Int16 x
//
//  wire Int8 d, e
//  wire Int16 f, g
//  wire Int16 xwire
//
//  d = a + b
//  e = a + c
//  g = d > e
//  z = g ? d : e
//  f = a * c
//  xwire = f - d  
//  x = xwire
// 
//////////////////////////////////////////////////////////////////////////////////

module circuit1(a, b, c, z, x);

    input [7:0] a, b, c;
    output reg [7:0] z;
    output reg [15:0] x;
    
    wire [7:0] d, e;
    wire [15:0] f, g, xwire;
    reg Clk;
    
    initial Clk = 0;
    always #10 Clk = ~Clk;
    
    ADD #(8) Add1(a, b, d);
    ADD #(8) Add2(a, c, e);
    COMP #(16) Comp1({8'b0, d},{8'b0, e}, g, 0, 0);
    MUX2x1 #(16) Mux1({8'b0,d}, {8'b0, e}, g, z);
    MUL #(16) Mult1({8'b0, a}, {8'b0, c}, f);
    SUB #(16) Sub1(f, {8'b0, d}, xwire);
    REG #(16) Reg1(xwire, Clk, 0, x);

endmodule
