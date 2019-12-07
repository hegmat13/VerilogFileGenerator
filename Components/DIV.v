`timescale 1ns / 1ns
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/08/2018 11:43:30 AM
// Design Name: 
// Module Name: DIV
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

module DIV  #( parameter DATAWIDTH = 64)(a, b, quo);
    input [DATAWIDTH - 1 : 0] a;
    input [DATAWIDTH - 1 : 0] b;
    
    output reg [DATAWIDTH - 1 : 0] quo;
    
    always @(a,b) begin
        quo <= a / b;
    end
endmodule
