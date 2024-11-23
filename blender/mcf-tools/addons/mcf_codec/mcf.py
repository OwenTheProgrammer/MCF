#type: ignore
from enum import IntEnum
import numpy as np
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

McfComponentTypeCodeTable = {
	McfComponentType.CHAR:	'c',
	McfComponentType.U8:	'B',
	McfComponentType.U16:	'H',
	McfComponentType.U32:	'L',
	McfComponentType.U64:	'Q',
	McfComponentType.I8:	'b',
	McfComponentType.I16:	'h',
	McfComponentType.I32:	'l',
	McfComponentType.I64:	'q',
	McfComponentType.FLOAT:	'f',
	McfComponentType.DOUBLE:'d'
}

def stride_padding(length, target):
	return (target-1) - ((length-1) % target)

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
	
	def serialize(self):
		buffer = bytearray( struct.pack('<4L', self.file_id, self.file_version, self.uid, self.block_count) )
		offsets = np.array(self.block_offsets, dtype=np.uint32).flatten()
		buffer.extend( bytes(memoryview(offsets)) )

		print(f"MCF: Encoded header ({len(buffer)} Bytes)")
		return buffer


class McfBlock:
	def __init__(self, type=McfBlockType.NONE, elem_count=0, component_count=0, component_type=McfComponentType.CHAR, data=None):
		self.type = type
		self.element_count = elem_count
		self.component_count = component_count
		self.component_type = component_type
		self.data = data

	@classmethod
	def from_binary(self, stream):
		block_type, elem_count, cmp_count, cmp_type = struct.unpack_from('<4L', stream)

		if block_type >= McfBlockType.MAX:
			raise Exception(f"Block type {block_type} is invalid")

		if cmp_type >= McfComponentType.MAX:
			raise Exception(f"Component type {cmp_type} is invalid")
		
		preamble_size = struct.calcsize('<4L')
		buffer_data = struct.unpack_from(f"<{elem_count*cmp_count}{McfComponentTypeCodeTable[cmp_type]}", stream, preamble_size)
		return McfBlock(block_type, elem_count, cmp_count, cmp_type, buffer_data)
	
	@classmethod
	def create_vertex_block(self, obj):

		vpos = [v.co for v in obj.data.vertices]
		vpos = np.array(vpos, dtype=np.float32).flatten()

		return McfBlock(
			type=McfBlockType.VERTEX, 
			elem_count=vpos.size/3, 
			component_count=3,
			component_type=McfComponentType.FLOAT,
			data=vpos
		)
	
	@classmethod
	def create_index_block(self, obj):
		mesh = obj.data

		mesh.calc_loop_triangles()

		tris = [[id for id in t.vertices] for t in mesh.loop_triangles]
		tris = np.array(tris, dtype=np.uint32).flatten()

		return McfBlock(
			type=McfBlockType.INDEX,
			elem_count=tris.size/3,
			component_count=3,
			component_type=McfComponentType.U32,
			data=tris
		)

	def serialize(self, eopt):
		if not eopt:
			return None
		
		buffer = bytearray( struct.pack('<4L', int(self.type), int(self.element_count), int(self.component_count), int(self.component_type)) )
		buffer.extend( bytes(memoryview(self.data)) )
		
		print(f"MCF: Encoded block {self.type} ({len(buffer)} Bytes)")
		return buffer

class McfModel:
	def __init__(self, name="Untitled MCF Model", block_list: list[McfBlock] = []):
		self.name = name
		self.block_list = block_list

		self.vertex_block = None
		self.index_block = None

		self.select_blocks()

	def select_blocks(self):
		for block in self.block_list:
			match block.type:
				case McfBlockType.VERTEX if self.vertex_block == None:
					self.vertex_block = block
				case McfBlockType.INDEX if self.index_block == None:
					self.index_block = block
	
	def compose_vertex_buffer(self):
		if self.vertex_block == None:
			return None
		
		block = self.vertex_block
		data = np.array(block.data, dtype=np.float32)
		
		# Add zero padding to ensure whatever stride will become all vector3
		padded = np.pad(data, (0, stride_padding(data.size, block.component_count)))
		padded = padded.reshape((-1, block.component_count))
		final = np.pad(padded, [(0,0),(0, (3 - block.component_count))])

		return final.tolist()
	
	def compose_index_buffer(self):
		if self.index_block == None:
			return None
		
		block = self.index_block
		data = np.array(block.data, dtype=np.uint32)

		if data.size % 3 != 0:
			raise Exception(f"Invalid index buffer size of {data.size} indices")
		
		index_buffer = data.reshape((-1, 3))
		
		return index_buffer.tolist()

	def create_mesh(self, context, vertex_buffer, index_buffer):
		import bpy
		import bmesh

		mesh = bpy.data.meshes.new(self.name)
		obj = bpy.data.objects.new(self.name, mesh)

		context.scene.collection.objects.link(obj)
		context.view_layer.objects.active = obj
		obj.select_set(True)

		mesh = context.object.data
		bm = bmesh.new()

		for v in vertex_buffer:
			bm.verts.new(v)

		# Recalculate the internal index buffer
		bm.verts.ensure_lookup_table()

		for t in index_buffer:
			triangle = (bm.verts[t[0]], bm.verts[t[1]], bm.verts[t[2]])
			bm.faces.new(triangle)
		
		bm.faces.ensure_lookup_table()

		bm.to_mesh(mesh)
		bm.free()
