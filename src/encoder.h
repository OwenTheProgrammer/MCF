#pragma once
#include <MCF/core.h>
#include "internal.h"
#include "model.h"

_mcfDataBuffer _mcf_encode_header(_mcfHeader* const header);

_mcfDataBuffer _mcf_encode_model(_mcfModel* const model);
