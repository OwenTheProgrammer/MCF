#include "footprint.h"

size_t _mcf_get_buffer_layout_footprint(mcfBufferLayout layout) {
    size_t element_size = layout.component_count * layout.component_stride;
    return element_size * layout.element_count;
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
    total += sizeof(block.buffer_layout.component_stride);
    total += block.buffer.size;
    return total;
}
