#ifndef _MCF_TYPE_EXT_H_
#define _MCF_TYPE_EXT_H_

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#include "core.h"
#include <stdint.h>
#include <stddef.h>

#define _MCF_INLINE __attribute__((always_inline)) inline

#define _MCF_FULL_BUFLAYOUT_FOR(_CT) \
	_MCF_INLINE mcfBufferLayout MCF_##_CT##_LAYOUT(uint32_t count) { return (mcfBufferLayout){.element_count=count, .component_count=1, .component_type=MCF_COMPONENT_TYPE_##_CT}; } \
	_MCF_INLINE mcfBufferLayout MCF_##_CT##_VEC2_LAYOUT(uint32_t count) { return (mcfBufferLayout){.element_count=count, .component_count=2, .component_type=MCF_COMPONENT_TYPE_##_CT}; }	\
	_MCF_INLINE mcfBufferLayout MCF_##_CT##_VEC3_LAYOUT(uint32_t count) { return (mcfBufferLayout){.element_count=count, .component_count=3, .component_type=MCF_COMPONENT_TYPE_##_CT}; }	\
	_MCF_INLINE mcfBufferLayout MCF_##_CT##_VEC4_LAYOUT(uint32_t count) { return (mcfBufferLayout){.element_count=count, .component_count=4, .component_type=MCF_COMPONENT_TYPE_##_CT}; }

_MCF_INLINE mcfBufferLayout MCF_CHAR_LAYOUT(uint32_t count) {
	return (mcfBufferLayout){.element_count=count, .component_count=1, .component_type=MCF_COMPONENT_TYPE_CHAR};
}

_MCF_FULL_BUFLAYOUT_FOR(I8)
_MCF_FULL_BUFLAYOUT_FOR(I16)
_MCF_FULL_BUFLAYOUT_FOR(I32)
_MCF_FULL_BUFLAYOUT_FOR(I64)
_MCF_FULL_BUFLAYOUT_FOR(U8)
_MCF_FULL_BUFLAYOUT_FOR(U16)
_MCF_FULL_BUFLAYOUT_FOR(U32)
_MCF_FULL_BUFLAYOUT_FOR(U64)
_MCF_FULL_BUFLAYOUT_FOR(FLOAT)
_MCF_FULL_BUFLAYOUT_FOR(DOUBLE)

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_MCF_TYPE_EXT_H_
