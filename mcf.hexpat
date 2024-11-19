/* MCF file header preamble */
struct mcfHeader {
    /* File identification code */
    u32 file_id;
    
    /* Version of the file format */
    u32 file_version;
    
    /* Optional unique asset identifier */
    u32 uid;
    
    /* Amount of blocks in this file */
    u32 block_count;
    
    /* Array of each blocks byte offset from the start of the file */
    u32 block_offsets[block_count];
};

/* A Model Component Format file or MCF file */
struct mcfModel {
    mcfHeader header;
};

mcfModel model @ 0x00;