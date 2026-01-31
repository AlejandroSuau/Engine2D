cmake -B build
cmake --build . --target GameSandbox --config Debug


cmake --build . --target Sandbox -j
./demos/sandbox/Debug/Sandbox

# Windows
cmake --build . --target PathfindingShowcase -j
.\demos\pathfinding-showcase\Debug\PathfindingShowcase.exe

