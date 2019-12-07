`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
//  input Int32 a, b, c
//
//  output Int32 z, x
//
//  wire Int32 d, e, f, g, h
//  wire Int1 dLTe, dEQe  
//  wire Int32 zwire, xwire
//
//  d = a + b
//  e = a + c
//  f = a - b  
//  dEQe = d == e
//  dLTe = d < e
//  g = dLTe ? d : e 
//  h = dEQe ? g : f 
//  xwire = g << dLTe
//  zwire = h >> dEQe
//  x = xwire
//  z = zwire
// 
//////////////////////////////////////////////////////////////////////////////////

module circuit2(a, b, c, z, x);
    input [31:0] a, b, c;
    output [31:0] z, x;
    
    wire [31:0] d, e, f, g, h, zwire, xwire;
    wire dLTe, dEQe;
    reg Clk; 
    
    initial Clk = 0; 
    always #10 Clk = ~Clk; 

    ADD #(32) Add3(a, b, d);
    ADD #(32) Add4(a, c, e);
    SUB #(32) Sub2(a, b, f);
    COMP #(32) Comp2(d, e, 0, 0, dEQe);
    COMP #(32) Comp3(d, e, 0, dLTe, 0);
    MUX2x1 #(32) Mux2(d, e, dLTe, g);
    MUX2x1 #(32) Mux3(g, f, dEQe, h);
    SHL #(32) Shl1(g, dLTe, xwire);
    SHR #(32) Shr1(h,  dEQe, zwire);
    REG #(32) Reg2(xwire, Clk, 0, x);
    REG #(32) Reg3(zwire, Clk, 0, z);

endmodule
