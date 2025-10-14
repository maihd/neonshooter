# Introduction
This document decribe how I programming, design system, and solving problems in common.
The first principles are simple and solve your problems at hands, fix the bugs, add more features by not break the designs.
System-based programming and compression-oriented programming support theses principles.


## Details of entity_system.odin
Unlike ECS, the entity_system have one key rules just manage entities, iterations and finding. It's a system contains and manage entities, not a system of entities with architypes.
Features of entity_system:
- Handle based memory access
- Fast iterations
- Fast add and remove, predictable performance
- Memory safety
- Supported fixed timestep update and rendering


## Hot reloading
- Start with gameplay logic hot reload
- Using odin parser, detecting struct changing and state changing
- Do serialization, use sjson as simple format for converting old state to new state