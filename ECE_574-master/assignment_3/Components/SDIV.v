module SDIV  #( parameter DATAWIDTH = 2)(a, b, quot);
    input signed [DATAWIDTH - 1 : 0] a;
    input signed [DATAWIDTH - 1 : 0] b;
    
    output reg signed [DATAWIDTH - 1 : 0] quot;
    
    always @(a,b) begin
        quot <= a / b;
    end
endmodule