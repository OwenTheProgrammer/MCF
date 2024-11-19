# type: ignore
from . import mcf
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
		print("executed")
		return {'FINISHED'}