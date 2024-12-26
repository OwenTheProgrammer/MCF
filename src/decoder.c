#include <mcf/core.h>
#include "internal.h"
#include "model.h"
#include "footprint.h"
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define DECODE_BEGIN(_STREAM, _PTR)			\
	size_t _decode_ptr = _PTR ? *_PTR : 0;	\
	char* _decode_buf = _STREAM;

#define DECODE_PARAM(_PARAM)										\
	_PARAM = *(_MCF_TYPEOF(_PARAM)*)(_decode_buf + _decode_ptr);	\
	_decode_ptr += sizeof(_MCF_TYPEOF(_PARAM));

#define DECODE_PARAM_AS(_PARAM, _TYPE)				\
	_PARAM = *(_TYPE*)(_decode_buf + _decode_ptr);	\
	_decode_ptr += sizeof(_TYPE);

#define ARRAY_ALLOC_FOR(_PARAM, _COUNT)	\
	_PARAM = _mcf_calloc(_COUNT, sizeof(*_PARAM));

#define DECODE_ARRAY_DIRECT(_PARAM, _COUNT)									\
	memcpy(_PARAM, _decode_buf + _decode_ptr, _COUNT * sizeof(*_PARAM));	\
	_decode_ptr += _COUNT * sizeof(*_PARAM);

#define DECODE_ARRAY(_PARAM, _COUNT)	\
	ARRAY_ALLOC_FOR(_PARAM, _COUNT)		\
	DECODE_ARRAY_DIRECT(_PARAM, _COUNT)

#define DECODE_DATA_BUFFER(_PARAM, _LAYOUT)	\
	_PARAM = _mcf_create_data_buffer(_mcf_get_buffer_layout_footprint(_LAYOUT) * sizeof(char), _decode_buf+_decode_ptr);

#define DECODE_TELL(_OUTPUT)	\
	if(_OUTPUT)					\
		*_OUTPUT = _decode_ptr;

static _mcfHeader _mcf_decode_header(char* _MCF_RESTRICT const stream, size_t* _MCF_RESTRICT const ptr) {
	_mcfHeader result = (_mcfHeader){0};

	DECODE_BEGIN(stream, ptr)

	DECODE_PARAM(result.file_id)
	DECODE_PARAM(result.file_version)
	DECODE_PARAM(result.uid)
	DECODE_PARAM(result.block_count)
	DECODE_ARRAY(result.block_offsets, result.block_count)

	DECODE_TELL(ptr)

	return result;
}

static _mcfBlock _mcf_decode_block(char* _MCF_RESTRICT const stream, size_t* _MCF_RESTRICT const ptr) {
	_mcfBlock result = (_mcfBlock){0};

	DECODE_BEGIN(stream, ptr)

	DECODE_PARAM(result.type)
	DECODE_PARAM(result.buffer_layout.element_count)
	DECODE_PARAM(result.buffer_layout.component_count)
	DECODE_PARAM_AS(result.buffer_layout.component_type, uint32_t)
	DECODE_DATA_BUFFER(result.buffer, result.buffer_layout)

	DECODE_TELL(ptr)

	return result;
}

MCFAPI mcfModel* mcf_import_model(const char* file_path) {
	_mcfModel* model = _mcf_calloc(1, sizeof(_mcfModel));
	_mcfDataBuffer file_data = _mcf_file_read(file_path);

	size_t header_size = 0;
	model->header = _mcf_decode_header(file_data.memory, &header_size);

	MCF_LOG("Loaded %zu bytes of header", header_size);
	MCF_LOG("Block Count: %u", model->header.block_count);

	model->block_list = _mcf_calloc(model->header.block_count, sizeof(_mcfBlock));

	for(size_t i = 0; i < model->header.block_count; i++) {
		size_t ptr = header_size + model->header.block_offsets[i];
		model->block_list[i] = _mcf_decode_block(file_data.memory, &ptr);
	}

	_mcf_free_data_buffer(&file_data);

	return (mcfModel*)model;
}