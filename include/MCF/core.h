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

	/* Amount of components each element has */
	uint32_t component_count;

	/* The type of an elements components */
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

/* === MODEL FUNCTIONS === */

/*!
 * @brief Creates a new MCF model
 * @param block_count The amount of blocks to initialize the model with
 * @return The created model
 */
MCFAPI mcfModel* mcf_create_model(uint32_t block_count, mcfBlock* const block_list);

/*!
 * @brief Adds a block to a models block list
 * @param model The model to add the block to
 * @param block The block to add
 * @return The index of the block in the models block list
 * @note The block list will extend to accomidate the new block if needed
 */
MCFAPI uint32_t mcf_model_add_block(mcfModel* const model, mcfBlock* const block);

/*!
 * @brief Retrieves a block from a models block list
 * @param model The model that contains the block
 * @param index The blocks index in the block list
 * @return The block at the index, `NULL` if the block was not found
 * @todo Add better functionality for indexing blocks that exist in a model
 */
MCFAPI mcfBlock* mcf_model_get_block(mcfModel* const model, uint32_t index);

/*!
 * @brief Returns the amount of blocks this model contains
 * @param model The model to evaluate
 * @return The amount of blocks in the models block list
 */
MCFAPI uint32_t mcf_model_get_block_count(mcfModel* const model);

/*!
 * @brief Imports an MCF model from a file
 * @param file_path The system filepath to load
 * @return The imported model file
 * @note The file path must contain the extension as well (for the time being)
 */
MCFAPI mcfModel* mcf_import_model(const char* file_path);

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

/* === BLOCK FUNCTIONS === */

/*!
 * @brief Creates a new block of data that holds resources
 * @param block_type The type of the block, defined using extensions or by you
 * @param buffer_layout The way the data is laid out within the buffer
 * @param src_buffer The source buffer data if available
 * @note `src_buffer` may be `NULL`
 * @return The newly created block
 */
MCFAPI mcfBlock* mcf_create_block(uint32_t block_type, mcfBufferLayout buffer_layout, void* const src_buffer);

/*!
 * @brief Sets a range of a blocks data
 * @param block The block that gets the data
 * @param dst_offset An offset from the destinations base pointer in bytes
 * @param src The source data buffer
 * @param src_size The size of the source in bytes
 * @return `MCF_OK` on success and the error code when something happens
 * @note When `src = NULL` the range will be set with zeros
 */
MCFAPI mcfErrorType mcf_block_set_data(mcfBlock* const block, size_t dst_offset, void* const src, size_t src_size);

/*!
 * @brief Calculates a pointer to the memory address of a blocks element
 * @param block The block to index into
 * @param index The index of the array element
 * @return The address of the element at the given index
 */
MCFAPI void* mcf_block_get_data(mcfBlock* const block, size_t index);

/*!
 * @brief Returns how many elements a block has stored
 * @param block The block to evaluate
 * @return The element count
 */
MCFAPI uint32_t mcf_block_get_element_count(mcfBlock* const block);

/*!
 * @brief Disposes of an MCF block
 * @param block The block to dispose
 */
MCFAPI void mcf_free_block(mcfBlock* const block);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_MCF_CORE_H_
