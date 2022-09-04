; ModuleID = 'text_module'
source_filename = "text_module"

define i32 @main() {
"1":
  %0 = alloca i32, align 4
  store i32 1, i32* %0, align 4
  %1 = alloca i32, align 4
  store i32 3, i32* %1, align 4
  %2 = alloca i32, align 4
  store i32 32, i32* %2, align 4
  %3 = alloca i32, align 4
  store i32 3, i32* %3, align 4
  %4 = load i32, i32* %0, align 4
  %5 = load i32, i32* %1, align 4
  %6 = icmp slt i32 %4, %5
  %7 = icmp ne i1 %6, false
  br i1 %7, label %"2", label %"5"

"2":                                              ; preds = %"5", %"1"
  %8 = load i32, i32* %1, align 4
  store i32 1, i32 %8, align 4
  br label %"4"

"3":                                              ; preds = %"5"
  %9 = load i32, i32* %1, align 4
  %10 = load i32, i32* %0, align 4
  %11 = load i32, i32 %10, align 4
  %12 = add i32 %9, %11
  store i32 %12, i32 %10, align 4
  br label %"4"

"4":                                              ; preds = %"3", %"2"
  ret i32 0

"5":                                              ; preds = %"1"
  %13 = load i32, i32* %2, align 4
  %14 = load i32, i32* %1, align 4
  %15 = icmp sgt i32 %13, %14
  %16 = icmp ne i1 %15, false
  br i1 %16, label %"2", label %"3"
}
