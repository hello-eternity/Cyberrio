
// These are the ALU values also used in the ISA
localparam ALU_ADD_SUB = 3'b000;
localparam ALU_SLL     = 3'b001;
localparam ALU_SLT     = 3'b010;
localparam ALU_SLTU    = 3'b011;
localparam ALU_XOR     = 3'b100;
localparam ALU_SRL_SRA = 3'b101;
localparam ALU_OR      = 3'b110;
localparam ALU_AND_CLR = 3'b111;

localparam ALU_SEL_REG = 2'b00;
localparam ALU_SEL_IMM = 2'b01;
localparam ALU_SEL_PC  = 2'b10;
localparam ALU_SEL_CSR = 2'b11;

localparam CMP_EQ  = 3'b000;
localparam CMP_NE  = 3'b001;
localparam CMP_LT  = 3'b110;
localparam CMP_GE  = 3'b111;
localparam CMP_LTU = 3'b100;
localparam CMP_GEU = 3'b101;

localparam WRITE_SEL_ALU     = 2'b00;
localparam WRITE_SEL_CSR     = 2'b01;
localparam WRITE_SEL_LOAD    = 2'b10;
localparam WRITE_SEL_NEXT_PC = 2'b11;

