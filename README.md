# karma

Karma Engine is a rendering & compute engine designed to be heavily modular, quick to learn, and quick to integrate into existing workflows.

Karma provides classes & functions to interact with the engine. The modules themselves are designed to be run with their defaults, but are also able to be heavily modified at runtime if the user wishes too. They can be modified either through JSON configurations, or through sending the data directly through a class provided by the engine.

The project provides callback systems for retrieving compute data once it is finished processing as well.

Currently, Karma is limited to a vulkan implementation. This is done first for the purpose of being cross-platform, but other Graphics API implementations are on the to-do list.

Karma is developed on Linux using Centos8 using GCC8, making use of many C++17 features as well. It is dependant on Vulkan 1.2 & SDL2.

The windows build of the library is also supported using MinGW-w64. Android & iOS are not yet supported.

For any questions or inquiries feel free to send them to jordiehendl@gmail.com !
