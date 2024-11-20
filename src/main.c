#include <MCF/core.h>
#include <MCF/type_ext.h>
#include <MCF/model_ext.h>

int main() {
    float vertex_data[2*4] = {
        0.0, 0.0,
        1.0, 0.0,
        0.0, 1.0,
        1.0, 1.0
    };

    int index_data[2*3] = {
        0, 1, 2,
        2, 1, 3
    };

    mcfBufferLayout vbuf_layout = MCF_FLOAT_VEC2_LAYOUT(4);
    mcfBufferLayout ibuf_layout = MCF_U32_VEC3_LAYOUT(2);

    mcfBlock* vertex_block = mcf_create_block(MCF_BLOCK_TYPE_VERTEX, vbuf_layout, &vertex_data);
    mcfBlock* index_block = mcf_create_block(MCF_BLOCK_TYPE_INDEX, ibuf_layout, &index_data);

    mcfModel* model = mcf_create_model(0, NULL);

    mcf_model_add_block(model, vertex_block);
    mcf_model_add_block(model, index_block);

    mcf_export_model(model, "file.mcf");

    mcf_free_model(model);
    mcf_free_block(vertex_block);
    mcf_free_block(index_block);
    return 0;
}
