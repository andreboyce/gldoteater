#ifndef __ENET_PACKET_H__
#define __ENET_PACKET_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <string.h>
#define ENET_BUILDING_LIB 1
#include "memory.h"
#include "enet.h"

int
enet_packet_resize (ENetPacket * packet, size_t dataLength);

void
enet_packet_destroy (ENetPacket * packet);

ENetPacket *
enet_packet_create (const void * data, size_t dataLength, enet_uint32 flags);

#ifdef __cplusplus
}
#endif

#endif

