#!/bin/bash
# 构建并启动 DBMS（Linux / macOS）
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -- -j$(nproc)
./build/dbms
