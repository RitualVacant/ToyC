Toy C89 compilers

#### Description

> This is a toy C89 compiler, which is used to learn compiler knowledge. It is not recommended to use it in production
> environment.

#### depends

1. LLVM
2. fmt
3. gflags
4. nlohmann/json

#### Installation

- set LLVM_DIR, gflags_DIR, fmt_DIR in CMakeLists.txt
- `add_subdirectory()` to add nlohmann/json path in CMakeLists.txt