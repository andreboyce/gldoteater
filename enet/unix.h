/** 
 @file  unix.h
 @brief ENet Unix header
*/
#ifndef __ENET_UNIX_H__
#define __ENET_UNIX_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>

typedef int ENetSocket;

enum
{
    ENET_SOCKET_NULL = -1
};

#define ENET_HOST_TO_NET_16(value) (htons (value)) /**< macro that converts host to net byte-order of a 16-bit value */
#define ENET_HOST_TO_NET_32(value) (htonl (value)) /**< macro that converts host to net byte-order of a 32-bit value */

#define ENET_NET_TO_HOST_16(value) (ntohs (value)) /**< macro that converts net to host byte-order of a 16-bit value */
#define ENET_NET_TO_HOST_32(value) (ntohl (value)) /**< macro that converts net to host byte-order of a 32-bit value */

typedef struct
{
    void * data;
    size_t dataLength;
} ENetBuffer;

#ifdef __cplusplus
}
#endif

#endif /* __ENET_UNIX_H__ */

