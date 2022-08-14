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
  %6 = icmp sle i32 %4, %5
  %7 = icmp ne i1 %6, false
  br i1 %7, label %"2", label %"4"

"2":                                              ; preds = %"5", %"1"
  %8 = load i32, i32* %0, align 4
  %9 = load i32, i32* %2, align 4
  %10 = add i32 %8, %9
  store i32 %10, i32* %3, align 4
  %11 = load i32, i32* %3, align 4
  br label %"3"

"3":                                              ; preds = %"2", %"5", %"4"
  %12 = load i32, i32* %0, align 4
  %13 = load i32, i32* %1, align 4
  %14 = add i32 %12, %13
  ret i32 %14

"4":                                              ; preds = %"1"
  %15 = load i32, i32* %2, align 4
  %16 = load i32, i32* %3, align 4
  %17 = icmp sle i32 %15, %16
  %18 = icmp ne i1 %17, false
  br i1 %18, label %"5", label %"3"

"5":                                              ; preds = %"4"
  %19 = load i32, i32* %0, align 4
  %20 = load i32, i32* %3, align 4
  %21 = icmp sle i32 %19, %20
  %22 = icmp ne i1 %21, false
  br i1 %22, label %"2", label %"3"
}
