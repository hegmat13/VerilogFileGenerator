`timescale 1ns / 1ns
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/08/2018 11:37:05 AM
// Design Name: 
// Module Name: COMP
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


module COMP #(parameter DATAWIDTH = 64)(a,b,gt,lt,eq);

	input [DATAWIDTH - 1:0] a;
	input [DATAWIDTH - 1:0] b;
	output reg gt;
	output reg lt;
	output reg eq;
always @(a,b) begin
		gt <= a>b;
		lt <= a<b;
		eq <= a==b; 

end
endmodule

