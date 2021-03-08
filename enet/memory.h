/** 
 @file  memory.h
 @brief ENet memory management
*/
#ifndef __ENET_MEMORY_H__
#define __ENET_MEMORY_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdlib.h>

/** @defgroup memory ENet internal memory management
    @{
    @ingroup private
*/
void * enet_malloc (size_t);
void * enet_realloc (void *, size_t);
void * enet_calloc (size_t, size_t);
void   enet_free (void *);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* __ENET_MEMORY_H__ */

