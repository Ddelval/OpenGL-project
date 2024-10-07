# OpenGL-project
> [!WARNING]
> This project has not been updated in over four years, and its build system is extremely basic.
> The chances of it building successfully on a modern system are slim.
> However, as of October 2024, both the macOS and Windows binaries in the latest release appear to work, for the most part.

Just a simple Minecraft-like game built using OpenGL 3.3, glad, glfw3 and glm.
So far I have included:
- Random terrain generation
- Shadow generation with two different shadow maps to increase the resolution for nearby objects
- Basic collision detection system using cuboids as hitboxes and only comparing the vertices to detect intersections.
- Simple ray casting to identify the closest filled block that the player is pointing at.
- Inventory bar
- Game engine handling collisons, entities and items

![Screenshot](screenshot.png)

Feel free to use the code and resources in this repository as you want.

Any comments on the approaches used to solve the problems tackled would be appreciated

\- David del Val
