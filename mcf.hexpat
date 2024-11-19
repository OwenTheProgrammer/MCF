#pragma magic [ 6D 63 66 00 ] @ 0x00

import std.core;

struct mcfHeader {
    u32 file_id [[comment("File identification code")]];
    
    u32 file_version [[comment("Version of the file format")]];
    
    u32 uid [[comment("Optional unique asset identifier")]];
    
    u32 block_count [[comment("Amount of data blocks in this file")]];
    
    u32 block_offsets[block_count] [[comment("Array of block addresses as base pointer offsets")]];
};

struct mcfBufferLayout {
    u32 element_count [[comment("Ammount of array elements in the buffer")]];
    
    u32 component_count [[comment("Amount of components each element has")]];
    
    u32 component_stride [[comment("Size of each component in bytes")]];
} [[comment("Defines the memory layout of an MCF buffer")]];

fn get_layout_footprint(mcfBufferLayout x) {
    return x.element_count * x.component_count * x.component_stride;
};

struct mcfBlock {
    u32 type [[comment("This blocks type ID")]];
    mcfBufferLayout buffer_layout [[comment("The memory layout of this buffer")]];
    char memory[get_layout_footprint(buffer_layout)] [[comment("The blocks data")]];
};

struct mcfModel {
    mcfHeader header;
    
    mcfBlock block_list[header.block_count] @ header.block_offsets[std::core::array_index()];
} [[comment("A Model Component Format file or MCF file")]];

mcfModel model @ 0x00;