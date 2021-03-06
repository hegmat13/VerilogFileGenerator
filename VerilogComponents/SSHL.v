`timescale 1ns / 1ns
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/15/2018 10:10:15 AM
// Design Name: 
// Module Name: SHL
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

module SSHL #(parameter DATAWIDTH = 64)(a, sh_amt, d);
	input signed [DATAWIDTH-1:0] a;
	input signed [DATAWIDTH-1:0] sh_amt;
	output reg signed [DATAWIDTH-1:0] d;
	
	always @(a, sh_amt) begin
		d <= a <<< sh_amt;
	end
endmodule