#include <MCF/core.h>

int main() {

    mcfModel* model = mcf_create_model(2);

    mcf_export_model(model, "file.mcf");

    mcf_free_model(model);

    return 0;
}
