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
  %6 = icmp sge i32 %4, %5
  %7 = icmp ne i1 %6, false
  br i1 %7, label %"4", label %"3"

"2":                                              ; preds = %"9"
  %8 = load i32, i32* %1, align 4
  store i32 %8, i32* %0, align 4
  %9 = load i32, i32* %0, align 4
  br label %"3"

"3":                                              ; preds = %"2", %"8", %"7", %"6", %"5", %"4", %"1"
  %10 = load i32, i32* %0, align 4
  %11 = load i32, i32* %1, align 4
  %12 = add i32 %10, %11
  ret i32 %12

"4":                                              ; preds = %"1"
  %13 = load i32, i32* %1, align 4
  %14 = load i32, i32* %2, align 4
  %15 = icmp sge i32 %13, %14
  %16 = icmp ne i1 %15, false
  br i1 %16, label %"5", label %"3"

"5":                                              ; preds = %"4"
  %17 = icmp ne i1 %16, false
  br i1 %17, label %"6", label %"3"

"6":                                              ; preds = %"5"
  %18 = load i32, i32* %0, align 4
  %19 = load i32, i32* %2, align 4
  %20 = icmp sge i32 %18, %19
  %21 = icmp ne i1 %20, false
  br i1 %21, label %"7", label %"3"

"7":                                              ; preds = %"6"
  %22 = icmp ne i1 %21, false
  br i1 %22, label %"8", label %"3"

"8":                                              ; preds = %"7"
  %23 = load i32, i32* %0, align 4
  %24 = load i32, i32* %3, align 4
  %25 = icmp sge i32 %23, %24
  %26 = icmp ne i1 %25, false
  br i1 %26, label %"9", label %"3"

"9":                                              ; preds = %"8"
  br label %"2"
}
