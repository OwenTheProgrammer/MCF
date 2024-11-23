#pragma once
#include <MCF/core.h>
#include "internal.h"
#include "model.h"

_mcfDataBuffer _mcf_encode_header(_mcfHeader header);

_mcfDataBuffer _mcf_encode_block(_mcfBlock block);

_mcfDataBuffer _mcf_encode_model(_mcfModel model);

_mcfHeader _mcf_decode_header(char* const stream, size_t* const ptr);

_mcfBlock _mcf_decode_block(char* const stream, size_t* const ptr);
