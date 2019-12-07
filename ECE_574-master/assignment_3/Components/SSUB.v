`timescale 1ns / 1ns
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/08/2018 11:43:30 AM
// Design Name: 
// Module Name: SUB
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

module SSUB  #( parameter DATAWIDTH = 64)(a, b, diff);   
    input signed [DATAWIDTH - 1 : 0] a;
    input signed [DATAWIDTH - 1 : 0] b;
    
    output reg signed [DATAWIDTH - 1 : 0] diff;
    
    always @(a,b) begin
        diff <= a - b;
    end
