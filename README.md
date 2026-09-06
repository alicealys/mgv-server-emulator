# mgv-server-emulator

![license](https://img.shields.io/github/license/alicealys/mgv-server-emulator.svg)
[![open bugs](https://img.shields.io/github/issues/alicealys/mgv-server-emulator/bug?label=bugs)](https://github.com/alicealys/mgv-server-emulator/issues?q=is%3Aissue+is%3Aopen+label%3Abug)
[![Build](https://github.com/alicealys/mgv-server-emulator/workflows/Build/badge.svg)](https://github.com/alicealys/mgv-server-emulator/actions)

Reimplementation of **METAL GEAR SURVIVE**'s backend server

discord server
https://discord.gg/hYfW9MEEGF

## Downloads

### Binary only

### Version history

## Documentation

## Compile from source

- Clone the Git repo. Do NOT download it as ZIP, that won't work.
- Install [premake5](https://premake.github.io/download) on your PATH
- **Windows** Run `generate.bat` and build via solution file in `build\mgv-server-emulator.sln`.
- **Linux** Run `generate.sh` then run `build-release.sh` or `build-debug.sh`  
  **NOTE**: make sure you are using `clang` or, if you are using `gcc`, use the [Mold](https://github.com/rui314/mold) linker
  ### Premake arguments

  | Argument                    | Description                                    |
  |:----------------------------|:-----------------------------------------------|
  | `--copy-to=PATH`            | Optional, copy the EXE to a custom folder after build, define the path here if wanted. |

