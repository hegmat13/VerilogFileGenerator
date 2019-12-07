`timescale 1ns / 1ns
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/22/2018 11:25:45 AM
// Design Name: 
// Module Name: circuit5
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


module circuit5( a, b, c, d, zero, Clk, Rst, z );

    input Clk, Rst; 
    
    parameter Int64 = 64;
    input [Int64-1:0] a, b, c, d, zero;

    output [Int64-1:0] z;

    wire [Int64-1:0] e, f, g, zwire;
    wire gEQz;  

    DIV    #(.DATAWIDTH(Int64)) div_1( a, b, e); //e = a / b
    DIV    #(.DATAWIDTH(Int64)) div_2( c, d, f); //f = c / d
    MOD    #(.DATAWIDTH(Int64)) mod_1( a, b, g); //g = a % b  
    COMP   #(.DATAWIDTH(Int64)) comp_1( g, zero, 0, 0, gEQz); //gEQz = g == zero
    MUX2x1 #(.DATAWIDTH(Int64)) mux2x1_1(e, f, gEQz, zwire); //zwire = gEQz ? e : f 
    REG    #(.DATAWIDTH(Int64)) reg_1(zwire, Clk, Rst, z); //z = zwire
    
endmodule
