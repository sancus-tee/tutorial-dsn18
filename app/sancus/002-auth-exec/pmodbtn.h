#ifndef SM_MMIO_PMODBTN_H_INC
#define SM_MMIO_PMODBTN_H_INC

#include <stdint.h>
#include <sancus/sm_support.h>

extern struct SancusModule pmodbtn;

uint8_t SM_MMIO_ENTRY(pmodbtn) pmodbtn_poll(void);
void SM_MMIO_ENTRY(pmodbtn) pmodbtn_init(void);

#endif
