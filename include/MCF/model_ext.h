#ifndef _MCF_MODEL_EXTENSION_H_
#define _MCF_MODEL_EXTENSION_H_

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#include "core.h"

typedef enum mcfBlockType {
    MCF_BLOCK_TYPE_NONE = 0,
    MCF_BLOCK_TYPE_VERTEX,
    MCF_BLOCK_TYPE_INDEX,
    MCF_BLOCK_TYPE_MAX
} mcfBlockType;

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_MCF_MODEL_EXTENSION_H_
