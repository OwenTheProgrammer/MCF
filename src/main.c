#include <MCF/core.h>

int main() {
    float vertex_data[2*4] = {
        0.0, 0.0,
        1.0, 0.0,
        0.0, 1.0,
        1.0, 1.0
    };

    int index_data[2*3] = {
        0, 1, 2,
        0, 2, 3
    };

    mcfBufferLayout vbuf_layout = (mcfBufferLayout){
        .element_count=4,
        .component_count=2,
        .component_type=MCF_COMPONENT_TYPE_FLOAT
    };
    mcfBufferLayout ibuf_layout = (mcfBufferLayout){
        .element_count=2,
        .component_count=3,
        .component_type=MCF_COMPONENT_TYPE_U32
    };

    mcfBlock* vertex_block = mcf_create_block(0, vbuf_layout, &vertex_data);
    mcfBlock* index_block = mcf_create_block(1, ibuf_layout, &index_data);

    mcfModel* model = mcf_create_model(0);

    mcf_model_add_block(model, vertex_block);
    mcf_model_add_block(model, index_block);

    mcf_export_model(model, "file.mcf");

    mcf_free_model(model);
    mcf_free_block(vertex_block);
    mcf_free_block(index_block);
    return 0;
}
