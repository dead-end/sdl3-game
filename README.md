## emsdk

Description of the installation:

https://emscripten.org/docs/getting_started/downloads.html

```sh
git clone https://github.com/emscripten-core/emsdk.git

cd emsdk

git pull

./emsdk install latest

./emsdk activate latest

source ./emsdk_env.sh
```

## VS Code

VS Code does not find the sdl.h files, because they are installed with emsdk.

Strg + Umschalt + P

C/C++: Configuration edit (JSON)

add to includePath

"${userHome}/Desktop/emsdk/upstream/emscripten/cache/sysroot/include"
