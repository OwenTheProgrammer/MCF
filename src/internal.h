#pragma once
#include <MCF/core.h>
#include <stdint.h>
#include <stddef.h>

#define _MCF_LOG_SIZE 1024

/* File magic/identifier code */
extern const uint32_t _MCF_FILE_ID;

/* Version of the file format */
extern const uint32_t _MCF_FILE_VERSION;

/* Global memory allocation functions used through the library */
extern mcfAllocator _mcf_allocator;

/* Default messages for error types */
extern const char* _MCF_ERROR_TABLE[MCF_ERROR_TYPE_MAX];

typedef struct _mcfHeader   _mcfHeader;

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
