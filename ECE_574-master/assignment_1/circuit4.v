`timescale 1ns / 1ns
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/22/2018 08:15:37 PM
// Design Name: 
// Module Name: circuit4
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module circuit4(a, b, c, Clk, Rst, z, x);
    parameter Int64=64;
    parameter Int32=32;
    input [Int64-1:0] a;
    input [Int64-1:0] b;
    input [Int64-1:0] c;
    input Clk, Rst;
    output [Int32-1:0] z;
    output [Int32-1:0] x;
    
    wire [Int64-1:0] d, e, f, g, h;
    wire dLTe, dEQe;
    wire [Int64-1:0] xrin, zrin;
    wire [Int64-1:0] greg, hreg;
    
    ADD #(Int64) add_1(.a(a), .b(b), .sum(d));
    ADD #(Int64) add_2(.a(a), .b(c), .sum(e));
    SUB #(Int64) sub_1(.a(a), .b(b), .diff(f));
    COMP #(Int64) comp_1(.a(d), .b(e), .gt(), .lt(dLTe), .eq(dEQe));
    MUX2x1 #(Int64) MUX2x1_1(.a(e), .b(d), .sel(dLTe), .d(g));
    MUX2x1 #(Int64) MUX2x1_2(.a(f), .b(g), .sel(dEQe), .d(h));
    REG #(Int64) REG_1(.d(g), .Clk(Clk), .Rst(Rst), .q(greg));
    REG #(Int64) REG_2(.d(h), .Clk(Clk), .Rst(Rst), .q(hreg));
    SHL #(Int64) SHL_1(.a(hreg), .sh_amt(dLTe), .d(xrin));
    SHR #(Int64) SHR_1(.a(greg), .sh_amt(dEQe), .d(zrin));
    REG #(Int64) REG_3(.d(xrin[Int32-1:0]), .Clk(Clk), .Rst(Rst), .q(x));
    REG #(Int64) REG_4(.d(zrin[Int32-1:0]), .Clk(Clk), .Rst(Rst), .q(z));
endmodule
