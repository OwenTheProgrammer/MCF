#include <MCF/core.h>
#include "encoder.h"

int main() {

    _mcfHeader header = _mcf_alloc_header(2);

    _mcfDataBuffer encoded = _mcf_encode_header(&header);

    _mcf_file_write(&encoded, "file.mcf");

    _mcf_free_data_buffer(&encoded);
    _mcf_free_header(&header);

    return 0;
}
