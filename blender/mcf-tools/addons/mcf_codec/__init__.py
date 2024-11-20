# type: ignore
# Model Component Format (.mcf) Importer/Exporter for blender

bl_info = {
	"name": "Model Component Format (.mcf)",
	"author": "OwenTheProgrammer",
	"version": (0, 0, 1),
	"blender": (4, 2, 0),
	"location": "File > Import-Export",
	"description": "Model Component Format makes working with meshes outside of blender simpler.",
	"warning": "",
	"category": "Import-Export",
}

if "bpy" in locals():
	import importlib
	importlib.reload(mcf)
	importlib.reload(importer)
else:
	from . import mcf
	from . import importer

import bpy

def menu_import(self, context):
	self.layout.operator(importer.mcf_importer.bl_idname, text="Import MCF model (.mcf)")


mcf_classes = [
	importer.mcf_importer
]

def register():
	from bpy.utils import register_class

	for segment in mcf_classes:
		register_class(segment)
	
	bpy.types.TOPBAR_MT_file_import.append(menu_import)
	print("MCF: Registered.")

def unregister():
	from bpy.utils import unregister_class

	for segment in reversed(mcf_classes):
		unregister_class(segment)
	
	bpy.types.TOPBAR_MT_file_import.remove(menu_import)
	print("MCF: Unregistered.")

if __name__ == "__main__":
	register()