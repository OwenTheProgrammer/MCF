#include <MCF/core.h>
#include "internal.h"
#include <stdint.h>
#include <stddef.h>

_mcfHeader _mcf_create_header(size_t block_count);

size_t _mcf_get_buffer_layout_index(mcfBufferLayout buffer_layout, size_t element_index);

uint32_t _mcf_header_add_block(_mcfHeader* const header);

void _mcf_free_header(_mcfHeader* const header);

