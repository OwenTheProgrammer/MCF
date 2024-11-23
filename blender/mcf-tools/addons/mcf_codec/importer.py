# type: ignore
from .mcf import *
from bpy_extras.io_utils import ImportHelper
from bpy.props import StringProperty
from bpy.types import Operator
from pathlib import Path

class McfImporter(Operator, ImportHelper):
	bl_idname = "mcf_importer.mcf"
	
	#This is what the import button says
	bl_label = "Import MCF file"

	filename_ext = ".mcf"

	filter_glob: StringProperty(
		default='*.mcf',
		options={'HIDDEN'},
		maxlen=255
	)

	def execute(self, ctx):
		file_path = self.properties.filepath
		print(f"MCF: Loading \"{file_path}\"..")

		mcf_data = None
		with open(file_path, mode='rb') as file:
			mcf_data = file.read()
			file.close()
		
		try:
			# Load the header section
			header = McfHeader.from_binary(mcf_data)
			print(f"MCF: Loading {header.block_count} blocks..")

			blocks = []
			for addr in header.block_offsets:
				decoded = McfBlock.from_binary(mcf_data[addr:])
				blocks.append(decoded)
			
			file_name = Path(file_path).stem
			model_data = McfModel(file_name, blocks)

			vertex_data = model_data.compose_vertex_buffer()
			if vertex_data == None:
				self.report({'ERROR'}, "MCF model does not contain a valid vertex buffer")
			
			index_data = model_data.compose_index_buffer()
			model_data.create_mesh(ctx, vertex_data, index_data)
			
			print(f"MCF: Imported {header.block_count} blocks successfully.")
			self.report({'INFO'}, f"MCF: Imported {header.block_count} blocks")
		except Exception as e:
			print(f"MCF: ERROR {e}")
			self.report({'ERROR'}, f"MCF: ERROR {e}")

			return {'CANCELLED'}

		return {'FINISHED'}