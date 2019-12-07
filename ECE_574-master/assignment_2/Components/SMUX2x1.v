`timescale 1ns / 1ns
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/14/2018 09:32:27 AM
// Design Name: 
// Module Name: MUX2x1
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


module SMUX2x1 #(parameter DATAWIDTH=
//2
//8
//16
//32
64
) (a, b, sel, d);
    input signed [DATAWIDTH-1:0] a;
    input signed [DATAWIDTH-1:0] b;
    input sel;
    output reg signed [DATAWIDTH-1:0] d;
    
    always @(a, b, sel)
    begin
        if (sel == 1) begin
            d <= b;
        end else begin
            d <= a;
        end
    end
endmodule
