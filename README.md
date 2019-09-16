# DirectX 11 ECS Playground

Small DirectX renderer with an Entity Component System architecture.

## Table of Contents

+ [Building](#Building)
+ [Examples](#Examples)
    + [Basics](#Basics)
    + [Blinn-Phong shading](#Blinn-Phong-Shading)
    + [Toon shading](#Toon-Shading)
	+ [Physically Based Rendering](#PBR-Shading)
	+ [Intermediate](#Intermediate)
    + [Advanced](#Advanced)
+ [Based on](#BasedOn)

## Building

You must download [Visual Studio IDE](https://visualstudio.microsoft.com/) with C++ packaged and open `directx11-playground.sln` with it.

## Examples

### Basics

#### [Triangle](src/examples/basics/basic-triangle/)

<p align="left">
<img src="doc/img/basic/triangle.png?raw=true" alt="Triangle exemple" height="160px">
</p>

A simple triangle. It is the only exemple that does not use indexed drawing.

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

### Blinn-Phong shading

#### [Directional light](src/examples/blinn-phong/point-light/)

<p align="left">
<img src="doc/img/blinn-phong/directional-light.png?raw=true" alt="Phong shading exemple" height="160px">
</p>

Setup a simple scene with a directional light and no material.
Uses ambient, diffuse and specular properties.

#### [Multiple light types](src/examples/blinn-phong/multiple-light-types/)

Uses multiple point, directional and spotlights.

#### [Materials](src/examples/blinn-phong/materials/)

Uses different objects with different materials applied to them to compare how they react to lighting.

### Toon shading

### PBR shading

### Intermediate

### Shadows

### Advanced

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
