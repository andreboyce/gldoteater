#ifndef __ENET_HOST_H
#define __ENET_HOST_H

#ifdef __cplusplus
extern "C"
{
#endif

#define ENET_BUILDING_LIB 1
#include "memory.h"
#include "enet.h"
#include "list.h"

void
enet_host_bandwidth_throttle (ENetHost * host);

void
enet_host_bandwidth_limit (ENetHost * host, enet_uint32 incomingBandwidth, enet_uint32 outgoingBandwidth);

void
enet_host_broadcast (ENetHost * host, enet_uint8 channelID, ENetPacket * packet);

ENetPeer *
enet_host_connect (ENetHost * host, const ENetAddress * address, size_t channelCount);

void
enet_host_destroy (ENetHost * host);

ENetHost *
enet_host_create (const ENetAddress * address, size_t peerCount, enet_uint32 incomingBandwidth, enet_uint32 outgoingBandwidth);

#ifdef __cplusplus
}
#endif

#endif

