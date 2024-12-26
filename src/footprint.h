#include <mcf/core.h>
#include "internal.h"
#include <stdint.h>
#include <stddef.h>

/*!
 * @brief Calculates the encoded/storage footprint of a buffer layout in bytes
 * @param layout The layout to evaluate
 * @return The size of the layout in bytes
 */
size_t _mcf_get_buffer_layout_footprint(mcfBufferLayout layout);

/*!
 * @brief Calculates the storage footprint of a model header in bytes
 * @param header The header to evaluate
 * @return The size of the stored header in bytes
 */
size_t _mcf_get_header_footprint(_mcfHeader header);

/*!
 * @brief Calculates the encoded/storage footprint of a block in bytes
 * @param block The block to evaluate
 * @return The size of the stored block in bytes
 */
size_t _mcf_get_block_footprint(_mcfBlock block);

/*!
 * @brief Calculates the encoded/storage footprint
 * @param type The component type to evaluate
 * @return The size of the component type in bytes
 */
size_t _mcf_get_component_type_footprint(mcfComponentType type);
