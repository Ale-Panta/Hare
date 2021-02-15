#!/bin/bash
pushd ../
./vendor/premake5 gmake2
popd
read -p "$*"
