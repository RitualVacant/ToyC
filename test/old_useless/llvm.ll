; ModuleID = 'text_module'
source_filename = "text_module"

define i32 @f1() {
"1":
  %0 = alloca i32, align 4
  %1 = alloca i32, align 4
  %2 = alloca [5 x i32], align 4
  %3 = alloca i32, align 4
  br label %"2"

"2":                                              ; preds = %"3", %"1"
  %4 = load i32*, i32* %3, align 8
  %5 = load i32*, i32* %1, align 8
  %6 = icmp slt i32* %4, %5
  %7 = icmp ne i1 %6, false
  br i1 %7, label %"3", label %"4"

"3":                                              ; preds = %"2"
  %8 = load i32*, i32* %1, align 8
  %9 = load i32*, i32* %3, align 8
  %10 = add i32* %8, %9
  store i32* %10, i32* %0, align 8
  %11 = load i32*, i32* %3, align 8
  %12 = load i32*, i32* %1, align 8
  %13 = add i32* %11, %12
  store i32* %13, i32* %3, align 8
  br label %"2"

"4":                                              ; preds = %"2"
  %14 = load i32*, i32* %0, align 8
  %15 = load i32*, i32* %1, align 8
  %16 = add i32* %14, %15
  ret i32* %16
}
