#!/bin/bash
cd $(dirname $0)


mkdir -p Logs 

mkdir -p build 
mkdir -p build/rel
mkdir -p build/rel/editor
mkdir -p build/rel/editor/obj
mkdir -p build/rel/editor/lib
mkdir -p build/rel
mkdir -p build/rel/player
mkdir -p build/rel/player/obj
mkdir -p build/rel/player/lib
mkdir -p build/dbg
mkdir -p build/dbg/editor
mkdir -p build/dbg/editor/obj
mkdir -p build/dbg/editor/lib
mkdir -p build/dbg
mkdir -p build/dbg/player
mkdir -p build/dbg/player/obj
mkdir -p build/dbg/player/lib
mkdir -p vendor/lib
mkdir -p vendor/lib/editorOnly
mkdir -p vendor/lib/shared
