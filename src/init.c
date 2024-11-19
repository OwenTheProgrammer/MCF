#include "internal.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stdarg.h>

/* Converts a four character string into its 32 bit int form */
#define _MCF_FILEID(x) ((uint32_t)(x[0]|(x[1]<<8)|(x[2]<<16)|(x[3]<<24)))

static void* _default_malloc(size_t size, void* user) {
    return malloc(size);
}
static void* _default_realloc(void* mem, size_t new_size, void* user) {
    return realloc(mem, new_size);
}
static void _default_dealloc(void* mem, void* user) {
    return free(mem);
}

const uint32_t _MCF_FILE_ID = _MCF_FILEID("mcf\0");
const uint32_t _MCF_FILE_VERSION = 0;

static mcfErrorFunc _error_func = NULL;

/* Default allocator functions */
mcfAllocator _mcf_allocator = {
    .allocate = _default_malloc,
    .reallocate = _default_realloc,
    .deallocate = _default_dealloc,
    .user = NULL
};

void _mcf_log(const char* func, const char* format, ...) {
    //TODO: Update this function to use stack buffers instead of stdout
    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
}

void _mcf_error(const char* func, int code, const char* format, ...) {
    if(_error_func == NULL)
        return;
    
    char message[_MCF_LOG_SIZE];

    if(format) {
        //Write the error message with the given format
        va_list args;
        va_start(args, format);
        vsnprintf(message, sizeof(message), format, args);
        va_end(args);
    }
    else if(code < MCF_ERROR_TYPE_MAX) {
        //Or just use the standard error description table
        strcpy(message, _MCF_ERROR_TABLE[code]);
    }
    else {
        //The hell you just do my guy?
        snprintf(message, sizeof(message), "UNKNOWN: %i", code);
    }

    _error_func(code, func, message);
}

void* _mcf_calloc(size_t count, size_t stride) {
    if(count && stride) {
        if(count > SIZE_MAX / stride) {
            MCF_ERROR(MCF_ERROR_INVALID_ARG, "Allocation size out of range");
            return NULL;
        }

        void* block = _mcf_allocator.allocate(count * stride, _mcf_allocator.user);

        if(block) {
            return memset(block, 0, count * stride);
        }
        else {
            MCF_ERROR(MCF_ERROR_OUT_OF_MEMORY, NULL);
            return NULL;
        }
    }
    return NULL;
}

void* _mcf_realloc(void* mem, size_t new_size) {
    if(mem && new_size) {
        void* block = _mcf_allocator.reallocate(mem, new_size, _mcf_allocator.user);

        if(block) {
            return block;
        }
        else {
            MCF_ERROR(MCF_ERROR_OUT_OF_MEMORY, NULL);
            return NULL;
        }
    } 
    else if(mem) {
        _mcf_free(mem);
        return NULL;
    }
    else {
        return _mcf_calloc(1, new_size);
    }
}

void _mcf_free(void* mem) {
    if(mem)
        _mcf_allocator.deallocate(mem, _mcf_allocator.user);
}

MCFAPI void mcf_set_error_handler(mcfErrorFunc pfun) {
    _error_func = pfun;
    MCF_LOG("Error handler function esablished.");
}

MCFAPI void mcf_set_allocation_handlers(mcfAllocator allocators) {
    if(allocators.allocate && allocators.deallocate && allocators.reallocate) {
        _mcf_allocator = allocators;
    }
    else {
        MCF_ERROR(MCF_ERROR_INVALID_ARG, "Please define all allocator handlers");
    }
}

const char* _MCF_ERROR_TABLE[MCF_ERROR_TYPE_MAX] = {
    "NONE: No error was presented",
    "INVALID_ARG: One or more arguments were invalid",
    "OUT_OF_MEMORY: Memory allocation failed"
};
