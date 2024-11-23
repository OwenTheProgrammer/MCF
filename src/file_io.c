#include "internal.h"
#include <stdio.h>
#include <stdlib.h>

_mcfDataBuffer _mcf_file_read(const char* file_path) {
	if(file_path == NULL) {
		MCF_ERROR(MCF_ERROR_NULL_TYPE, "Invalid file path given");
		return (_mcfDataBuffer){0};
	}

	FILE* file = fopen(file_path, "rb");
	if(file == NULL) {
		MCF_ERROR(MCF_ERROR_FILE_IO, "Failed to open file \"%s\"", file_path);
		return (_mcfDataBuffer){0};
	}

	//Get the size of the file
	fseek(file, 0, SEEK_END);
	size_t file_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	MCF_LOG("Reading %zu bytes from \"%s\"", file_size, file_path);

	_mcfDataBuffer buffer = _mcf_create_data_buffer(file_size, NULL);
	size_t bytes_read = fread(buffer.memory, sizeof(char), file_size, file);
	fclose(file);

	if(bytes_read != file_size) {
		MCF_ERROR(MCF_ERROR_FILE_IO, "Failed to fully read file \"%s\", read %zu / %zu Bytes", file_path, bytes_read, file_size)
	}

	buffer.used_space = file_size;

	MCF_LOG("Loaded file to buffer successfully");
	return buffer;
}

mcfErrorType _mcf_file_write(_mcfDataBuffer buffer, const char* file_path) {
	if(file_path == NULL) {
		MCF_ERROR(MCF_ERROR_NULL_TYPE, "Invalid file path given");
		return MCF_ERROR_NULL_TYPE;
	}

	if(buffer.memory == NULL) {
		MCF_ERROR(MCF_ERROR_TYPE_UNINITIALIZED, "Buffer needs to be initialized first");
		return MCF_ERROR_TYPE_UNINITIALIZED;
	}

	//Unlikely case but I see no reason to treat writing a zero-sized buffer as an error,
	//therefore it is not handled as such.

	FILE* file = fopen(file_path, "wb");

	if(file == NULL) {
		MCF_ERROR(MCF_ERROR_FILE_IO, "Failed to open file \"%s\"", file_path);
		return MCF_ERROR_FILE_IO;
	}

	MCF_LOG("Writing %zu bytes to \"%s\"", buffer.size, file_path);

	if(buffer.size) {
		fwrite(buffer.memory, sizeof(char), buffer.size, file);
		fflush(file);
		fclose(file);
	}

	MCF_LOG("Exported %zu bytes to \"%s\" Successfully.", buffer.size, file_path);
	return MCF_OK;
}
