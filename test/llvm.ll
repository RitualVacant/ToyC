; ModuleID = 'text_module'
source_filename = "text_module"

define i32 @f1() {
func_body:
  %0 = alloca i32, align 4
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = load i32, i32* %0, align 4
  %4 = load i32, i32* %1, align 4
  %5 = add i32 %3, %4
  store i32 %5, i32* %1, align 4
}
