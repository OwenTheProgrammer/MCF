#include <MCF/core.h>

int main() {
    // mcfModel* model = mcf_create_model(2);
    // mcf_export_model(model, "file.mcf");
    // mcf_free_model(model);

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

    mcfBufferLayout vbuf_layout = (mcfBufferLayout){.component_count=2, .component_stride=sizeof(float), .element_count=4};
    mcfBufferLayout ibuf_layout = (mcfBufferLayout){.component_count=3, .component_stride=sizeof(int), .element_count=2};

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
