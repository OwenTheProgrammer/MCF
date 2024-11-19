#pragma magic [ 6D 63 66 00 ] @ 0x00

import std.core;

enum mcfComponentType : u32 {
    CHAR,
    U8,
    U16,
    U32,
    U64,
    I8,
    I16,
    I32,
    I64,
    FLOAT,
    DOUBLE,
    MAX
};

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
    
    mcfComponentType component_type [[comment("Type of each component")]];
} [[comment("Defines the memory layout of an MCF buffer")]];

fn get_layout_footprint(mcfBufferLayout x) {
    return x.element_count * x.component_count;
};

struct mcfBlock {
    u32 type [[comment("This blocks type ID")]];
    mcfBufferLayout buffer_layout [[comment("The memory layout of this buffer")]];
    
    match(buffer_layout.component_type) {
        (mcfComponentType::CHAR)    : { char buffer[get_layout_footprint(buffer_layout)]; }
        (mcfComponentType::U8)      : { u8 buffer[get_layout_footprint(buffer_layout)]; }
        (mcfComponentType::U16)     : { u16 buffer[get_layout_footprint(buffer_layout)]; }
        (mcfComponentType::U32)     : { u32 buffer[get_layout_footprint(buffer_layout)]; }
        (mcfComponentType::U64)     : { u64 buffer[get_layout_footprint(buffer_layout)]; }
        (mcfComponentType::I8)      : { s8 buffer[get_layout_footprint(buffer_layout)]; }
        (mcfComponentType::I16)     : { s16 buffer[get_layout_footprint(buffer_layout)]; }
        (mcfComponentType::I32)     : { s32 buffer[get_layout_footprint(buffer_layout)]; }
        (mcfComponentType::I64)     : { s64 buffer[get_layout_footprint(buffer_layout)]; }
        (mcfComponentType::FLOAT)   : { float buffer[get_layout_footprint(buffer_layout)]; }
        (mcfComponentType::DOUBLE)  : { double buffer[get_layout_footprint(buffer_layout)]; }
        (_) : {
            std::error("Invalid component type read");
        }
    }
};

struct mcfModel {
    mcfHeader header;
    
    mcfBlock block_list[header.block_count] @ header.block_offsets[std::core::array_index()] + sizeof(header);
} [[comment("A Model Component Format file or MCF file")]];

mcfModel model @ 0x00;