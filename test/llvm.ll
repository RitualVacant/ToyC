; ModuleID = 'text_module'
source_filename = "text_module"

define i32 @f1() {
"1":
  %0 = alloca i32, align 4
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  br label %"2"

"2":                                              ; preds = %"5", %"2", %"1"
  %4 = load i32, i32* %0, align 4
  %5 = load i32, i32* %1, align 4
  %6 = add i32 %4, %5
  store i32 %6, i32* %0, align 4
  %7 = load i32, i32* %0, align 4
  %8 = load i32, i32* %1, align 4
  %9 = icmp slt i32 %7, %8
  %10 = icmp ne i1 %9, false
  br i1 %10, label %"2", label %"4"

"3":                                              ; preds = %"5", %"4"
  %11 = load i32, i32* %0, align 4
  %12 = load i32, i32* %1, align 4
  %13 = add i32 %11, %12
  ret i32 %13

"4":                                              ; preds = %"2"
  %14 = load i32, i32* %2, align 4
  %15 = load i32, i32* %3, align 4
  %16 = icmp slt i32 %14, %15
  %17 = icmp ne i1 %16, false
  br i1 %17, label %"5", label %"3"

"5":                                              ; preds = %"4"
  %18 = load i32, i32* %1, align 4
  %19 = load i32, i32* %2, align 4
  %20 = icmp slt i32 %18, %19
  %21 = icmp ne i1 %20, false
  br i1 %21, label %"2", label %"3"
}
