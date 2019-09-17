# DirectX 11 ECS Playground

Small DirectX renderer with an Entity Component System architecture.

## Table of Contents

+ [Building](#Building)
+ [Examples](#Examples)
    + [Basics](#Basics)
    + [Blinn-Phong shading](#Blinn-Phong-Shading)
    + [Non-Photorealistic rendering](#Non-Photorealistic-Rendering-NPR)
    + [Intermediate](#Intermediate)
    + [Physically based rendering](#Physically-based-rendering-PBR)
    + [Shadows](#Shadows)
	+ [Animations](#Animations)
	+ [Post-Processing](#Post-Processing)
    + [Advanced](#Advanced)
+ [Based on](#Based-On)

## Building

You must download [Visual Studio IDE](https://visualstudio.microsoft.com/) and install the C++ package. Now you can open `directx11-playground.sln` and build the app with `f5`.

## Examples

### Basics

#### [Triangle](src/examples/basics/basic-triangle/)

<p align="left">
<img src="doc/img/basic/triangle.png?raw=true" alt="Triangle exemple" height="160px">
</p>

A simple triangle. The only example that does not use indexed drawing.

#### [Rotating cube](src/examples/basics/rotating-cube/)

<p align="left">
<img src="doc/img/basic/cube.png?raw=true" alt="Rotating cube exemple" height="160px">
</p>

A rotating colored cube. Each face is independant so that the colors can be applied without smearing.

#### [Textured primitives](src/examples/basics/textured-primitives/)

<p align="left">
<img src="doc/img/basic/textured-primitives.png?raw=true" alt="Textured primitive exemple" height="160px">
</p>

Show some procedurally-generated primitives with their UV maps.

#### [Model loading](src/examples/basics/model-loading/)

<p align="left">
<img src="doc/img/basic/model.png?raw=true" alt="Model loading exemple" height="160px">
</p>

Loads a GLTF model with its texture.

#### [WIC] [Skybox](#)

Show an environment in the background using a cubemap.

#### [WIC] [Global GUI](#)

Display a grid, some icons for the lights and a dynamic gizmo to always know the orientation of the camera.

### Blinn-Phong shading

#### [Directional light](src/examples/blinn-phong/point-light/)

<p align="left">
<img src="doc/img/blinn-phong/directional-light.png?raw=true" alt="Phong shading exemple" height="160px">
</p>

Setup a simple scene with a directional light and no material.
A menu is available to edit ambient, diffuse and specular properties.

#### [WIC] [Multiple light types](src/examples/blinn-phong/multiple-light-types/)

Uses multiple point, directional and spotlights.

#### [WIC] [Materials](src/examples/blinn-phong/materials/)

Uses different objects with different materials applied to them to compare how they react to lighting.

### Non-Photorealistic Rendering (NPR)

#### [WIC] [Gooch shading](#)

Change pixel color value from "cool to warm" based on normal direction angle from the camera.

#### [WIC] [Toon shading](#)

A rendering technique closer to traditional 2D animation.

### Intermediate

#### [WIC] [Object picking](#)

Select objects on the scene with the mouse and raytracing.

#### [WIC] [View frustrum culling](#)

Do not render objects outside of the camera view.

#### [WIC] [Occlusion culling](#)

Do not render objects hidden by other ones.

#### [WIC] [High Dynamic Range](#)

Allows a rendered image to show more details when the scene contains both shadows and highlights.

#### [WIC] [Gamma correction](#)

Fix the gamma of the screen.

#### [WIC] [Normal mapping](#)

Show micro-geometry with lighting based on a texture.

#### [WIC] [Parallax mapping](#)

Adds micro-geometry to a model based on a texture.

### Physically based rendering (PBR)

#### [WIC] [Directional light](#)

Base exemple of PBR.

#### [WIC] [Multiple light types](#)

Test multiple light types with PBR.

#### [WIC] [Materials](#)

Uses PBR materials.

#### [WIC] [Image based lighting (IBL)](#)

Local illumination technique based on the scene.

#### [WIC] [Screen-space subsurface scattering (SSS)](#)

Shows hows lights can traverse some materials.

#### [WIC] [Hair](#)

Render hair and fur.

### Shadows

#### [WIC] [Directional shadow mapping](#)

Shadows created by a directional light.

#### [WIC] [Point shadow mapping](#)

Shadows created by point lights.

#### [WIC] [Screen space ambient occlusion (SSAO)](#)

Adds more shadows to micro-geometry based on vertex density.

#### [WIC] [Light map](#)

Baked global illumination based on raytracing.

### Animations

#### [WIC] [Linear transform](#)

Basic translation and rotation loaded from gltf.

#### [WIC] [Squeletal animation](#)

A basic character movement loaded from gltf.

### Post-Processing

#### [WIC] [Bloom](#)

Show a light with bloom effect.

#### [WIC] [Depth of field](#)

Blur the out of focus area of the image.

#### [WIC] [Motion blur](#)

Blur based on movement of the camera.

### Advanced

#### [WIC] [Particle system](#)

Shows emissive particles.

#### [WIC] [Instanced rendering](#)

Render a forest.

#### [WIC] [Tesselation](#)

Shows how an object contains more or less geometry based on camera distance.

#### [WIC] [Deferred shading](#)

Improve lighting performance with geometry buffer.

#### [WIC] [Deferred plus shading](#)

Improve lighting performance in a forward shading pipeline by dividing the screen on multiple areas.

## Based on

### Youtube tutorials

- [ChiliTomatoNoodle](https://www.youtube.com/channel/UCsyHonfwHi4fLb2lkq0DEAA)

### Websites

- [RasterTek](http://www.rastertek.com/tutdx11.html)
- [D3dCoder](http://www.d3dcoder.net/d3d11.htm)
- [DirectXTutorials](http://www.directxtutorial.com/LessonList.aspx?listid=11)

### Repos

- [DirectX SDK samples](https://github.com/walbourn/directx-sdk-samples/tree/master/Direct3D11Tutorials)
- [gltf DXViewer](https://github.com/microsoft/glTF-DXViewer)
- [fx gltf Viewer](https://github.com/jessey-git/fx-gltf/blob/master/examples/viewer/)

### Books

- [Beginning DirectX 11 Game Programming](https://www.oreilly.com/library/view/beginning-directx11-game/9781435458956/)
- [Real-Time 3D Rendering with DirectX](https://www.oreilly.com/library/view/real-time-3d-rendering/9780133570120/)
