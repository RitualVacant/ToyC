; ModuleID = 'text_module'
source_filename = "text_module"

define i32 @main() {
"1":
  %0 = alloca i32, align 4
  %1 = alloca i32, align 4
  store i32 1, i32* %1, align 4
  %2 = alloca i32*, align 8
  %3 = alloca double, align 8
  store double 3.000000e+00, double* %3, align 8
  %4 = alloca double, align 8
  %5 = alloca i64, align 8
  %6 = alloca i32, align 4
  store i32 2, i32* %6, align 4
  ret i32 0
}
