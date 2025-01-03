#pragma once
#include <mcf/core.h>
#include <stdint.h>
#include <stddef.h>

#define _MCF_LOG_SIZE 1024
#define _MCF_RESTRICT __restrict__
#define _MCF_TYPEOF(x) typeof(x)
#define _MCF_UNUSED_PARAM(x) (void)x;

/* File magic/identifier code */
extern const uint32_t _MCF_FILE_ID;

/* Version of the file format */
extern const uint32_t _MCF_FILE_VERSION;

/* Global memory allocation functions used through the library */
extern mcfAllocator _mcf_allocator;

/* Default messages for error types */
extern const char* _MCF_ERROR_TABLE[MCF_ERROR_TYPE_MAX];

typedef struct _mcfHeader       _mcfHeader;
typedef struct _mcfBlock        _mcfBlock;
typedef struct _mcfModel        _mcfModel;

typedef struct _mcfDataBuffer   _mcfDataBuffer;

/* MCF file header preamble */
struct _mcfHeader {
	/* File identification code */
	uint32_t file_id;

	/* Version of the file format */
	uint32_t file_version;

	/* Optional unique asset identifier */
	uint32_t uid;

	/* Amount of blocks in this file */
	uint32_t block_count;

	/* Array of each blocks byte offset from the start of the file */
	uint32_t* block_offsets;
};

/* A managed block of memory */
struct _mcfDataBuffer {
	/* Size of the allocated buffer in bytes */
	size_t size;

	/* Amount of bytes used in the buffer */
	size_t used_space;

	/* Internal allocated buffer */
	char* memory;
};

struct _mcfBlock {
	/* The type of this block (application specific and defined externally) */
	uint32_t type;

	/* Describes how the block data is stored */
	mcfBufferLayout buffer_layout;

	/* The data of this block */
	_mcfDataBuffer buffer;
};

/* Defines an MCF models data */
struct _mcfModel {
	/* The header section of the model */
	_mcfHeader header;

	/* The list of blocks this model contains */
	_mcfBlock* block_list;
};

void _mcf_log(const char* func, const char* format, ...);
void _mcf_error(const char* func, int code, const char* format, ...);

#define MCF_LOG(fmt, ...) _mcf_log(__func__, fmt "\n", ##__VA_ARGS__);
#define MCF_ERROR(code, fmt, ...) _mcf_error(__func__, (int)(code), fmt, ##__VA_ARGS__);

/*!
 * @brief Allocates and zeros an array of `count` elements, each `stride` bytes wide
 * @param count Amount of array elements
 * @param stride Byte stride of each array element
 * @return Allocated block or `NULL` when allocation size is zero or an error occured
 */
void* _mcf_calloc(size_t count, size_t stride);

/*!
 * @brief Reallocates a block of memory
 * @param mem The block of memory to reallocate
 * @param new_size The new size of the block in bytes
 * @return Allocated block or `NULL` when the reallocation has zero size or an error occurs
 */
void* _mcf_realloc(void* mem, size_t new_size);

/*!
 * @brief Disposes of an allocated memory block
 * @param mem The block of memory to dispose of
 * @note `mem = NULL` will result in noop
 */
void _mcf_free(void* mem);

/*!
 * @brief Allocates and initializes a new data buffer
 * @param size Size of the data buffer in bytes
 * @param data Optional data to initialize the buffer with
 * @return The allocated data buffer
 * @note `size` will define the size of `data` in bytes, when `data != NULL`
 */
_mcfDataBuffer _mcf_create_data_buffer(size_t size, void* data);

/*!
 * @brief Sets a range of a data buffers memory
 * @param dst The destination data buffer
 * @param dst_offset Base pointer offset of the destinations memory buffer in bytes
 * @param src The source memory to write to the destination
 * @param src_size The size of the source memory in bytes
 * @param can_resize When true, the buffer will resize when an overflow occurs
 * @return `MCF_OK` on success and the error code when something fails
 * @note When `src = NULL` the defined range of `dst` will be zero initialized
 */
mcfErrorType _mcf_data_buffer_set(_mcfDataBuffer* _MCF_RESTRICT const dst, size_t dst_offset, void* _MCF_RESTRICT const src, size_t src_size, mcfBool can_resize);

/*!
 * @brief Appends a block of data onto the end of a data buffer
 * @param dst The destination data buffer
 * @param src The source memory to append with
 * @param src_size The size of the source in bytes
 * @param can_resize When true, the buffer will resize when an overflow occurs
 * @return `MCF_OK` on success and the error code when something fails
 */
mcfErrorType _mcf_data_buffer_append(_mcfDataBuffer* _MCF_RESTRICT const dst, void* _MCF_RESTRICT const src, size_t src_size, mcfBool can_resize);

/*!
 * @brief Combines the contents of `src` onto the end of `dst`, optionally disposing of `src`
 * @param dst The destination buffer `src` will merge with
 * @param src The source buffer that will merge into `dst`
 * @param can_resize When true, the `dst` buffer will resize if more room is needed
 * @param consume_src When true, the `src` buffer will be disposed of after being merged into `dst`
 * @return `MCF_OK` on success and the error code when something fails
 */
mcfErrorType _mcf_data_buffer_extend(_mcfDataBuffer* _MCF_RESTRICT const dst, _mcfDataBuffer* _MCF_RESTRICT const src, mcfBool can_resize, mcfBool consume_src);

/*!
 * @brief Get memory from a data buffer at an offset
 * @param buffer The data buffer to get data from
 * @param offset The base offset in bytes
 * @return Pointer to the offset address or `NULL` when an error occurs
 */
void* _mcf_data_buffer_get(_mcfDataBuffer* const buffer, size_t offset);

/*!
 * @brief Resizes an allocated data buffer
 * @param buffer The buffer to reallocate
 * @param new_size The new size of the buffer
 * @return `MCF_OK` on success and the error code when something fails
 */
mcfErrorType _mcf_data_buffer_resize(_mcfDataBuffer* const buffer, size_t new_size);

/*!
 * @brief Disposes of an allocated data buffers memory
 * @param buffer The data buffer to dispose of
 * @note `buffer = NULL` will be ignored
 */
void _mcf_free_data_buffer(_mcfDataBuffer* const buffer);

/*!
 * @brief Reads the contents of a file into a data buffer
 * @param file_path The system path of the file
 * @return The loaded file data or a blank buffer if an error occurs
 */
_mcfDataBuffer _mcf_file_read(const char* file_path);

/*!
 * @brief Write the contents of a data buffer to a file
 * @param buffer The buffer to export
 * @param file_path The system path of the file
 * @return `MCF_OK` on success and the error code when something fails
 */
mcfErrorType _mcf_file_write(_mcfDataBuffer buffer, const char* file_path);
