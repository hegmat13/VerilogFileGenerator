`timescale 1ns / 1ns
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/14/2018 09:32:27 AM
// Design Name: 
// Module Name: MOD
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


module SMOD #(parameter DATAWIDTH=
2
//8
//16
//32
//64
) (a, b, rem);
    input signed [DATAWIDTH-1:0] a;
    input signed [DATAWIDTH-1:0] b;
    output reg signed [DATAWIDTH-1:0] rem;
    
    always @(a, b)
    begin
        rem <= a % b;
    end
endmodule
