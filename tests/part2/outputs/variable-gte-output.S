.globl main
.data
.text
main:
addi sp sp -4
sw fp 0(sp)
mv fp sp
addi sp sp -48
sw s1 0(sp)
sw s2 4(sp)
sw s3 8(sp)
sw s4 12(sp)
sw s5 16(sp)
sw s6 20(sp)
sw s7 24(sp)
sw s8 28(sp)
sw s9 32(sp)
sw s10 36(sp)
sw s11 40(sp)
sw ra 44(sp)
addi sp sp -8
li s1 6
sw s1 -52(fp)
li s1 7
sw s1 -56(fp)
addi sp sp -4
addi s1 fp -52
lw s1 0(s1)
sw s1 0(sp)
addi s1 fp -56
lw s1 0(s1)
lw a0 0(sp)
addi sp sp 4
beq a0 s1 __label__0
slt s1 s1 a0
beq s1 x0 __label__1
j __label__0
__label__1:
add s1 x0 x0
j __label__2
__label__0:
addi s1 x0 1
__label__2:
mv a0 s1
j _main
addi sp sp 8
_main:
addi sp fp -48
lw ra 44(sp)
lw s11 40(sp)
lw s10 36(sp)
lw s9 32(sp)
lw s8 28(sp)
lw s7 24(sp)
lw s6 20(sp)
lw s5 16(sp)
lw s4 12(sp)
lw s3 8(sp)
lw s2 4(sp)
lw s1 0(sp)
addi sp sp 48
lw fp 0(sp)
addi sp sp 4
mv a1 a0
addi a0 x0 17
ecall
PrintString:
  addi sp sp -4
  sw ra 0(sp)
  lw a1 4(sp) 
  addi a0 x0 4
  ecall
  jal PrintNewline
  lw ra 0(sp)
  addi sp sp 4
  jr ra

PrintInt:
  addi sp sp -4
  sw ra 0(sp)
  lw a1 4(sp) 
  addi a0 x0 1
  ecall
  jal PrintNewline
  lw ra 0(sp)
  addi sp sp 4
  jr ra

PrintChar:
  addi sp sp -4
  sw ra 0(sp)
  lb a1 4(sp)
  andi a1 a1 0xff 
  addi a0 x0 11
  ecall
  jal PrintNewline
  lw ra 0(sp)
  addi sp sp 4
  jr ra

PrintBool:
  addi sp sp -4
  sw ra 0(sp)
  lw a1 4(sp) 
  addi a0 x0 1
  ecall
  jal PrintNewline
  lw ra 0(sp)
  addi sp sp 4
  jr ra

PrintNewline:
  addi a1 x0 10 #Load newline char
  addi a0 x0 11
  ecall
  mv a0 x0
  jr ra

PrintInts:
  addi sp sp -8
  sw ra 4(sp)
  lw t0 8(sp)
  sw t0 0(sp)
  jal PrintInt
  lw t0 12(sp)
  sw t0 0(sp)
  jal PrintInt
  lw t0 16(sp)
  sw t0 0(sp)
  jal PrintInt
  lw ra 4(sp)
  addi sp sp 8
  jr ra

PrintChars:
  addi sp sp -8
  sw ra 4(sp)
  lb t0 8(sp)
  sb t0 0(sp)
  jal PrintChar
  lb t0 9(sp)
  sb t0 0(sp)
  jal PrintChar
  lb t0 10(sp)
  sb t0 0(sp)
  jal PrintChar
  lw ra 4(sp)
  addi sp sp 8
  jr ra
  

PrintMixed:
  addi sp sp -8
  sw ra 4(sp)
  lb t0 8(sp)
  sb t0 0(sp)
  jal PrintChar
  lb t0 9(sp)
  sb t0 0(sp)
  jal PrintChar
  lw t0 12(sp)
  sw t0 0(sp)
  jal PrintInt
  lw t0 16(sp)
  sw t0 0(sp)
  jal PrintInt
  lb t0 20(sp)
  sb t0 0(sp)
  jal PrintChar
  lw t0 24(sp)
  sw t0 0(sp)
  jal PrintInt
  lw ra 4(sp)
  addi sp sp 8
  jr ra
  
