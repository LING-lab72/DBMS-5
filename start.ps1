# 构建并启动 DBMS（Windows PowerShell）
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
.\build\Release\dbms.exe
