; ModuleID = 'main'
source_filename = "main"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@a = common global i64 1.500000e+00
@b = common global i64 0
@c = common global i64 0

declare void @printInt(i64)

declare void @printDouble(double)

define i64 @testFunction(i64 %a, i64 %b) {
entry:
  %a1 = alloca i64, align 8
  store i64 %a, ptr %a1, align 4
  %b2 = alloca i64, align 8
  store i64 %b, ptr %b2, align 4
  %c = alloca i64, align 8
  %a3 = load i64, ptr %a1, align 4
  %b4 = load i64, ptr %b2, align 4
  %addtmp = add i64 %a3, %b4
  store i64 %addtmp, ptr %c, align 4
  %c5 = load i64, ptr %c, align 4
  ret i64 %c5
}

define void @nothing() {
entry:
  ret void
  ret void <badref>
}

define i64 @fib(i64 %a) {
entry:
  %a1 = alloca i64, align 8
  store i64 %a, ptr %a1, align 4
  %a2 = load i64, ptr %a1, align 4
  %eqtmp = icmp eq i64 %a2, 0
  br i1 %eqtmp, label %then, label %mergeBlock

then:                                             ; preds = %entry
  ret i64 0

mergeBlock:                                       ; preds = %entry
  %a3 = load i64, ptr %a1, align 4
  %eqtmp4 = icmp eq i64 %a3, 1
  br i1 %eqtmp4, label %then5, label %mergeBlock7

then5:                                            ; preds = %mergeBlock
  ret i64 1

mergeBlock7:                                      ; preds = %mergeBlock
  %a8 = load i64, ptr %a1, align 4
  %subtmp = sub i64 %a8, 1
  %calltmp = call i64 @fib(i64 %subtmp)
  %a9 = load i64, ptr %a1, align 4
  %subtmp10 = sub i64 %a9, 2
  %calltmp11 = call i64 @fib(i64 %subtmp10)
  %addtmp = add i64 %calltmp, %calltmp11
  ret i64 %addtmp
}

