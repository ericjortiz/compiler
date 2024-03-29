.globl main
.data
.text
h:
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
addi sp sp -4
addi sp sp -4
addi s1 fp 4
lw s1 0(s1)
sw s1 0(sp)
addi s1 fp 8
lw s1 0(s1)
lw a0 0(sp)
addi sp sp 4
mul s1 a0 s1
sw s1 0(sp)
addi sp sp -4
addi sp sp -4
addi s1 fp 12
lw s1 0(s1)
sw s1 0(sp)
addi s1 fp 16
lw s1 0(s1)
lw a0 0(sp)
addi sp sp 4
mul s1 a0 s1
sw s1 0(sp)
addi s1 fp 20
lw s1 0(s1)
lw a0 0(sp)
addi sp sp 4
div s1 a0 s1
lw a0 0(sp)
addi sp sp 4
add s1 s1 a0
mv a0 s1
j _h
_h:
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
jr ra
main:
addi sp sp -4
sw fp 0(sp)
mv fp sp
addi sp sp -48
sw s1 -4(fp)
sw s2 -8(fp)
sw s3 -12(fp)
sw s4 -16(fp)
sw s5 -20(fp)
sw s6 -24(fp)
sw s7 -28(fp)
sw s8 -32(fp)
sw s9 -36(fp)
sw s10 -40(fp)
sw s11 -44(fp)
sw ra -48(fp)
addi sp sp -20
addi s1 x0 3
sw s1 16(sp)
addi s1 x0 5
sw s1 12(sp)
addi s1 x0 4
sw s1 8(sp)
addi s1 x0 2
sw s1 4(sp)
addi s1 x0 10
sw s1 0(sp)
jal ra h
mv s1 a0
addi sp sp 20
mv a0 s1
j _main
_main:
lw s1 -4(fp)
lw s2 -8(fp)
lw s3 -12(fp)
lw s4 -16(fp)
lw s5 -20(fp)
lw s6 -24(fp)
lw s7 -28(fp)
lw s8 -32(fp)
lw s9 -36(fp)
lw s10 -40(fp)
lw s11 -44(fp)
lw ra -48(fp)
mv sp fp
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
  
