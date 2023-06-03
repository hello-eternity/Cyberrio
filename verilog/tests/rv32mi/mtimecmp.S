# See LICENSE for license details.

#*****************************************************************************
# mtimecmp.S
#-----------------------------------------------------------------------------
#
# Test the mtimecmp csr and timer interupt
#

#include "riscv_test.h"
#include "test_macros.h"

# The following are non standard/custom CSRs (The spec assumes they sould be memory mapped)
#define mtime 0xb01
#define mtimecmp 0xbc0

RVTEST_RV32M
RVTEST_CODE_BEGIN

  # This assumes the current mtime fits into one word
  csrr t0, mtime
  addi t0, t0, 1000
  csrw mtimecmp, t0
  li t1, (1 << 7) # Enable timer interupts
  csrrs zero, mie, t1
  li t1, (1 << 3) # Enable global interupts
  csrrs zero, mstatus, t1
1:
  wfi
  j 1b

  TEST_PASSFAIL

mtvec_handler:
  # Make sure CAUSE indicates a timer interrupt
  csrr t1, mcause
  li t2, 0x80000007
  bne t1, t2, fail
  # Make sure time is greater or equal to the set comparison value
  csrr t1, mtime
  blt t1, t0, fail
  j pass

RVTEST_CODE_END

  .data
RVTEST_DATA_BEGIN

  TEST_DATA

RVTEST_DATA_END
