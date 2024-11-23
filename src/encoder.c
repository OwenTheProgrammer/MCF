#include "codec.h"
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "footprint.h"

#define ENCODE_BEGIN(_size)												\
	_mcfDataBuffer _encode_buf = _mcf_create_data_buffer(_size, NULL);	\
	size_t _encode_ptr = 0;

#define ENCODE_PARAM(_param)											\
	memcpy(_encode_buf.memory + _encode_ptr, &_param, sizeof(_param));	\
	_encode_buf.used_space += sizeof(_param);							\
	_encode_ptr += sizeof(_param);

#define ENCODE_PARAM_AS(_param, _type)											\
	memcpy(_encode_buf.memory + _encode_ptr, (_type*)&_param, sizeof(_type));	\
	_encode_buf.used_space += sizeof(_type);									\
	_encode_ptr += sizeof(_type);

#define ENCODE_ARRAY(_array, _count)														\
	_mcf_data_buffer_append(&_encode_buf, _array, (_count) * sizeof(*_array), MCF_TRUE);	\
	_encode_ptr += (_count) * sizeof(*_array);

#define ENCODE_END() _encode_buf;

_mcfDataBuffer _mcf_encode_header(_mcfHeader header) {
	MCF_LOG("Encoding header");
	size_t header_size = _mcf_get_header_footprint(header);
	
	ENCODE_BEGIN(header_size)

	ENCODE_PARAM(header.file_id)
	ENCODE_PARAM(header.file_version)
	ENCODE_PARAM(header.uid)
	ENCODE_PARAM(header.block_count)
	ENCODE_ARRAY(header.block_offsets, header.block_count)

	return ENCODE_END()
}

_mcfDataBuffer _mcf_encode_block(_mcfBlock block) {
	MCF_LOG("Encode block");
	size_t block_size = _mcf_get_block_footprint(block);

	ENCODE_BEGIN(block_size)

	ENCODE_PARAM(block.type)
	ENCODE_PARAM(block.buffer_layout.element_count)
	ENCODE_PARAM(block.buffer_layout.component_count)
	ENCODE_PARAM_AS(block.buffer_layout.component_type, uint32_t)
	ENCODE_ARRAY(block.buffer.memory, block.buffer.size)

	return ENCODE_END()
}

_mcfDataBuffer _mcf_encode_model(_mcfModel model) {
	MCF_LOG("Encoding model");

	_mcfDataBuffer encode_buffer = _mcf_create_data_buffer(0, NULL);
	for(uint32_t i = 0; i < model.header.block_count; i++) {
		model.header.block_offsets[i] = encode_buffer.used_space;

		_mcfDataBuffer block_enc = _mcf_encode_block(model.block_list[i]);
		_mcf_data_buffer_extend(&encode_buffer, &block_enc, MCF_TRUE, MCF_TRUE);
	}

	_mcfDataBuffer final_buffer = _mcf_encode_header(model.header);
	_mcf_data_buffer_extend(&final_buffer, &encode_buffer, MCF_TRUE, MCF_TRUE);
	
	return final_buffer;
}

MCFAPI mcfErrorType mcf_export_model(mcfModel* const model, const char* file_path) {
	if(model == NULL) {
		MCF_ERROR(MCF_ERROR_NULL_TYPE, "Model can not be null");
		return MCF_ERROR_NULL_TYPE;
	}

	_mcfModel _model = *(_mcfModel*)model;

	_mcfDataBuffer model_enc = _mcf_encode_model(_model);

	mcfErrorType _err = _mcf_file_write(&model_enc, file_path);
	
	_mcf_free_data_buffer(&model_enc);

	return _err;
}
