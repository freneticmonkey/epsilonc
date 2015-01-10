epsilonc
========

A basic C++ sandbox / prototyping engine.  The engine is developed in my spare time and as such updates are currently quite sporadic.  Development started when I began to run into performance limitations of the previous Python implementation of the engine.  I'm using this engine for proof of concept implementations for rapid game development features.

Features
========
* OpenGL Renderer
* Cross Platform: Windows / MacOS
* Python scripting
* Bullet Physics
* Hotloading Resources
* Basic XML Scene format

### Component Based Engine ###
I developed the engine with the Unity 3D architecture in mind.  Each scene contains nodes, which can be assigned various components for audio, physics, script etc.

### Scripting ###
Where possible I've tried to expose the engines classes to Python and most engine components are able to be modified from script.

### Hotloading ###
Currently all engine resources can be modified in place without restarting the engine.  This includes:
- scripts
- shaders
- models
- textures
- audio
- scenes

Dependencies
============
* SFML
* SFGUI
* Bullet Physics
* Intel Thread Building Blocks
* Boost ( Python, FileSystem, Format)
* Glew

TODO
====

### 10/1/2015 ###
* Convert math classes to glm
* Shadow Rendering
* Package Management
* Documentation
* Unit Testing
* Fix currently disabled UI
* Job tree threading implementation
* Scene Editor
* Switch to cross platform build system