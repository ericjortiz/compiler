li a0 0 # Replace ??? with your register choice from part 1
li s1 0
jal Lt
jal PrintRes

li a0 4 # Replace ??? with your register choice from part 1
li s1 0
jal Lt
jal PrintRes

# ADD MORE TESTS HERE FOLLOWING THE STRUCTURE ABOVE

li a0 9 # Replace ??? with your register choice from part 1
li s1 0
jal Lt
jal PrintRes

li a0 4 # Replace ??? with your register choice from part 1
li s1 8
jal Lt
jal PrintRes

li a0 4 # Replace ??? with your register choice from part 1
li s1 4
jal Lt
jal PrintRes

jal Exit

Lt:
  # Place your Code here for <. Remember this is NOT a function call
  # You should perform all of your operations on the s1 and ??? register. You
  # should also assume that these are the only two values to operate on
  # (no nested expressions)

  slt s1, s1, a0

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
