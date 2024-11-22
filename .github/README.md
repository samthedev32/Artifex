# Artifex - 2D Game Engine

## About

Artifex is a light-weight game engine "skeleton", meaning that it only does the bare minimum it (module, entity & asset management); the rest is up to you!

It comes with a default Renderer, Mixer & Physics Engine, but you can always just use your own.

## Operating Principle

At creation, you specify the amount of threads the engine will use (or will be done automatically), then create *modules* that run inside the engine. \
A module can be *global*, *local* (to entities) or both. This means that a module will have a callback in each frame (or so) both globally and per (assigned) entity.

You can also create new entities (in the ECS), which are globally accessible for the modules (given that they have their ID). \
As hinted to before, you can pass this ID to modules (that support it), and they will - from that point on - get callbacks with that entity's context. \
For example, for a renderer you would pass in an entity that should be rendered, with some info of which of it component is its location, size, texture, etc.

This structure makes the engine really light-weight, at the cost of ease-of-use... sort of. \
While the engine is primarily focused on speed, efficiency and modularity, it is also a priority to make it easy to use.

## Quick Start

Check out the [examples](examples/) to get familiar with the engine or to get template code! \
(there are also [unit tests](tests/), but they are less interesting)

## Behind the API

### Module System

In Artifex, Scripts are implemented as "modules". \
A module is a function-package with a custom pointer that are called on specific events (creation, update, deletion, etc). \
You can also have the same module executed with multiple user-pointers, for example an entity update script with each entity storing it's user pointer, but how are entities stored anyway? Onto the *ECS* section

### Entity Component System

The engine also features a basic entity-component system called **Typeless**. \
Using this ECS, you can create a new type - referred to with a string (or its hash) - to which you pre-define the size (in bytes), therefore the internal structure will only know the size a single element, but nothing about the type itself (that's where the name comes from)

### Asset Manager

The Asset Manager is similiar to the ECS, but instead of storing game entities it stores assets or their ID (GPU Texture IDs, etc) \
It is implemented so assets are stored in a centralized database, thus one does not have to consult with other modules to retrieve their data.
