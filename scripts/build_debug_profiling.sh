#!/bin/bash

SCRIPTPATH="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
cd $SCRIPTPATH/..

export PROFILER_FLAG=-DUSE_PROFILER
make debug-editor