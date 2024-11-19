#include <MCF/core.h>
#include "internal.h"
#include <stdint.h>
#include <stddef.h>

_mcfHeader _mcf_create_header(size_t block_count);

void _mcf_free_header(_mcfHeader* const header);

