#ifndef __MCF_CORE_H__
#define __MCF_CORE_H__

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#include <stdint.h>
#include <stddef.h>

#define MCFAPI

#define MCF_OK		0
#define MCF_FAIL	1

typedef enum mcfBool {
	MCF_FALSE = 0,
	MCF_TRUE = 1
} mcfBool;

typedef enum mcfErrorType {
	MCF_ERROR_NONE = 0,
	MCF_ERROR_INVALID_ARG,
	MCF_ERROR_OUT_OF_MEMORY,
	MCF_ERROR_TYPE_MAX
} mcfErrorType;

typedef struct mcfAllocator		mcfAllocator;

/*!
 * @brief Error handler callback function pointer
 * @param error_code Error that occured
 * @param func Function name the error occured within
 * @param msg Error message in text for context on what happened
 */
typedef void (*mcfErrorFunc)(int error_code, const char* func, const char* msg);

/*!
 * @brief Memory allocation function pointer
 * @param size Amount of bytes to allocate
 * @param user User defined data present in all allocator stages
 * @return Allocated buffer or `NULL` when an error occurs
 * @note This will never invoke with `size = 0`
 */
typedef void* (*mcfAllocFunc)(size_t size, void* user);

/*!
 * @brief Memory reallocation function pointer
 * @param mem Block of memory to reallocate
 * @param new_size New size of `mem` in bytes
 * @param user User defined data present in all allocator stages
 * @return Reallocated buffer or `NULL` when an error occurs
 * @note This will never invoke with `mem = NULL` nor `new_size = 0`
 */
typedef void* (*mcfReallocFunc)(void* mem, size_t new_size, void* user);

/*!
 * @brief Memory disposal function pointer
 * @param mem Block of memory to dispose of
 * @param user User defined data present in all allocator stages
 * @note This will never invoke with `mem = NULL`
 */
typedef void (*mcfDeallocFunc)(void* mem, void* user);

/* Defines custom memory management functions */
struct mcfAllocator {
	/* Pointer to your memory allocation function */
	mcfAllocFunc allocate;

	/* Pointer to your memory reallocation function */
	mcfReallocFunc reallocate;

	/* Pointer to your memory deallocation function */
	mcfDeallocFunc deallocate;

	/* User defined data to be passed through each memory management callback on all invocations */
	void* user;
};

/*!
 * @brief Sets the global error handler function
 * @param pfun Pointer to the error handler function
 */
MCFAPI void mcf_set_error_handler(mcfErrorFunc pfun);

/*!
 * @brief Sets custom global memory allocation callbacks
 * @param allocators Group of allocation handler function pointers
 * @note All allocators must be defined
 */
MCFAPI void mcf_set_allocation_handlers(mcfAllocator allocators);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__MCF_CORE_H__
