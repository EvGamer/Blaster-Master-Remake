### Platformer Game
Continuation of my platformer project, that originally started as assignment for Computer Graphics course.
Recently changed to use Raylib to support multiple platforms

### Dependencies
- [CMake](https://cmake.org/download/)
- [Raylib](https://www.raylib.com/index.html)


### Configure cmake for emscripten
- Install [emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html)
- Select "specify toolchain file for cross-compiling"
- In emscripten SDK directory select toolchain file at `./upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake` 
- As `NODE_JS_EXECUTABLE` set `$path_to_emsdk/node/$node_version/bin/node` where you substitute `$path_to_emsdk` with path to emscripten SDK
 and `$node_version` with node version that is available there
- Set `PLATFORM` variable to `Web`

### Configure CMake for emscripten with VS Code CMake plugin
- Add CMake Tools plugin into VS Code
- Open command Palette
- Find and chose CMake: Edit User-Local CMake kits
- Add following, where you substitute `$path_to_emsdk` with path to emscripten SDK
```json
{
    "name": "Emscripten",
    "toolchainFile": "$path_to_emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake",
    "cmakeSettings": {
      "PLATFORM": "Web"
    }
}
```
- Choose `Emscripten` kit

### Build for web with VSCode
- Install [Emscripten](https://emscripten.org/)
- Configure CMake for emscripten with VS Code CMake plugin
- Click Build

### Build for web on unix
- Install [Emscripten](https://emscripten.org/)
- Configure project with CMake
- In `./build` folder run `make`