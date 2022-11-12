; ModuleID = 'text_module'
source_filename = "text_module"

define i32 @main() {
"1":
  %0 = alloca i32, align 4
  store i32 1, i32* %0, align 4
  %1 = alloca i32, align 4
  store i32 2, i32* %1, align 4
  %2 = alloca i32, align 4
  store i32 3, i32* %2, align 4
  %3 = load i32, i32* %1, align 4
  %4 = load i32, i32* %2, align 4
  %5 = add i32 %3, %4
  %6 = load i32, i32* %0, align 4
  store i32 %5, i32 %6, align 4
  ret i32 0
}
