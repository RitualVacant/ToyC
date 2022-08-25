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
  %6 = icmp sgt i32 %4, %5
  %7 = icmp ne i1 %6, false
  br i1 %7, label %"2", label %"3"

"2":                                              ; preds = %"1"
  %8 = load i32, i32* %1, align 4
  store i32 %8, i32* %0, align 4
  br label %"3"

"3":                                              ; preds = %"2", %"1"
  %9 = load i32, i32* %0, align 4
  %10 = load i32, i32* %1, align 4
  %11 = add i32 %9, %10
  ret i32 %11
}
