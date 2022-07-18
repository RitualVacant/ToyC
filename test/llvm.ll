; ModuleID = 'text_module'
source_filename = "text_module"

define i32 @f1() {
"1":
  %0 = alloca i32, align 4
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = load i32, i32* %0, align 4
  %5 = load i32, i32* %3, align 4
  %6 = add i32 %4, %5
  %7 = load i32, i32* %1, align 4
  %8 = icmp sge i32 %6, %7
  %9 = icmp ne i1 %8, false
  br i1 %9, label %"4", label %"3"

"2":                                              ; No predecessors!
  %10 = load i32, i32* %1, align 4
  store i32 %10, i32* %0, align 4
  %11 = load i32, i32* %0, align 4
  br label %"3"

"3":                                              ; preds = %"2", %"1"
  %12 = load i32, i32* %0, align 4
  %13 = load i32, i32* %1, align 4
  %14 = add i32 %12, %13
  ret i32 %14

"4":                                              ; preds = %"1"
  %15 = load i32, i32* %1, align 4
  %16 = load i32, i32* %2, align 4
  %17 = icmp sge i32 %15, %16
}
