#include <MCF/core.h>
#include "internal.h"
#include <stdint.h>
#include <stddef.h>

/*!
 * @brief Calculates the storage footprint of a buffer layout in bytes
 * @param layout The layout in question
 * @return The size of the layout in bytes
 */
size_t _mcf_get_buffer_layout_footprint(mcfBufferLayout layout);

/*!
 * @brief Calculates the storage footprint of a model header in bytes
 * @param header The header to evaluate
 * @return The size of the stored header in bytes
 */
size_t _mcf_get_header_footprint(_mcfHeader header);

size_t _mcf_get_block_footprint(_mcfBlock block);

size_t _mcf_get_component_type_footprint(mcfComponentType type);
