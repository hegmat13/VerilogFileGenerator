`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
//  input Int16 a, b, c, d, e, f, g, h
//  input Int8 sa
//
//  output Int16 avg
//
//  wire Int32 l00, l01, l02, l03, l10, l11, l2, l2div2, l2div4, l2div8
//
//  l00 = a + b
//  l01 = c + d
//  l02 = e + f
//  l03 = g + h
//  l10 = l00 + l01
//  l11 = l02 + l03
//  l2 = l10 + l11
//  l2div2 = l2 >> sa
//  l2div4 = l2div2 >> sa 
//  l2div8 = l2div4 >> sa
//  avg = l2div8
// 
//////////////////////////////////////////////////////////////////////////////////

module circuit3(a, b, c, d, e, f, g, h, sa, avg);

    input [15:0] a, b, c, d, e, f, g, h;
    input [7:0] sa;
    output reg [15:0] avg;
    
    wire [31:0] l00, l01, l02, l03, l10, l11, l2, l2div2, l2div4, l2div8;
    reg Clk;
    
    initial Clk = 0;
    always #10 Clk = ~Clk;
    
    ADD #(32) Add5({16'b0, a}, {16'b0, b}, l00);
    ADD #(32) Add6({16'b0, c}, {16'b0, d}, l01);
    ADD #(32) Add7({16'b0, e}, {16'b0, f}, l02);
    ADD #(32) Add8({16'b0, g}, {16'b0, h}, l03);
    ADD #(32) Add9(l00, l01, l10);
    ADD #(32) Add10(l02, l03, l11);
    ADD #(32) Add11(l10, l11, l2);
    SHR #(32) Shr2(l2, {24'b0, sa}, l2div2);
    SHR #(32) Shr3(l2div2, {24'b0, sa}, l2div4);
    SHR #(32) Shr4(l2div4, {24'b0, sa}, l2div8);
    REG #(32) Reg4(l2div8, Clk, 0, avg);

endmodule
