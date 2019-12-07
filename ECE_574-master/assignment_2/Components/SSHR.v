`timescale 1ns / 1ns
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/15/2018 10:05:20 AM
// Design Name: 
// Module Name: SHR
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

module SSHR #(parameter DATAWIDTH = 64)(a, sh_amt, d);
	input signed [DATAWIDTH-1:0] a;
	input signed [DATAWIDTH-1:0] sh_amt;
	output reg signed [DATAWIDTH-1:0] d;
	
	always @(a, sh_amt) begin
		d <= a >>> sh_amt;
	end
endmodule