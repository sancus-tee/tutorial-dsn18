#include "can.h"
#include "debug.h"

#include <linux/can.h>
#include <unistd.h>
#include <string.h>
#include <net/if.h>

int can_open(char *ican)
{
    int can_socket;
	struct sockaddr_can can_addr;

    info("Creating Linux CAN socket for interface '%s'...", ican);
    ASSERT( (can_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW)) >= 0 );
    
    memset(&can_addr, 0, sizeof(can_addr));
    can_addr.can_family = AF_CAN;
    ASSERT( (can_addr.can_ifindex = if_nametoindex(ican)) );
    ASSERT( bind(can_socket, (struct sockaddr *)&can_addr, sizeof(can_addr)) >= 0 );

    return can_socket;
}

void can_dump_frame(char *msg, struct can_frame *frame)
{
    info("%s: CAN frame with ID 0x%x (len=%d)", msg, frame->can_id, frame->can_dlc);
    dump_hex("MSG   ", frame->data, frame->can_dlc);
}

int can_send(int can_socket, uint16_t id, uint8_t *buf, size_t len)
{
	struct can_frame frame;
    int i;
    ASSERT(len <= CAN_PAYLOAD_SIZE);

    frame.can_id  = id;
    frame.can_dlc = len;
    for (i=0; i < len; i++)
        frame.data[i] = buf[i];

    can_dump_frame("send", &frame);

    return write(can_socket, &frame, CAN_MTU);
}

int can_recv(int can_socket, uint16_t *id, uint8_t *buf)
{
    struct can_frame frame;
    int i;
    ASSERT( recv(can_socket, &frame, sizeof(frame), 0) == CAN_MTU );

    *id = frame.can_id;
    for (i=0; i < frame.can_dlc; i++)
        buf[i] = frame.data[i];

    can_dump_frame("recv", &frame);

    return frame.can_dlc; 
}

int can_close(int can_socket)
{
    return close(can_socket);
}
