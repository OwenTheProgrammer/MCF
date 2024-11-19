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

    return (mcfModel*)model;
}

MCFAPI void mcf_free_model(mcfModel* const model) {
    if(model == NULL)
        return;
    
    _mcfModel _model = *(_mcfModel*)model;
    _mcf_free_header(&_model.header);
    _mcf_free(model);
}
