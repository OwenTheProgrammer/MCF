#include "internal.h"
#include <string.h>

_mcfDataBuffer _mcf_create_data_buffer(size_t size, void* data) {
	_mcfDataBuffer buffer = (_mcfDataBuffer){0};

	buffer.size = size;
	buffer.memory = _mcf_calloc(size, sizeof(char));

	if(data && size && buffer.memory) {
		memcpy(buffer.memory, data, size);
		buffer.used_space = size;
	}

	return buffer;
}

mcfErrorType _mcf_data_buffer_set(_mcfDataBuffer* _MCF_RESTRICT const dst, size_t dst_offset, void* _MCF_RESTRICT const src, size_t src_size, mcfBool can_resize) {
	if(!src_size) {
		return MCF_OK;
	}

	if(dst == NULL) {
		MCF_ERROR(MCF_ERROR_NULL_TYPE, "Destination can not be null");
		return MCF_ERROR_NULL_TYPE;
	}

	size_t domain_size = dst_offset + src_size;

	if(dst->memory == NULL) {
		//Treating the destination being uninitialized as an error
		//Is more annoying and makes less sense compared to initializing it

		//MCF_ERROR(MCF_ERROR_TYPE_UNINITIALIZED, "Please initialize the destination first");
		//return MCF_ERROR_TYPE_UNINITIALIZED;
		*dst = _mcf_create_data_buffer(domain_size, NULL);
		memset(dst->memory, 0, dst_offset);
		memcpy(dst->memory + dst_offset, src, src_size);
		dst->used_space = domain_size;
	}


	//Resize or error
	if(domain_size > dst->size) {
		if(can_resize) {
			dst->memory = _mcf_realloc(dst->memory, domain_size);
			dst->size = domain_size;
		}
		else {
			MCF_ERROR(MCF_ERROR_OVERFLOW, "Range out of bounds: src<%zu B> -> dst[%zu..%zu], dst<%zu B>",
				src_size, dst_offset, domain_size, dst->size
			);
			return MCF_ERROR_OVERFLOW;
		}
	}

	if(src) {
		//Set range data with source
		memcpy(dst->memory + dst_offset, src, src_size);
	}
	else {
		//Or set with zeros if source is NULL
		memset(dst->memory + dst_offset, 0, src_size);
	}

	//Update the used space if the domain covers more area than it
	if(can_resize && dst->used_space < domain_size) {
		dst->used_space = domain_size;
	}

	return MCF_OK;
}

mcfErrorType _mcf_data_buffer_append(_mcfDataBuffer* _MCF_RESTRICT const dst, void* _MCF_RESTRICT const src, size_t src_size, mcfBool can_resize) {
	return _mcf_data_buffer_set(dst, dst->used_space, src, src_size, can_resize);
}

mcfErrorType _mcf_data_buffer_extend(_mcfDataBuffer* _MCF_RESTRICT const dst, _mcfDataBuffer* _MCF_RESTRICT const src, mcfBool can_resize, mcfBool consume_src) {
	mcfErrorType _err = _mcf_data_buffer_append(dst, src->memory, src->used_space, can_resize);
	if(consume_src) {
		_mcf_free_data_buffer(src);
	}
	return _err;
}

void* _mcf_data_buffer_get(_mcfDataBuffer* const buffer, size_t offset) {
	if(buffer->memory == NULL) {
		MCF_ERROR(MCF_ERROR_TYPE_UNINITIALIZED, "Buffer was uninitialized");
		return NULL;
	}

	if(offset >= buffer->size) {
		MCF_ERROR(MCF_ERROR_OVERFLOW, "Offset %zu of buffer<%zu B> is out of range", offset, buffer->size);
		return NULL;
	}

	return (void*)&buffer->memory[offset];
}

mcfErrorType _mcf_data_buffer_resize(_mcfDataBuffer* const buffer, size_t new_size) {
	//Invalid buffer input
	if(buffer == NULL) {
		MCF_ERROR(MCF_ERROR_NULL_TYPE, "Buffer can not be null");
		return MCF_ERROR_NULL_TYPE;
	}

	//Buffer is already the target size
	if(buffer->size == new_size) {
		return MCF_OK;
	}

	//Buffer is unallocated
	if(buffer->memory == NULL || buffer->size == 0) {
		*buffer = _mcf_create_data_buffer(new_size, NULL);
		return MCF_OK;
	}

	//When buffer shrinks, update the used space
	if(buffer->used_space > new_size) {
		buffer->used_space = new_size;
	}

	//Resize the buffer
	if(new_size) {
		buffer->memory = _mcf_realloc(buffer->memory, new_size);
		buffer->size = new_size;
	}
	else {
		_mcf_free_data_buffer(buffer);
	}

	return MCF_OK;
}

void _mcf_free_data_buffer(_mcfDataBuffer* const buffer) {
	if(buffer) {
		_mcf_free(buffer->memory);
		*buffer = (_mcfDataBuffer){0};
	}
}
