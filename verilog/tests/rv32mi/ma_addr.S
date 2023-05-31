# See LICENSE for license details.

#*****************************************************************************
# ma_addr.S
#-----------------------------------------------------------------------------
#
# Test misaligned ld/st trap.
#

#include "riscv_test.h"
#include "test_macros.h"

RVTEST_RV32M
RVTEST_CODE_BEGIN

  .align 2
  .option norvc

  la s0, data

  # indicate it's a load test
  li s1, CAUSE_MISALIGNED_LOAD

#define SEXT(x, n) ((-((x) >> ((n)-1)) << (n)) | ((x) & ((1 << (n))-1)))

/* Check that a misaligned load either writes the correct value, or
   takes an exception and performs no writeback.  */
#define MISALIGNED_LOAD_TEST(testnum, insn, base, offset, res) \
  li TESTNUM, testnum; \
  la t2, 1f; \
  addi t1, base, offset; \
  insn t1, offset(base); \
  li t2, res; \
  bne t1, t2, fail; \
1:

  MISALIGNED_LOAD_TEST(2,  lh,  s0, 1, SEXT(0xbbcc, 16))
  MISALIGNED_LOAD_TEST(3,  lhu, s0, 1, 0xbbcc)
  MISALIGNED_LOAD_TEST(4,  lw,  s0, 1, SEXT(0x99aabbcc, 32))
  MISALIGNED_LOAD_TEST(5,  lw,  s0, 2, SEXT(0x8899aabb, 32))
  MISALIGNED_LOAD_TEST(6,  lw,  s0, 3, SEXT(0x778899aa, 32))

  # indicate it's a store test
  li s1, CAUSE_MISALIGNED_STORE

/* Check that a misaligned store has some effect and takes no exception,
   or takes no effect and generates an exception.  This is not very
   thorough.  */
#define MISALIGNED_STORE_TEST(testnum, insn, base, offset, size) \
  li TESTNUM, testnum; \
  la t2, 1f; \
  addi t1, base, offset; \
  insn x0, offset(base); \
  lb t1, (offset - 1)(base); \
  beqz t1, fail; \
  lb t1, (offset + size)(base); \
  beqz t1, fail; \
  lb t1, (offset + 0)(base); \
  bnez t1, fail; \
  lb t1, (offset + size - 1)(base); \
  bnez t1, fail; \
1:

  MISALIGNED_STORE_TEST(22,  sh,  s0, 1, 2)
  MISALIGNED_STORE_TEST(23,  sw,  s0, 5, 4)
  MISALIGNED_STORE_TEST(24,  sw,  s0, 10, 4)
  MISALIGNED_STORE_TEST(25,  sw,  s0, 15, 4)

  TEST_PASSFAIL

  .align 3
mtvec_handler:
  csrr t0, mcause
  bne t0, s1, fail

  csrr t0, mtval
  beqz t0, 1f # mtval can be hardcoded to zero
  bne t0, t1, fail
1:

  lb t0, (t0)
  beqz t0, fail

  csrw mepc, t2
  mret

RVTEST_CODE_END

  .data
RVTEST_DATA_BEGIN

data:
  .align 3
.word 0xaabbccdd
.word 0x66778899
.word 0x22334455
.word 0xeeffee11
.fill 0xff, 1, 80


  TEST_DATA

RVTEST_DATA_END
