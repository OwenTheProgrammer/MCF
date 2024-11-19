#pragma once
#include <MCF/core.h>
#include "internal.h"

_mcfHeader _mcf_alloc_header(size_t block_count);

void _mcf_free_header(_mcfHeader* const header);

_mcfDataBuffer _mcf_encode_header(_mcfHeader* const header);
