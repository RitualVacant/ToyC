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
  %3 = load i32, i32* %1, align 4
  %4 = load i32, i32 %3, align 4
  %5 = add i32 %2, %4
  store i32 %5, i32 %3, align 4
  %6 = load i32, i32* %0, align 4
  %7 = icmp slt i32 %6, 4
  %8 = icmp ne i1 %7, false
  br i1 %8, label %"2", label %"3"

"3":                                              ; preds = %"2"
  ret i32 0
}
