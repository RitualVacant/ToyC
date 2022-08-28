; ModuleID = 'text_module'
source_filename = "text_module"

define i32 @f1() {
"1":
  %0 = alloca i32, align 4
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  br label %"2"

"2":                                              ; preds = %"3", %"1"
  %3 = load i32, i32* %2, align 4
  %4 = load i32, i32* %1, align 4
  %5 = icmp slt i32 %3, %4
  %6 = icmp ne i1 %5, false
  br i1 %6, label %"3", label %"4"

"3":                                              ; preds = %"2"
  %7 = load i32, i32* %1, align 4
  %8 = load i32, i32* %2, align 4
  %9 = add i32 %7, %8
  store i32 %9, i32* %0, align 4
  %10 = load i32, i32* %2, align 4
  %11 = load i32, i32* %1, align 4
  %12 = add i32 %10, %11
  store i32 %12, i32* %2, align 4
  br label %"2"

"4":                                              ; preds = %"2"
  %13 = load i32, i32* %0, align 4
  %14 = load i32, i32* %1, align 4
  %15 = add i32 %13, %14
  ret i32 %15
}
