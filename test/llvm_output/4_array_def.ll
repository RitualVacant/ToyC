; ModuleID = 'text_module'
source_filename = "text_module"

define i32 @main() {
"1":
  %0 = alloca [3 x i32], align 4
  %1 = alloca [3 x double], align 8
  %2 = alloca [3 x i32], align 4
  %3 = alloca [3 x i64*], align 8
  %4 = alloca [3 x [3 x [3 x i32]]], align 4
  %5 = alloca [3 x [3 x i32]], align 4
}
