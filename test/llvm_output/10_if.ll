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
  %4 = load i32*, i32* %0, align 8
  %5 = load i32*, i32* %1, align 8
  %6 = icmp slt i32* %4, %5
  %7 = icmp ne i1 %6, false
  br i1 %7, label %"2", label %"5"

"2":                                              ; preds = %"5", %"1"
  %8 = load i32, i32 1, align 4
  store i32 %8, i32* %1, align 4
  br label %"4"

"3":                                              ; preds = %"5"
  %9 = load i32*, i32* %1, align 8
  %10 = load i32*, i32* %0, align 8
  %11 = add i32* %9, %10
  store i32* %11, i32* %0, align 8
  br label %"4"

"4":                                              ; preds = %"3", %"2"
  ret i32 0

"5":                                              ; preds = %"1"
  %12 = load i32*, i32* %2, align 8
  %13 = load i32*, i32* %1, align 8
  %14 = icmp sgt i32* %12, %13
  %15 = icmp ne i1 %14, false
  br i1 %15, label %"2", label %"3"
}
