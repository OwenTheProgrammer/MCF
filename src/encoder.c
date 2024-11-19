#include "encoder.h"
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

#define ENCODE_ARRAY(_array, _count)														\
	_mcf_data_buffer_append(&_encode_buf, _array, (_count) * sizeof(*_array), MCF_TRUE);	\
	_encode_ptr += (_count) * sizeof(*_array);

#define ENCODE_END() _encode_buf;

_mcfDataBuffer _mcf_encode_header(_mcfHeader* const header) {
	MCF_LOG("Encoding header");
	size_t header_size = _mcf_get_header_footprint(*header);
	
	ENCODE_BEGIN(header_size)

	ENCODE_PARAM(header->file_id)
	ENCODE_PARAM(header->file_version)
	ENCODE_PARAM(header->uid)
	ENCODE_PARAM(header->block_count)
	ENCODE_ARRAY(header->block_offsets, header->block_count)

	return ENCODE_END()
}

_mcfDataBuffer _mcf_encode_model(_mcfModel* const model) {
	MCF_LOG("Encoding model");

	_mcfDataBuffer header_enc = _mcf_encode_header(&model->header);

	return header_enc;
}

MCFAPI mcfErrorType mcf_export_model(mcfModel* const model, const char* file_path) {
	if(model == NULL) {
		MCF_ERROR(MCF_ERROR_NULL_TYPE, "Model can not be null");
		return MCF_ERROR_NULL_TYPE;
	}

	_mcfModel* _model = (_mcfModel*)model;

	_mcfDataBuffer header_enc = _mcf_encode_header(&_model->header);

	mcfErrorType _err = _mcf_file_write(&header_enc, file_path);
	
	_mcf_free_data_buffer(&header_enc);

	return _err;
}
