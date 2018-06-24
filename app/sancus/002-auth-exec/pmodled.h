#ifndef SM_MMIO_PMODLED_H_INC
#define SM_MMIO_PMODLED_H_INC

#include <stdint.h>
#include <sancus/sm_support.h>

extern struct SancusModule pmodled;

void SM_MMIO_ENTRY(pmodled) pmodled_actuate(uint8_t state);
void SM_MMIO_ENTRY(pmodled) pmodled_init(void);

#endif
