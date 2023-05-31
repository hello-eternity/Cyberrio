module cmp (
    input clk,
    input [31:0] input_a,
    input [31:0] input_b,
    input [2:0] function_select,
    output reg result
);

    reg negate;
    reg quasi_result;
    wire usign = function_select[1];
    wire less = function_select[2];
    wire is_equal = (input_a == input_b);
    wire is_less = usign ? (input_a < input_b) : ($signed(input_a) < $signed(input_b));

    always @(posedge clk) begin
        negate <= function_select[0];
        quasi_result <= (less ? is_less : is_equal);
    end

    assign result = negate ? ~quasi_result : quasi_result;
endmodule
