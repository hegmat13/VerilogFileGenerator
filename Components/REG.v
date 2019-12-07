`timescale 1ns / 1ns
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/14/2018 09:32:27 AM
// Design Name: 
// Module Name: REG
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


module REG #(parameter DATAWIDTH=
2
//8
//16
//32
//64
) (d, Clk, Rst, q);
    input [DATAWIDTH-1:0] d;
    input Clk;
    input Rst;
    output reg [DATAWIDTH-1:0] q;
    
    always @(posedge Clk)
    begin
        if (Rst) begin
            q <= 0;
        end else begin
            q <= d;
        end
    end
endmodule
