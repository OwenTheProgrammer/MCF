#include "footprint.h"

static const size_t __MCF_COMPONENT_TYPE_FOOTPRINT[MCF_COMPONENT_TYPE_MAX] = {
	1,
	1, 2, 4, 8,
	1, 2, 4, 8,
	4, 8
};

size_t _mcf_get_buffer_layout_footprint(mcfBufferLayout layout) {
	size_t component_size = _mcf_get_component_type_footprint(layout.component_type);
	return layout.element_count * (layout.component_count * component_size);
}

size_t _mcf_get_header_footprint(_mcfHeader header) {
	size_t total = 0;
	total += sizeof(header.file_id);
	total += sizeof(header.file_version);
	total += sizeof(header.uid);
	total += sizeof(header.block_count);
	total += header.block_count * sizeof(*header.block_offsets);
	return total;
}

size_t _mcf_get_block_footprint(_mcfBlock block) {
	size_t total = 0;
	total += sizeof(block.type);
	total += sizeof(block.buffer_layout.element_count);
	total += sizeof(block.buffer_layout.component_count);
	total += _mcf_get_component_type_footprint(block.buffer_layout.component_type);
	total += block.buffer.size;
	return total;
}

size_t _mcf_get_component_type_footprint(mcfComponentType type) {
	return __MCF_COMPONENT_TYPE_FOOTPRINT[type];
}
