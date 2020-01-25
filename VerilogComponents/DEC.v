`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/08/2018 11:43:30 AM
// Design Name: 
// Module Name: DEC
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


module DEC #(parameter DATAWIDTH = 64)(a,d);

	input [DATAWIDTH - 1:0] a;
	output reg [DATAWIDTH - 1:0] d;
always @(a) begin
		d <= a-1; 
end
endmodule