define i64 @main() {
entry:
  %a = alloca i64, align 8
  store double 1.500000e+00, ptr %a, align 8
  %b = alloca i64, align 8
  %z = alloca double, align 8
  %x = alloca double, align 8
  store double 3.140000e+00, ptr %x, align 8
  %y = alloca double, align 8
  store double 2.710000e+00, ptr %y, align 8
  %x1 = load double, ptr %x, align 8
  %calltmp = call void @printDouble(double %x1)
  %z2 = alloca i64, align 8
  %calltmp3 = call i64 @testFunction(i64 1, i64 2)
  store i64 %calltmp3, ptr %z2, align 4
  %z4 = load i64, ptr %z2, align 4
  %calltmp5 = call void @printInt(i64 %z4)
  store i64 5, ptr %a, align 4
  %a6 = load i64, ptr %a, align 4
  %addtmp = add i64 %a6, 10
  store i64 %addtmp, ptr %b, align 4
  %a7 = load i64, ptr %a, align 4
  %b8 = load i64, ptr %b, align 4
  %addtmp9 = add i64 %a7, %b8
  %multmp = mul i64 %addtmp9, 2
  %a10 = load i64, ptr %a, align 4
  %b11 = load i64, ptr %b, align 4
  %addtmp12 = add i64 %a10, %b11
  %calltmp13 = call i64 @testFunction(i64 8, i64 %addtmp12)
  %0 = or i64 %multmp, %calltmp13
  %1 = and i64 %0, -4294967296
  %2 = icmp eq i64 %1, 0
  br i1 %2, label %3, label %8

3:                                                ; preds = %entry
  %4 = trunc i64 %calltmp13 to i32
  %5 = trunc i64 %multmp to i32
  %6 = udiv i32 %5, %4
  %7 = zext i32 %6 to i64
  br label %10

8:                                                ; preds = %entry
  %9 = sdiv i64 %multmp, %calltmp13
  br label %10

10:                                               ; preds = %8, %3
  %11 = phi i64 [ %7, %3 ], [ %9, %8 ]
  store i64 %11, ptr @c, align 4
  %a14 = load i64, ptr %a, align 4
  %b15 = load i64, ptr %b, align 4
  %eqtmp = icmp eq i64 %a14, %b15
  br i1 %eqtmp, label %then, label %else

then:                                             ; preds = %10
  %a16 = load i64, ptr %a, align 4
  %b17 = load i64, ptr %b, align 4
  %addtmp18 = add i64 %a16, %b17
  store i64 %addtmp18, ptr @c, align 4
  br label %mergeBlock

else:                                             ; preds = %10
  %a19 = load i64, ptr %a, align 4
  %b20 = load i64, ptr %b, align 4
  %gttmp = icmp sgt i64 %a19, %b20
  br i1 %gttmp, label %then21, label %else22

mergeBlock:                                       ; preds = %then21, %else22, %then
  %c = load i64, ptr @c, align 4
  %calltmp29 = call void @printInt(i64 %c)
  br label %while_condition

then21:                                           ; preds = %else
  %a24 = load i64, ptr %a, align 4
  %b25 = load i64, ptr %b, align 4
  %subtmp = sub i64 %a24, %b25
  store i64 %subtmp, ptr @c, align 4
  br label %mergeBlock

else22:                                           ; preds = %else
  %a26 = load i64, ptr %a, align 4
  %b27 = load i64, ptr %b, align 4
  %multmp28 = mul i64 %a26, %b27
  store i64 %multmp28, ptr @c, align 4
  br label %mergeBlock

while_condition:                                  ; preds = %mergeBlock38, %while_loop, %mergeBlock
  %c30 = load i64, ptr @c, align 4
  %gttmp31 = icmp sgt i64 %c30, 0
  br i1 %gttmp31, label %while_loop, label %while_after

while_loop:                                       ; preds = %while_condition
  %c32 = load i64, ptr @c, align 4
  %divtmp33 = sdiv i64 %c32, 2
  store i64 %divtmp33, ptr @c, align 4
  %c34 = load i64, ptr @c, align 4
  %gttmp35 = icmp sgt i64 %c34, 10
  br i1 %gttmp35, label %while_condition, label %mergeBlock38

while_after:                                      ; preds = %while_condition
  %i = alloca i64, align 8
  store i64 0, ptr %i, align 4
  br label %for_condition

mergeBlock38:                                     ; preds = %while_loop
  %c39 = load i64, ptr @c, align 4
  %calltmp40 = call void @printInt(i64 %c39)
  br label %while_condition

for_condition:                                    ; preds = %for_increment, %while_after
  %i41 = load i64, ptr %i, align 4
  %lttmp = icmp slt i64 %i41, 10
  br i1 %lttmp, label %for_loop, label %for_after

for_loop:                                         ; preds = %for_condition
  %i42 = load i64, ptr %i, align 4
  %eqtmp43 = icmp eq i64 %i42, 5
  br i1 %eqtmp43, label %for_after, label %mergeBlock46

for_increment:                                    ; preds = %mergeBlock46, %mergeBlock51
  %i54 = load i64, ptr %i, align 4
  %addtmp55 = add i64 %i54, 1
  store i64 %addtmp55, ptr %i, align 4
  br label %for_condition

for_after:                                        ; preds = %for_loop, %for_condition
  %calltmp56 = call i64 @fib(i64 10)
  %calltmp57 = call void @printInt(i64 %calltmp56)
  ret i64 0

mergeBlock46:                                     ; preds = %for_loop
  %i47 = load i64, ptr %i, align 4
  %eqtmp48 = icmp eq i64 %i47, 3
  br i1 %eqtmp48, label %for_increment, label %mergeBlock51

mergeBlock51:                                     ; preds = %mergeBlock46
  %i52 = load i64, ptr %i, align 4
  %calltmp53 = call void @printInt(i64 %i52)
  br label %for_increment
}
