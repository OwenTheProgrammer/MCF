#include <MCF/core.h>
#include "internal.h"
#include <stdint.h>
#include <stddef.h>

/*!
 * @brief Initializes a new model header
 * @param block_count The amount of blocks to initially allocate with
 * @return The created header
 */
_mcfHeader _mcf_create_header(size_t block_count);

/*!
 * @brief Calculates the address of an element within a buffers layout schema
 * @param buffer_layout The layout of the data buffer
 * @param element_index The index of the element
 * @return The byte offset of the element
 * @note This currently considers each "component" of an "element" as an element itself.
 * One element with three components will be three elements here.
 */
size_t _mcf_get_buffer_layout_index(mcfBufferLayout buffer_layout, size_t element_index);

/*!
 * @brief Allocates a space in a headers block offsets table
 * @param header The header to add a block space to
 * @return The block index of the space
 */
uint32_t _mcf_header_add_block_space(_mcfHeader* const header);

/*!
 * @brief Disposes of a headers memory
 * @param header The header to dispose of
 */
void _mcf_free_header(_mcfHeader* const header);

