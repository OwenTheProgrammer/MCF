#include "footprint.h"

size_t _mcf_get_layout_size(mcfBufferLayout layout) {
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
