`timescale 1ns / 1ns
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/15/2018 09:55:39 AM
// Design Name: 
// Module Name: ADD
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


module ADD #( parameter DATAWIDTH = 32)(a, b, sum);
    input [DATAWIDTH - 1 : 0] a;
    input [DATAWIDTH - 1 : 0] b;
    
    output reg [DATAWIDTH - 1 : 0] sum;

    always @(a,b) begin
        sum <= a + b;
    end
    
endmodule
