`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/14/2018 08:32:44 PM
// Design Name: 
// Module Name: MUL
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


module SMUL #(parameter DATAWIDTH = 64)(a,b,prod);

	input signed [DATAWIDTH - 1:0] a;
	input signed [DATAWIDTH - 1:0] b;
	output reg signed [DATAWIDTH - 1:0] prod;
	
    always @(a,b) begin
		prod <= a*b; 
end    
endmodule
