; ModuleID = 'text_module'
source_filename = "text_module"

define i32 @main() {
"1":
  %0 = alloca i32, align 4
  store i32 1, i32* %0, align 4
  %1 = alloca i32, align 4
  store i32 3, i32* %1, align 4
  br label %"2"

"2":                                              ; preds = %"2", %"1"
  %2 = load i32, i32* %0, align 4
  %3 = load i32, i32 %2, align 4
  %4 = sub i32 3, %3
  store i32 %4, i32 %2, align 4
  %5 = load i32, i32 %2, align 4
  %6 = load i32, i32* %1, align 4
  %7 = load i32, i32 %6, align 4
  %8 = add i32 %5, %7
  store i32 %8, i32 %6, align 4
  %9 = load i32, i32* %0, align 4
  %10 = icmp slt i32 %9, 4
  %11 = icmp ne i1 %10, false
  br i1 %11, label %"2", label %"3"

"3":                                              ; preds = %"2"
  ret i32 0
}
