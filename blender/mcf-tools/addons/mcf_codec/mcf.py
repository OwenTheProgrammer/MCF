
from enum import IntEnum
import struct

MCF_FILE_ID = int.from_bytes(b"mcf\0", byteorder='little', signed=False)
MCF_FILE_VERSION = 0

class McfBlockType(IntEnum):
	NONE    = 0
	VERTEX  = 1
	INDEX   = 2
	MAX     = 3

class McfComponentType(IntEnum):
	CHAR     = 0
	U8       = 1
	U16      = 2
	U32      = 3
	U64      = 4
	I8       = 5
	I16      = 6
	I32      = 7
	I64      = 8
	FLOAT    = 9
	DOUBLE   = 10
	MAX      = 11

class McfHeader:
	def __init__(self, file_id=MCF_FILE_ID, file_version=MCF_FILE_VERSION, uid=0, block_count=0, block_offsets=[]):
		self.file_id = file_id
		self.file_version = file_version
		self.uid = uid
		self.block_count = block_count
		self.block_offsets = block_offsets
	
	@classmethod
	def from_binary(self, stream):
		# Load the first 4 u32 values
		fid, version, uid, count = struct.unpack_from('<4L', stream)

		# Fail if this isnt an MCF file
		if fid != MCF_FILE_ID:
			raise Exception("File ID is invalid")
		
		# Read the block_offset table
		preamble_size = struct.calcsize('<4L')
		offsets = struct.unpack_from(f"<{count}L", stream, preamble_size)

		# Add the header size to the block offsets
		header_size = preamble_size + struct.calcsize('<L') * count
		offsets = [x + header_size for x in offsets]

		return McfHeader(fid, version, uid, count, offsets)


