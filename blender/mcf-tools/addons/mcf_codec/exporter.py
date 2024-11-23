# type: ignore
from .mcf import *
from bpy_extras.io_utils import ExportHelper
from bpy.props import StringProperty, BoolProperty
from bpy.types import Operator

class McfExporter(Operator, ExportHelper):
    bl_idname = "mcf_exporter.mcf"
    
    #This is what the export button says
    bl_label = "Export MCF files"

    #This is what the export buttons hover description says
    bl_description = "Export objects in the Modular Component Format (MCF)"

    filename_ext = ".mcf"

    filter_glob: StringProperty(
        default='*.mcf',
        options={'HIDDEN'}
    )

    eopt_include_vertex: BoolProperty(
        name="Vertex Data",
        description="If enabled, the vertex buffer containing the positions of each vertex will be included on export",
        default=True
    )
    eopt_include_index: BoolProperty(
        name="Index Data",
        description="If enabled, the index buffer for defining the order of vertices will be included on export",
        default=True
    )

    @classmethod
    def poll(self, ctx):
        return (ctx.mode == 'OBJECT') and (ctx.active_object != None)
    
    def draw(self, ctx):
        layout = self.layout

        layout.label(text="Export With:")

        export_toggles = layout.box()
        export_toggles.prop(self, 'eopt_include_vertex')
        export_toggles.prop(self, 'eopt_include_index')

    def invoke(self, ctx, events):
        obj = ctx.active_object

        name = obj.name.lower().replace(" ", "_")
        self.filepath = name + self.filename_ext

        return super().invoke(ctx, events)
    
    def execute(self, ctx):
        print(f"MCF: Exporting \"{self.filepath}\"..")

        obj = ctx.active_object

        block_gen = [
            McfBlock.create_vertex_block(obj).serialize(self.eopt_include_vertex),
            McfBlock.create_index_block(obj).serialize(self.eopt_include_index)
        ]

        file_blocks = []
        
        file_ptr = 0
        file_offsets = []
        for block in block_gen:
            if block is not None:
                file_offsets.append(file_ptr)
                file_blocks.append(block)
                file_ptr += len(block)

        header = McfHeader(block_count=len(file_blocks), block_offsets=file_offsets)

        with open(self.filepath, "wb") as file:
            file.write(header.serialize())

            for block in file_blocks:
                file.write(block)
            
            file.flush()

        print("MCF: Exported Successfully")
        return {'FINISHED'}