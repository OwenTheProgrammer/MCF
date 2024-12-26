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

typedef struct mcfVec2f		mcfVec2f;
typedef struct mcfVec3f		mcfVec3f;
typedef struct mcfVec4f		mcfVec4f;

struct mcfVec2f { float x, y; };
struct mcfVec3f { float x, y, z; };
struct mcfVec4f { float x, y, z, w; };

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_MCF_MODEL_EXTENSION_H_
