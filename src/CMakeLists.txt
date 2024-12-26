
add_library(mcf
	internal.h init.c
	memory.c file_io.c
	footprint.h footprint.c
	model.h model.c
	decoder.c encoder.c
)

target_include_directories(mcf PUBLIC ${PROJECT_SOURCE_DIR}/include/)

set_target_properties(mcf PROPERTIES PUBLIC_HEADER
	mcf/core.h
	mcf/model_ext.h
	mcf/type_ext.h
)

target_compile_features(mcf PRIVATE c_std_17)
target_compile_options(mcf PRIVATE
	-Wall
	-Wextra
	-Wno-unknown-pragmas

	$<$<CONFIG:Debug>:
		-O0
		-DMCF_DEBUG
		-ggdb
		-Wno-unused-parameter
		-Wno-unused-function
	>

	$<$<CONFIG:Release>:-O3>
)