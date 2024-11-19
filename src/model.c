#include "model.h"
#include "footprint.h"

_mcfHeader _mcf_create_header(size_t block_count) {
	_mcfHeader header = (_mcfHeader){0};

	header.file_id = _MCF_FILE_ID;
	header.file_version = _MCF_FILE_VERSION;
	header.uid = 0;
	header.block_count = block_count;
	header.block_offsets = _mcf_calloc(block_count, sizeof(*header.block_offsets));

	return header;
}

size_t _mcf_get_buffer_layout_index(mcfBufferLayout buffer_layout, size_t element_index) {
    size_t offset = buffer_layout.component_count * buffer_layout.component_stride;
    return element_index * offset;
}

uint32_t _mcf_header_add_block(_mcfHeader* const header) {    
    uint32_t idx = header->block_count;
    header->block_count++;
    header->block_offsets = _mcf_realloc(header->block_offsets, header->block_count * sizeof(header->block_count));
    return idx;
}

void _mcf_free_header(_mcfHeader* const header) {
	if(header) {
		_mcf_free(header->block_offsets);
		*header = (_mcfHeader){0};
	}
}

MCFAPI mcfModel* mcf_create_model(uint32_t block_count) {
    _mcfModel* model = _mcf_calloc(1, sizeof(_mcfModel));

    model->header = _mcf_create_header(block_count);
    model->block_list = NULL;

    MCF_LOG("Created model");
    return (mcfModel*)model;
}

MCFAPI mcfBlock* mcf_create_block(uint32_t block_type, mcfBufferLayout buffer_layout, void* const src_buffer) {
    _mcfBlock* block = _mcf_calloc(1, sizeof(_mcfBlock));

    block->type = block_type;
    block->buffer_layout = buffer_layout;

    size_t alloc_size = _mcf_get_buffer_layout_footprint(buffer_layout);
    block->buffer = _mcf_create_data_buffer(alloc_size, src_buffer);

    MCF_LOG("Block created");
    return (mcfBlock*)block;
}

MCFAPI mcfErrorType mcf_block_set_data(mcfBlock* const block, size_t dst_offset, void* const src, size_t src_size) {
    _mcfBlock* _block = (_mcfBlock*)block;
    return _mcf_data_buffer_set(&_block->buffer, dst_offset, src, src_size, MCF_TRUE);
}

MCFAPI void* mcf_block_get_data(mcfBlock* const block, size_t index) {
    _mcfBlock* _block = (_mcfBlock*)block;
    size_t offset = _mcf_get_buffer_layout_index(_block->buffer_layout, index);
    return _mcf_data_buffer_get(&_block->buffer, offset);
}

MCFAPI uint32_t mcf_model_add_block(mcfModel* const model, mcfBlock* const block) {
    _mcfBlock* _block = (_mcfBlock*)block;
    _mcfModel* _model = (_mcfModel*)model;
    
    uint32_t block_idx = _mcf_header_add_block(&_model->header);
    _model->block_list = _mcf_realloc(_model->block_list, _model->header.block_count * sizeof(_mcfBlock));
    _model->block_list[block_idx] = *_block;

    MCF_LOG("Added block %u to model", block_idx);

    return block_idx;
}

MCFAPI void mcf_free_model(mcfModel* const model) {
    if(model == NULL)
        return;
    
    _mcfModel* _model = (_mcfModel*)model;
    _mcf_free_header(&_model->header);
    _mcf_free(model);
    
    MCF_LOG("Disposed model");
}

MCFAPI void mcf_free_block(mcfBlock* const block) {
    if(block == NULL)
        return;
    
    _mcfBlock* _block = (_mcfBlock*)block;
    _mcf_free_data_buffer(&_block->buffer);
    _mcf_free(block);

    MCF_LOG("Disposed block");
}
