#include "encoder.h"
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define ENCODE_BEGIN(_size)												\
	_mcfDataBuffer _encode_buf = _mcf_create_data_buffer(_size, NULL);	\
	size_t _encode_ptr = 0;

#define ENCODE_PARAM(_param)											\
	memcpy(_encode_buf.memory + _encode_ptr, &_param, sizeof(_param));	\
	_encode_ptr += sizeof(_param);

#define ENCODE_END() _encode_buf;

static size_t __get_header_footprint(_mcfHeader header) {
	size_t total = 0;
	total += sizeof(header.file_id);
	total += sizeof(header.file_version);
	total += sizeof(header.uid);
	total += sizeof(header.block_count);
	total += header.block_count * sizeof(*header.block_offsets);
	return total;
}

_mcfHeader _mcf_alloc_header(size_t block_count) {
	_mcfHeader header = (_mcfHeader){0};

	header.file_id = _MCF_FILE_ID;
	header.file_version = _MCF_FILE_VERSION;
	header.uid = 0;
	header.block_count = block_count;
	header.block_offsets = _mcf_calloc(block_count, sizeof(*header.block_offsets));

	return header;
}

void _mcf_free_header(_mcfHeader* const header) {
	if(header) {
		_mcf_free(header->block_offsets);
		*header = (_mcfHeader){0};
	}
}

_mcfDataBuffer _mcf_encode_header(_mcfHeader* const header) {
	MCF_LOG("Encoding header");
	size_t header_size = __get_header_footprint(*header);
	
	ENCODE_BEGIN(header_size)

	ENCODE_PARAM(header->file_id)
	ENCODE_PARAM(header->file_version)
	ENCODE_PARAM(header->uid)
	ENCODE_PARAM(header->block_count)

	return ENCODE_END()
}
