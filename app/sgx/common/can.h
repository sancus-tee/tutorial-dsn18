#ifndef CAN_H_INC
#define CAN_H_INC

#include <stdint.h>
#include <stddef.h>

#define CAN_PAYLOAD_SIZE    8
#define CAN_SID_SIZE        2

int can_open(char *ican);
int can_send(int can_socket, uint16_t id, uint8_t *buf, size_t len);
int can_recv(int can_socket, uint16_t *id, uint8_t *buf);
int can_close(int can_socket);

#endif
