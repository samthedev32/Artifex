# Typeless - Entity Component System

## About

**Typeless** is an ECS[^1] made for the game engine ***Artifex***

## Concept

**Typeless** doesn't distinguish between types, only their byte-size. This means that types with the same length go in the same column as others, and returns a UUID[^2] \
This UUID can then be used to index that value until its deletion, but be aware that there may be an entity with the same UUID as your value[^3]

## Implementation

The back-end implementation of the library is as follows

### Values

Each different-size values have their own dynamic arrays, alongside with a free-list for each, mapping unoccupied locations.

## Footnote
[^1]: ECS is the short form for *Entity-Component System*
[^2]: UUID stands for *Universally Unique ID*, although we have our own implementation
[^3]: values and entities do not share the same "ID-Space"
