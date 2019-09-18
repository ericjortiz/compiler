li a0 0 # Replace ??? with your register choice from part 1
li s1 0
jal LogOr
jal PrintRes

li a0 4 # Replace ??? with your register choice from part 1
li s1 0
jal LogOr
jal PrintRes

li a0 0 # Replace ??? with your register choice from part 1
li s1 -1
jal LogOr
jal PrintRes

li a0 2 # Replace ??? with your register choice from part 1
li s1 1
jal LogOr
jal PrintRes

li a0 2 # Replace ??? with your register choice from part 1
li s1 2
jal LogOr
jal PrintRes

li a0 12 # Replace ??? with your register choice from part 1
li s1 9
jal LogOr
jal PrintRes

li a0 0 # Replace ??? with your register choice from part 1
li s1 -0
jal LogOr
jal PrintRes

jal Exit

LogOr:
  # Place your Code here for logical or. Remember this is NOT a function call
  # You should perform all of your operations on the s1 and ??? register. You
  # should also assume that these are the only two values to operate on
  # (no nested expressions)


  bne a0, x0, next

  then:
  bne s1, x0, good

  thenn:
  add s1, x0, x0
  j end

  next:
  addi s1, x0, 1

  good:
  addi s1, x0, 1

  end:

  # End of your code
  jr ra

PrintRes:
  mv a1 s1
  li a0 1
  ecall
  jr ra

Exit:
  li a0 10
  ecall
