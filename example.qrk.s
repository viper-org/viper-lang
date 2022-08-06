define i32 @main() {
main:
  %y = alloca i32*, align 8
  %x = alloca i32, align 4
  store i32 1, i32* %x, align 4
  %x1 = load i32, i32* %x, align 4
  store i32* %x, i32** %y, align 8
  %y2 = load i32*, i32** %y, align 8
  %dereftmp = load i32*, i32** %y, align 8
  %dereftmp3 = load i32, i32* %dereftmp, align 8
  ret i32 %dereftmp3
}
