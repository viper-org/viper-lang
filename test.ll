
define dso_local i32 @main() #0 {
  %2 = alloca i32, align 4
  %3 = alloca i32*, align 8
  store i32 4, i32* %2, align 4
  store i32* %2, i32** %3, align 8
  %4 = load i32*, i32** %3, align 8
  %5 = load i32, i32* %4, align 4
  ret i32 %5
}