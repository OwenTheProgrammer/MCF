#include <MCF/core.h>
#include <MCF/type_ext.h>
#include <MCF/model_ext.h>

#include <stdio.h>
#include <stdlib.h>

static void error_handler(int error_code, const char* function, const char* message) {
	printf("[%s threw %i]: ERROR: %s\n", function, error_code, message);
	exit(EXIT_FAILURE);
}

static void export_model() {
	float vertex_data[4*4] = {
		0.0, 0.0, 1.0,
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		1.0, 1.0, -1.0
	};

	int index_data[2*3] = {
		0, 1, 2,
		2, 1, 3
	};

	mcfBufferLayout vbuf_layout = MCF_FLOAT_VEC3_LAYOUT(4);
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
}

static void import_model() {
	mcfModel* model = mcf_import_model("file.mcf");

	mcfBlock* block = mcf_model_get_block(model, 0);
	uint32_t elems = mcf_block_get_element_count(block);

	for(uint32_t j = 0; j < elems; j++) {
		float x = *(float*)mcf_block_get_data(block, j*3+0);
		float y = *(float*)mcf_block_get_data(block, j*3+1);
		float z = *(float*)mcf_block_get_data(block, j*3+2);
		printf("Vertex %u: (%f, %f, %f)\n", j, x, y, z);
	}

	mcf_free_model(model);
}

int main() {
	mcf_set_error_handler(error_handler);

	export_model();
	import_model();

	return 0;
}
