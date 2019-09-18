li a0 0 # Replace ??? with your register choice from part 1
li s1 0
jal Gte
jal PrintRes

li a0 4 # Replace ??? with your register choice from part 1
li s1 0
jal Gte
jal PrintRes

# ADD MORE TESTS HERE FOLLOWING THE STRUCTURE ABOVE


li a0 4 # Replace ??? with your register choice from part 1
li s1 8
jal Gte
jal PrintRes

li a0 8 # Replace ??? with your register choice from part 1
li s1 8
jal Gte
jal PrintRes

jal Exit

Gte:
  # Place your Code here for >=. Remember this is NOT a function call
  # You should perform all of your operations on the s1 and ??? register. You
  # should also assume that these are the only two values to operate on
  # (no nested expressions)

  bge s1, a0, else
  then:
    add s1, x0, x0
    j end
  else:
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
