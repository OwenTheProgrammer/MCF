#include <MCF/core.h>
#include "internal.h"
#include "model.h"
#include "footprint.h"
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/*!
 * @brief Begins the context of an encoder pass
 * @param _SIZE The size of the encoder buffer in bytes (optional)
*/
#define ENCODE_BEGIN(_SIZE)												\
	_mcfDataBuffer _encode_buf = _mcf_create_data_buffer(_SIZE, NULL);	\
	size_t _encode_ptr = 0;

/*!
 * @brief Encodes a single parameter and writes it to the encoding buffer
 * @param _PARAM The parameter or argument to write
 * @param _TYPE The data type of `_PARAM`
*/
#define ENCODE_PARAM_AS(_PARAM, _TYPE)									\
	memcpy(_encode_buf.memory + _encode_ptr, &_PARAM, sizeof(_TYPE));	\
	_encode_buf.used_space += sizeof(_TYPE);							\
	_encode_ptr += sizeof(_TYPE);

/*!
 * @brief Same as `ENCODE_PARAM_AS` but the type is implied from the parameter
 * @param _PARAM The parameter or argument to write
*/
#define ENCODE_PARAM(_PARAM) ENCODE_PARAM_AS(_PARAM, _PARAM)

/*!
 * @brief Encodes an array of parameters and writes it to the encoding buffer
 * @param _ARRAY The array to encode and write
 * @param _COUNT The amount of elements in the array
*/
#define ENCODE_ARRAY(_ARRAY, _COUNT)														\
	_mcf_data_buffer_append(&_encode_buf, _ARRAY, (_COUNT) * sizeof(*_ARRAY), MCF_TRUE);	\
	_encode_ptr += (_COUNT) * sizeof(*_ARRAY);

/*!
 * @brief Closes an open encoding context
*/
#define ENCODE_END() _encode_buf;

static _mcfDataBuffer _mcf_encode_header(_mcfHeader header) {
	MCF_LOG("Encoding header");
	
	ENCODE_BEGIN(_mcf_get_header_footprint(header))

	ENCODE_PARAM(header.file_id)
	ENCODE_PARAM(header.file_version)
	ENCODE_PARAM(header.uid)
	ENCODE_PARAM(header.block_count)
	ENCODE_ARRAY(header.block_offsets, header.block_count)

	return ENCODE_END()
}

static _mcfDataBuffer _mcf_encode_block(_mcfBlock block) {
	MCF_LOG("Encode block");

	ENCODE_BEGIN(_mcf_get_block_footprint(block))

	ENCODE_PARAM(block.type)
	ENCODE_PARAM(block.buffer_layout.element_count)
	ENCODE_PARAM(block.buffer_layout.component_count)
	ENCODE_PARAM_AS(block.buffer_layout.component_type, uint32_t)
	ENCODE_ARRAY(block.buffer.memory, block.buffer.size)

	return ENCODE_END()
}

static _mcfDataBuffer _mcf_encode_model(_mcfModel model) {
	MCF_LOG("Encoding model");

	_mcfDataBuffer encode_buffer = _mcf_create_data_buffer(0, NULL);
	for(uint32_t i = 0; i < model.header.block_count; i++) {
		model.header.block_offsets[i] = encode_buffer.used_space;

		//Encode each block and merge it into encode_buffer
		_mcfDataBuffer block_enc = _mcf_encode_block(model.block_list[i]);
		_mcf_data_buffer_extend(&encode_buffer, &block_enc, MCF_TRUE, MCF_TRUE);
	}

	//Encode the header data from the finished blocks and prepend it to encode_buffer
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

	mcfErrorType _err = _mcf_file_write(model_enc, file_path);
	
	_mcf_free_data_buffer(&model_enc);

	return _err;
}
