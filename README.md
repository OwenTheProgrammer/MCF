# Modular Component Format (MCF)

The MCF file format `*.mcf` is a lightweight 3D model oriented framework that aims to package everything you need in a "here you go" file format / codec. MCF is built as a layered system that makes custom specifications of 3D model data from Blender to your engine and back as simple as possible.

MCF skips the bloat of file formats like FBX and the lacking support of simple formats like OBJ. When all you want is the vertex buffer, index buffer and (insert attribute that's not typical like weight painting and RGBA vertex colors) from Blender, It's a real fun game of guess-who and wastes many *many* hours of my time.

I often compare different model formats like a thrifty bitch only to find out the simple formats don't support things I need, while formats that do have support (if any) come with a birthday cake of PAIN AND SUFFERING for implementation. I DON'T HAVE TIME TO WRITE AN FBX CODEC!

## MCF Core
MCF without any extensions is just a container of vector-typed-element arrays. The core of MCF supports the bare minimum file read/write and simple manipulation functions that provide enough to let you build your custom model format.

This makes integration of new features and weird quirks as simple as:
- Creating wrappers that use base MCF functions to define the logic for your specific cases
- (Optionally) Adding encoder support by defining your field structure in the blender codec

You can opt in and out of almost everything this format has to offer. Want a model file with everything? yep! What about one with *just the index buffer for some reason?* yep!

As this project is just starting, I will be extending this readme in the future, but as of right now you can check out [The main file in src](src/main.c) as it demos my usage of MCF as of right now. All functions meant for you to use are located in [the includes directory](include/MCF) and the Blender extension can be added
through `Edit > Preferences > File Paths > Script Directories "...\blender\mcf-tools\"` or just installing as zip.

Even though this format serves as an intermediate layer to your game engines runtime or whatever, there's nothing stopping you from using MCF as the core format. Do keep in mind there is no compression here as I favour simplicity over storing every byte perfectly.
