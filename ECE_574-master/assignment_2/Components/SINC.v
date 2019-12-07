`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/20/2018 08:30:32 AM
// Design Name: 
// Module Name: SINC
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


module SINC #(parameter DATAWIDTH=2)(a, d);
    input signed [DATAWIDTH-1:0] a;
    output reg signed [DATAWIDTH-1:0] d;
    
    always @(a) begin
        d <= a + 1;
    end
endmodule
