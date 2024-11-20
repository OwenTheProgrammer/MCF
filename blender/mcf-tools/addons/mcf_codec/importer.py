# type: ignore
from .mcf import *
from bpy_extras.io_utils import ImportHelper
from bpy.props import StringProperty
from bpy.types import Operator

class mcf_importer(Operator, ImportHelper):
	bl_idname = "mcf_importer.mcf"
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
		
		header = McfHeader.from_binary(mcf_data)

		print(f"MCF: Loaded {header.block_count} blocks successfully.")
		return {'FINISHED'}