; ModuleID = 'text_module'
source_filename = "text_module"

define i32 @f1() {
"1":
  %0 = alloca i32, align 4
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = load i32, i32* %0, align 4
  %5 = load i32, i32* %1, align 4
  %6 = icmp slt i32 %4, %5
  %7 = icmp ne i1 %6, false
  br i1 %7, label %"2", label %"4"

"2":                                              ; preds = %"5", %"1"
  %8 = load i32, i32* %0, align 4
  %9 = load i32, i32* %1, align 4
  %10 = add i32 %8, %9
  store i32 %10, i32* %0, align 4
  br label %"3"

"3":                                              ; preds = %"2", %"5", %"4"
  %11 = load i32, i32* %0, align 4
  %12 = load i32, i32* %1, align 4
  %13 = add i32 %11, %12
  ret i32 %13

"4":                                              ; preds = %"1"
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
