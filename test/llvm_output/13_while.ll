; ModuleID = 'text_module'
source_filename = "text_module"

define i32 @main() {
"1":
  %0 = alloca i32, align 4
  store i32 4, i32* %0, align 4
  %1 = alloca i32, align 4
  store i32 1, i32* %1, align 4
  br label %"2"

"2":                                              ; preds = %"3", %"1"
  %2 = load i32, i32* %1, align 4
  %3 = load i32, i32* %0, align 4
  %4 = icmp slt i32 %2, %3
  %5 = icmp ne i1 %4, false
  br i1 %5, label %"3", label %"4"

"3":                                              ; preds = %"2"
  %6 = load i32, i32* %1, align 4
  %7 = load i32, i32* %0, align 4
  %8 = load i32, i32 %7, align 4
  %9 = add i32 %6, %8
  store i32 %9, i32 %7, align 4
  br label %"2"

"4":                                              ; preds = %"2"
}
