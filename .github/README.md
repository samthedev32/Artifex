# Artifex - 2D Game Engine

## About

### Concept

The idea behind this engine is that instead of every system being baked in to the core of the engine, everything is a
module.

This means that everything - except the core - is a separate independent class.
This may come with a slight performance loss, but enables this engine to be very flexible; one can write a custom
renderer, physics engine and audio engine, while keeping the engine do its thing.

It also enables having multiple renderers at once, each rendering a separate entity or sequentially switching.

It is also cross-platform, currently supporting linux, windows and web builds.

### Basic Modules

It has a built-in renderer, that uses...

- [GLFW](www.glfw.org) for Windowing
- [OpenGL](www.opengl.org) for Rendering
- [OpenAL](https://www.openal-soft.org/) for Audio

but it can always be disabled completely.

There is also a built-in 2D physics engine, which can also be overridden.

## Components
TODO

## Documentation

### Setup

This Project is set up to work on both Linux and Windows with a few tweaks
I am using CLion to develop it, and I am trying to make it work out-of-the-box on both platforms

#### Linux (Debian Based)

Install Dependencies

```bash
sudo apt install libglfw3-dev libopenal-dev libuvc-dev
```

#### Windows

Every Include & Binary is in `/external`, should work as-is

#### macOS & iOS

I am not planning on making macOS & iOS Support, but I would appreciate if you did!

#### Android

I am planning on porting the engine to Android in the future, but it won't come anytime soon!

#### WASM (Emscripten)

WASM support is coming soon!

### Usage

...
