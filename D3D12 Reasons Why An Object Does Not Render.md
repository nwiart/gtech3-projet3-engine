## Reasons why an object may not render

This is a list of everything that might cause one object (or an entire scene) not to render,
and just leave a blank screen to the developer.
This is one of the worst problems to get when making a rendering engine, simply because of
the number of potential causes to this one problem.

- Viewport / scissors not set.
- Vertex / index buffer not bound.
- Shader failed to compile.
- Pipeline state object not bound.
- No render target bound.
- Non-matching input layout and vertex format / Malformed input layout.
- Pipeline state format not matching with swap chain formats.
- Incorrect vertex / primitive count in draw call.
- Rendering an object which requires indexing, with a non-indexed draw call.
- Setting instance count to 0.
- Stencil test occludes everything.
- Blending is active, and alpha is zero for all pixels.
- Primitive's color is the same as the background color.
- Primitive is culled away.
- Object is behind camera (or culled by depth test).
- Depth buffer cleared using the wrong value.
- Presenting frame too early (e.g. while command queue isn't done executing yet)
- One or more matrices (world, view, projection) are corrupted.
- Matrices are stored in one format when the shader program expects the opposite.
- Mixed coordinate spaces (right-handed / left-handed).
- Didn't update matrices in the shader.
- Didn't execute the command list.
- Simply forgot to issue the draw call...
