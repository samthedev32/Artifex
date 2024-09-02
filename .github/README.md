# Artifex - 2D Game Engine

## About

Artifex is a light-weight game engine "skeleton", meaning that it only does the bare minimum it (module, entity & resource management); the rest is up to you!

It comes with a default Renderer, Mixer & Physics Engine, but you can always just use your own.

## Operating Principle

When you create the engine, you specify the amount of threads it is allowed to use (use 0 for automatic), \
then you can (and should) add modules and set up dependencies between them (the engine then allocates them to update threads)

...