#ifndef _MCF_CORE_H_
#define _MCF_CORE_H_

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#include <stdint.h>
#include <stddef.h>

#define MCFAPI

#define MCF_FALSE	0
#define MCF_TRUE	1

typedef enum mcfErrorType {
	MCF_OK = 0,
	MCF_ERROR_INVALID_ARG,
	MCF_ERROR_OUT_OF_MEMORY,
	MCF_ERROR_NULL_TYPE,
	MCF_ERROR_TYPE_UNINITIALIZED,
	MCF_ERROR_OVERFLOW,
	MCF_ERROR_FILE_IO,
	MCF_ERROR_TYPE_MAX
} mcfErrorType;

typedef enum mcfComponentType {
	MCF_COMPONENT_TYPE_CHAR,
	MCF_COMPONENT_TYPE_U8,
	MCF_COMPONENT_TYPE_U16,
	MCF_COMPONENT_TYPE_U32,
	MCF_COMPONENT_TYPE_U64,
	MCF_COMPONENT_TYPE_I8,
	MCF_COMPONENT_TYPE_I16,
	MCF_COMPONENT_TYPE_I32,
	MCF_COMPONENT_TYPE_I64,
	MCF_COMPONENT_TYPE_FLOAT,
	MCF_COMPONENT_TYPE_DOUBLE,
	MCF_COMPONENT_TYPE_MAX
} mcfComponentType;

typedef int mcfBool;
typedef int mcfResult;

typedef struct mcfAllocator		mcfAllocator;
typedef struct mcfBufferLayout	mcfBufferLayout;

typedef struct mcfBlock		mcfBlock;
typedef struct mcfModel		mcfModel;

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

/* Defines the memory layout of an MCF buffer */
struct mcfBufferLayout {
	/* Amount of elements in the array */
	uint32_t element_count;

	/* Amount of components each array element has */
	uint32_t component_count;

	/* The type of each component */
	mcfComponentType component_type;
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


/*!
 * @brief Creates a new MCF model
 * @param block_count The amount of blocks to initialize the model with
 * @return The created model
 */
MCFAPI mcfModel* mcf_create_model(uint32_t block_count);

MCFAPI mcfBlock* mcf_create_block(uint32_t block_type, mcfBufferLayout buffer_layout, void* const src_buffer);

MCFAPI mcfErrorType mcf_block_set_data(mcfBlock* const block, size_t dst_offset, void* const src, size_t src_size);

MCFAPI void* mcf_block_get_data(mcfBlock* const block, size_t index);

MCFAPI uint32_t mcf_model_add_block(mcfModel* const model, mcfBlock* const block);

/*!
 * @brief Exports an MCF model to a file
 * @param model The model to export
 * @param file_path The destination file name
 * @return `MCF_OK` on success, the error code on failure
 * @note For the time being, `file_path` has to include the extension
 */
MCFAPI mcfErrorType mcf_export_model(mcfModel* const model, const char* file_path);

/*!
 * @brief Disposes of an MCF model
 * @param model The model to dispose
 */
MCFAPI void mcf_free_model(mcfModel* const model);

/*!
 * @brief Disposes of an MCF block
 * @param block The block to dispose
 */
MCFAPI void mcf_free_block(mcfBlock* const block);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_MCF_CORE_H_
