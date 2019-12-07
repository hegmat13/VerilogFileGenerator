`timescale 1ns / 1ns
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/15/2018 09:55:39 AM
// Design Name: 
// Module Name: add
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


module SADD #( parameter DATAWIDTH = 2)(a, b, sum);
    input signed [DATAWIDTH - 1 : 0] a;
    input signed [DATAWIDTH - 1 : 0] b;
    
    output reg signed [DATAWIDTH - 1 : 0] sum;

    always @(a,b) begin
        sum <= a + b;
    end
    
endmodule
