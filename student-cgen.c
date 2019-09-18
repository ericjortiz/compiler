#include "additional-instructions.h"
#include "instructions.h"
#include "student-cgen.h"
#include "cgen.h"
#include "cgen-helpers.h"
#include "decorate-builtins.h"
#include "utils.h"

/*
 *  Code used to generate the negate expression. This is complete and is
 *  included to provide you an example of code generation.
 *
 *  Note how it calls dispatch on its child to recurse into generate code
 *  that needs to be negated. Then it cause apply its operation to the value
 *  of register S1 because we assume our most recent output is always in S1.
 *  Finally it places it own result in S1.
 */
void ProcessExprPrefixNegate(DAST* dast,
                             char* startLabel,
                             char* endLabel, int regBytes) {
  DAST* child1 = dast->children[0];

  /* cgen and load arguments */
  Dispatch(child1, startLabel, endLabel, regBytes);
  /* Negate by flipping all the bits and add 1. */
  EmitXORI(S1, S1, -1);
  EmitADDI(S1, S1, 1);

  /* Make sure we squash any overflow for future operations. */
  ApplyTypeRules (dast, S1);
}

/*
 *  Helper function used to ease maintaining our latest value in S1 invariant
 *  for binary expression. It returns an enum value which contains the value of
 *  the register used to hold a value the first of the two expresions,
 *  (the second is defined to be in s1).
 *
 *  You should use the enum values in instructions.h and be sure and restore the
 *  stack to its previous value upon returning from the function
 *
 *   Hint: When using lw/sw the argument order is Reg, imm, Reg and it matches
 *
 *   lw register immediate(register)
 */
enum reg SetupBinaryInvariant(DAST* dast, char* startLabel, char* endLabel, int regBytes) {
  DAST* child1 = dast->children[0];
  DAST* child2 = dast->children[1];
  /* STUDENT CODE GOES HERE. */

  EmitADDI(SP, SP, -4);

  Dispatch(child1, startLabel, endLabel, regBytes);
  EmitSW(S1, 0, SP);
  Dispatch(child2, startLabel, endLabel, regBytes);
  EmitLW(A0, 0, SP);

  EmitADDI(SP, SP, 4);

  ApplyTypeRules(dast, S1);
  /*
   *  Replace this return value with the register you load the first arg into.
   */
  return A0;
}

/*
 *  Function used to handle code generation for all & expressions. As with all
 *  binary expressions it relies on the binary invariant completed in part 1.
 *
 *  No type checking should be necessary for this operator.
 */
void ProcessExprBinaryBitAnd(DAST* dast,
                             char* startLabel,
                             char* endLabel, int regBytes) {

  DAST* child1 = dast->children[0];
  DAST* child2 = dast->children[1];
  /* First apply our invariant. */
  enum reg firstReg = SetupBinaryInvariant (dast, startLabel, endLabel, regBytes);
  /* Make sure type sizes match. */
  MatchSizes (child1, firstReg, child2, S1);
  /* STUDENT CODE HERE. */
  EmitAND(S1, firstReg, S1);
  /* Make sure we squash any overflow for future operations. */
  ApplyTypeRules (dast, S1);
}

/*
 *  Function used to handle code generation for all | expressions. As with all
 *  binary expressions it relies on the binary invariant completed in part 1.
 *
 *  No type checking should be necessary for this operator.
 */
void ProcessExprBinaryBitOr(DAST* dast,
                            char* startLabel,
                            char* endLabel, int regBytes) {

  DAST* child1 = dast->children[0];
  DAST* child2 = dast->children[1];
  /* First apply our invariant. */
  enum reg firstReg = SetupBinaryInvariant (dast, startLabel, endLabel, regBytes);
  /* Make sure type sizes match. */
  MatchSizes (child1, firstReg, child2, S1);
  /* STUDENT CODE HERE. */
  EmitOR(S1, S1, firstReg);
  /* Make sure we squash any overflow for future operations. */
  ApplyTypeRules (dast, S1);
}

/*
 *  Function used to handle code generation for all ^ expressions. As with all
 *  binary expressions it relies on the binary invariant completed in part 1.
 *
 *  No type checking should be necessary for this operator.
 */
void ProcessExprBinaryBitXor(DAST* dast,
                             char* startLabel,
                             char* endLabel, int regBytes) {

  DAST* child1 = dast->children[0];
  DAST* child2 = dast->children[1];
  /* First apply our invariant. */
  enum reg firstReg = SetupBinaryInvariant (dast, startLabel, endLabel, regBytes);
  /* Make sure type sizes match. */
  MatchSizes (child1, firstReg, child2, S1);
  /* STUDENT CODE HERE. */
  EmitXOR(S1, S1, firstReg);
  /* Make sure we squash any overflow for future operations. */
  ApplyTypeRules (dast, S1);
}

/*
 *  Function used to handle code generation for all && expressions. As with all
 *  binary expressions it relies on the binary invariant completed in part 1.
 *
 *  An operand is considered to have a true value if it is nonzero and a false
 *  value if it is 0.
 *
 *  If the condition is met then you should produce 1. If the condition is not
 *  met not then you should produce 0. Failure to return either of these two
 *  values exactly may result in a loss of credit, regardless of the truthiness
 *  of the values returned being correct.
 */
void ProcessExprBinaryLogicAnd(DAST* dast,
                               char* startLabel,
                               char* endLabel, int regBytes) {

  /* First apply our invariant. */
  enum reg firstReg = SetupBinaryInvariant (dast, startLabel, endLabel, regBytes);
  /* STUDENT CODE HERE. */
  char* false_label = GenerateLocalLabel();
  char* end_label   = GenerateLocalLabel();

  EmitBEQ(firstReg, X0, false_label);
  EmitBEQ(S1, X0, false_label);
  EmitADDI(S1, X0, 1);
  EmitJ(end_label);

  EmitLABEL(false_label);
  EmitADD(S1, X0, X0);

  EmitLABEL(end_label);

  ApplyTypeRules(dast, S1);
  free(false_label);
  free(end_label);
}

/*
 *  Function used to handle code generation for all || expressions. As with all
 *  binary expressions it relies on the binary invariant completed in part 1.
 *
 *  An operand is considered to have a true value if it is nonzero and a false
 *  value if it is 0.
 *
 *  If the condition is met then you should produce 1. If the condition is not
 *  met not then you should produce 0. Failure to return either of these two
 *  values exactly may result in a loss of credit, regardless of the truthiness
 *  of the values returned being correct.
 */
void ProcessExprBinaryLogicOr(DAST* dast,
                              char* startLabel,
                              char* endLabel, int regBytes) {

  /* First apply our invariant. */
  enum reg firstReg = SetupBinaryInvariant (dast, startLabel, endLabel, regBytes);
  /* STUDENT CODE HERE. */

  char* success_label = GenerateLocalLabel();
  char* false_label = GenerateLocalLabel();
  char* end_label   = GenerateLocalLabel();

  EmitBNE(firstReg, X0, success_label);
  EmitBNE(S1, X0, success_label);

  EmitLABEL(false_label);
  EmitADD(S1, X0, X0);
  EmitJ(end_label);

  EmitLABEL(success_label);
  EmitADDI(S1, X0, 1);

  EmitLABEL(end_label);

  ApplyTypeRules(dast, S1);
  free(success_label);
  free(false_label);
  free(end_label);
}

/*
 *  Function used to handle code generation for all == expressions. As with all
 *  binary expressions it relies on the binary invariant completed in part 1.
 *
 *  For equality you should not need to consider the types of the operands at all
 *  and simply need to determine if the results are equal.
 *
 *  If the condition is met then you should produce 1. If the condition is not
 *  met not then you should produce 0. Failure to return either of these two
 *  values exactly may result in a loss of credit, regardless of the truthiness
 *  of the values returned being correct.
 */
void ProcessExprBinaryEq(DAST* dast,
                         char* startLabel,
                         char* endLabel, int regBytes) {

  DAST* child1 = dast->children[0];
  DAST* child2 = dast->children[1];
  /* First apply our invariant. */
  enum reg firstReg = SetupBinaryInvariant (dast, startLabel, endLabel, regBytes);
  /* Make sure type sizes match. */
  MatchSizes (child1, firstReg, child2, S1);
  /* STUDENT CODE HERE. */
  //Decl* child1_type = dast->children[0]->typeDecl;
  //Decl* child2_type = dast->children[1]->typeDecl;

  char* false_label = GenerateLocalLabel();
  char* end_label   = GenerateLocalLabel();

  //EmitBNE(child1_type, child2_type, false_label);
  EmitBNE(firstReg, S1, false_label);
  EmitADDI(S1, X0, 1);
  EmitJ(end_label);

  EmitLABEL(false_label);
  EmitADD(S1, X0, X0);

  EmitLABEL(end_label);
  ApplyTypeRules(dast, S1);
  free(false_label);
  free(end_label);
}

/*
 *  Function used to handle code generation for all != expressions. As with all
 *  binary expressions it relies on the binary invariant completed in part 1.
 *
 *  For equality you should not need to consider the types of the operands at all
 *  and simply need to determine if the results are equal.
 *
 *  If the condition is met then you should produce 1. If the condition is not
 *  met not then you should produce 0. Failure to return either of these two
 *  values exactly may result in a loss of credit, regardless of the truthiness
 *  of the values returned being correct.
 */
void ProcessExprBinaryNotEq(DAST* dast,
                            char* startLabel,
                            char* endLabel, int regBytes) {

  DAST* child1 = dast->children[0];
  DAST* child2 = dast->children[1];
  /* First apply our invariant. */
  enum reg firstReg = SetupBinaryInvariant (dast, startLabel, endLabel, regBytes);
  /* Make sure type sizes match. */
  MatchSizes (child1, firstReg, child2, S1);

  /* STUDENT CODE HERE. */
  //Decl* child1_type = dast->children[0]->typeDecl;
  //Decl* child2_type = dast->children[1]->typeDecl;

  char* true_label = GenerateLocalLabel();
  char* end_label   = GenerateLocalLabel();

  //EmitBNE(child1_type, child2_type, true_label);
  EmitBNE(firstReg, S1, true_label);
  EmitADD(S1, X0, X0);
  EmitJ(end_label);

  EmitLABEL(true_label);
  EmitADDI(S1, X0, 1);

  EmitLABEL(end_label);
  ApplyTypeRules(dast, S1);
  free(true_label);
  free(end_label);
}

/*
 *  Function used to handle code generation for all add expressions. As with all
 *  binary expressions it relies on the binary invariant completed in part 1.
 *
 *  Add needs to handle both normal arithmetic and pointer arithmetic. As a
 *  refresher in C adding to a pointer increments the address by one
 *  element in size. For example if we have
 *
 *  x + 1
 *
 *  where x is an int *, then x + 1 produces x + sizeof (int) * 1 in assembly.
 *
 *  To handle this case you should use c1PtrTotal and c2PtrTotal to determine
 *  if either child is a pointer (only 1 will ever be a pointer and you do not
 *  need to check this condition). Then you will need to determine the sizeof
 *  amount. Our compiler organizes types as having a typeDecl, which contains
 *  a base type (like int), and a pointer counter, which roughly indicates how
 *  many *s there are. To find the size of the base type, you should look at
 *  the child1/2->typeDecl->dataSize.
 */
void ProcessExprBinaryAdd(DAST* dast,
                          char* startLabel,
                          char* endLabel, int regBytes) {

  DAST* child1   = dast->children[0];
  int c1PtrTotal = child1->typeDeclArrCount + child1->typeDeclPtrCount;
  DAST* child2   = dast->children[1];
  int c2PtrTotal = child2->typeDeclArrCount + child2->typeDeclPtrCount;
  /* First apply our invariant. */
  enum reg firstReg = SetupBinaryInvariant (dast, startLabel, endLabel, regBytes);
  /* Make sure type sizes match. */
  MatchSizes (child1, firstReg, child2, S1);
  /* STUDENT CODE HERE. */

  if (c1PtrTotal > 0) { /* child 1 is the pointer, meaning S1 is an int */
    int dataSize = child1->typeDecl->dataSize;
    EmitADD(T0, X0, S1);
    EmitADDI(T1, X0, dataSize);
    EmitMUL(T1, T1, T0);
    EmitADD(S1, firstReg, T1);
  } else if (c2PtrTotal > 0) { /* child 2 is the pointer, meaning A0 is an int */
    int dataSize = child2->typeDecl->dataSize;
    EmitADD(T0, X0, firstReg);
    EmitADDI(T1, X0, dataSize);
    EmitMUL(T1, T1, T0);
    EmitADD(S1, S1, T1);
  } else {  /* normal addition */
    EmitADD(S1, S1, firstReg);
  }
  /* Make sure we squash any overflow for future operations. */
  ApplyTypeRules (dast, S1);
}

/*
 *  Function used to handle code generation for all sub expressions. As with all
 *  binary expressions it relies on the binary invariant completed in part 1.
 *
 *  Sub needs to handle both normal subtraction, pointer arithmetic and
 *  subtracting one pointer from another. As a refresher in C adding to a pointer
 *  decrements the address by one element in size. For example if we have
 *
 *  x - 1
 *
 *  where x is an int *, then x - 1 produces x - sizeof (int) * 1 in assembly.
 *
 *  To handle this case you should use c1PtrTotal and c2PtrTotal to determine
 *  if either child is a pointer. Then you will need to determine the sizeof
 *  amount. Our compiler organizes types as having a typeDecl, which contains
 *  a base type (like int), and a pointer counter, which roughly indicates how
 *  many *s there are. To find the size of the base type, you should look at
 *  the child1->typeDecl->dataSize.
 *
 *  In addition you also need to consider the case where two pointers are
 *  being subtracted from each other. For example:
 *
 *  (x + 2) - x = 2
 *
 *  regardless of the actual distance between the addresses in x and x + 2.
 *  Once again you will need the pointer count and/or the dataSize to complete
 *  this step.
 */
void ProcessExprBinarySub(DAST* dast,
                          char* startLabel,
                          char* endLabel, int regBytes) {

  DAST* child1   = dast->children[0];
  int c1PtrTotal = child1->typeDeclArrCount + child1->typeDeclPtrCount;
  DAST* child2   = dast->children[1];
  int c2PtrTotal = child2->typeDeclArrCount + child2->typeDeclPtrCount;
  /* First apply our invariant. */
  enum reg firstReg = SetupBinaryInvariant (dast, startLabel, endLabel, regBytes);
  /* Make sure type sizes match. */
  MatchSizes (child1, firstReg, child2, S1);
  /* STUDENT CODE HERE. */
  if (c1PtrTotal > 0 && c2PtrTotal == 0) { /* child 1 is the pointer, S1 is an int */
    int dataSize = child1->typeDecl->dataSize;
    EmitADD(T0, X0, S1);
    EmitADDI(T1, X0, dataSize);
    EmitMUL(T1, T1, T0);
    EmitSUB(S1, firstReg, T1);
  } else if (c2PtrTotal > 0 && c1PtrTotal == 0) { /* child 2 is the pointer, A0 is an int */
    int dataSize = child2->typeDecl->dataSize;
    EmitADD(T0, X0, firstReg);
    EmitADDI(T1, X0, dataSize);
    EmitMUL(T1, T1, T0);
    EmitSUB(S1, S1, T1);
  } else if (c2PtrTotal > 0 && c1PtrTotal > 0) { /* A0 and S1 are pointers */
    int dataSize = child1->typeDecl->dataSize;
    EmitADDI(T0, X0, dataSize);
    EmitSUB(S1, firstReg, S1);
    EmitDIV(S1, S1, T0);
  } else {  /* normal addition */
    EmitSUB(S1, firstReg, S1);
  }
  /* Make sure we squash any overflow for future operations. */
  ApplyTypeRules (dast, S1);
}

/*
 *  Function used to handle code generation for all >= expressions. As with all
 *  binary expressions it relies on the binary invariant completed in part 1.
 *
 *  For comparison operations the type of the operator is important for selecting
 *  the correct comparison instruction. In particular we are concerned with two
 *  types of comparisons:
 * signed
 * unsigned
 *
 *  To determine when to select which type of comparison, we have two conditions.
 *  First if we are working with pointers, both pointers must be the same type
 *  and c1PtrTotal/c2PtrTotal will be > 0. All pointer comparisons are unsigned.
 *  The other possibility is that we are working with an integer type. There are
 *  5 integer types and they can be determined by comparing child1/2->typedecl
 *  directly with some global variables (use ==). These types are:
 *
 *   1. unsigned int (global variable: uintType), unsigned
 *   2. int (global variable: intType), signed
 *   3. unsigned char (global variable: ucharType), unsigned
 *   4. char (global variable: charType), signed
 *   5. NULL (global variable: nullType), signed/unsigned (doesn't matter)
 *
 *  For our comparisons we select the signedness (signed or unsigned) of the
 *  highest priority operand (unsigned int is highest, NULL is lowest). So for
 *  example if we had:
 *
 *  x >= y
 *
 *  where x is a char and y is an unsigned int, then we would do an unsigned
 *  comparison because unsigned int is higher priority than char. Do not worry
 *  about making the sizes of the operands the same, this is handled for you by
 *  the starter code.
 *
 *  If the condition is met then you should produce 1. If the condition is not
 *  met not then you should produce 0. Failure to return either of these two
 *  values exactly may result in a loss of credit, regardless of the truthiness
 *  of the values returned being correct.
 */
void ProcessExprBinaryGTEq(DAST* dast,
                           char* startLabel,
                           char* endLabel, int regBytes) {

  DAST* child1   = dast->children[0];
  int c1PtrTotal = child1->typeDeclArrCount + child1->typeDeclPtrCount;
  DAST* child2   = dast->children[1];
  int c2PtrTotal = child2->typeDeclArrCount + child2->typeDeclPtrCount;
  /* First apply our invariant. */
  enum reg firstReg = SetupBinaryInvariant (dast, startLabel, endLabel, regBytes);
  /* Make sure type sizes match. */
  MatchSizes (child1, firstReg, child2, S1);

  /* STUDENT CODE HERE. */

  //Decl* child1_type = dast->children[0]->typeDecl;
  //Decl* child2_type = dast->children[1]->typeDecl;

  if (c1PtrTotal > 0 && c2PtrTotal > 0) { /* we are working with pointers */
    char* trueLabel  = GenerateLocalLabel();
    char* falseLabel = GenerateLocalLabel();
    char* endLabel   = GenerateLocalLabel();
    EmitBEQ(firstReg, S1, trueLabel);

    EmitSLTU(S1, S1, firstReg);
    EmitBEQ(S1, X0, falseLabel);
    EmitJ(trueLabel);

    EmitLABEL(falseLabel);
    EmitADD(S1, X0, X0);
    EmitJ(endLabel);

    EmitLABEL(trueLabel);
    EmitADDI(S1, X0, 1);

    EmitLABEL(endLabel);
    free(trueLabel);
    free(falseLabel);
    free(endLabel);
  } else {
    char* trueLabel = GenerateLocalLabel();
    char* falseLabel = GenerateLocalLabel();
    char* endLabel   = GenerateLocalLabel();
    int highest; /* true for signed, false for unsigned */
    Decl* child1_type = child1->typeDecl;
    Decl* child2_type = child2->typeDecl;

    if (child1_type == uintType || child2_type == uintType) { /* if highest priority, unsigned */
      highest = 0;
    } else if (child1_type == intType || child2_type == intType) { /* if second highest priority, signed */
      highest = 1;
    } else if (child1_type == ucharType || child2_type == ucharType) { /* if third highest priority, unsigned */
      highest = 0;
    } else { /* either 4th lowest priority or lowest priority, call it signed either way*/
      highest = 1;
    }
    if (highest) {
      EmitBEQ(firstReg, S1, trueLabel);

      EmitSLT(S1, S1, firstReg);
      EmitBEQ(S1, X0, falseLabel);
      EmitJ(trueLabel);

      EmitLABEL(falseLabel);
      EmitADD(S1, X0, X0);
      EmitJ(endLabel);

      EmitLABEL(trueLabel);
      EmitADDI(S1, X0, 1);

      EmitLABEL(endLabel);
    } else {
      EmitBEQ(firstReg, S1, trueLabel);

      EmitSLTU(S1, S1, firstReg);
      EmitBEQ(S1, X0, falseLabel);
      EmitJ(trueLabel);

      EmitLABEL(falseLabel);
      EmitADD(S1, X0, X0);
      EmitJ(endLabel);

      EmitLABEL(trueLabel);
      EmitADDI(S1, X0, 1);

      EmitLABEL(endLabel);
    }
    free(trueLabel);
    free(falseLabel);
    free(endLabel);
  }
  ApplyTypeRules(dast, S1);
}

/*
 *  Function used to handle code generation for all > expressions. As with all
 *  binary expressions it relies on the binary invariant completed in part 1.
 *
 *  For comparison operations the type of the operator is important for selecting
 *  the correct comparison instruction. In particular we are concerned with two
 *  types of comparisons:
 * signed
 * unsigned
 *
 *  To determine when to select which type of comparison, we have two conditions.
 *  First if we are working with pointers, both pointers must be the same type
 *  and c1PtrTotal/c2PtrTotal will be > 0. All pointer comparisons are unsigned.
 *  The other possibility is that we are working with an integer type. There are
 *  5 integer types and they can be determined by comparing child1/2->typedecl
 *  directly with some global variables (use ==). These types are:
 *
 *   1. unsigned int (global variable: uintType), unsigned
 *   2. int (global variable: intType), signed
 *   3. unsigned char (global variable: ucharType), unsigned
 *   4. char (global variable: charType), signed
 *   5. NULL (global variable: nullType), signed/unsigned (doesn't matter)
 *
 *  For our comparisons we select the signedness (signed or unsigned) of the
 *  highest priority operand (unsigned int is highest, NULL is lowest). So for
 *  example if we had:
 *
 *  x > y
 *
 *  where x is a char and y is an unsigned int, then we would do an unsigned
 *  comparison because unsigned int is higher priority than char. Do not worry
 *  about making the sizes of the operands the same, this is handled for you by
 *  the starter code.
 *
 *  If the condition is met then you should produce 1. If the condition is not
 *  met not then you should produce 0. Failure to return either of these two
 *  values exactly may result in a loss of credit, regardless of the truthiness
 *  of the values returned being correct.
 */
void ProcessExprBinaryGT(DAST* dast,
                         char* startLabel,
                         char* endLabel, int regBytes) {

  DAST* child1   = dast->children[0];
  int c1PtrTotal = child1->typeDeclArrCount + child1->typeDeclPtrCount;
  DAST* child2   = dast->children[1];
  int c2PtrTotal = child2->typeDeclArrCount + child2->typeDeclPtrCount;
  /* First apply our invariant. */
  enum reg firstReg = SetupBinaryInvariant (dast, startLabel, endLabel, regBytes);
  /* Make sure type sizes match. */
  MatchSizes (child1, firstReg, child2, S1);
  /* STUDENT CODE HERE. */
  //Decl* child1_type = dast->children[0]->typeDecl;
  //Decl* child2_type = dast->children[1]->typeDecl;

  if (c1PtrTotal > 0 && c2PtrTotal > 0) { /* we are working with pointers */
    char* trueLabel  = GenerateLocalLabel();
    char* falseLabel = GenerateLocalLabel();
    char* endLabel   = GenerateLocalLabel();

    EmitSLTU(S1, S1, firstReg);
    EmitBEQ(S1, X0, falseLabel);
    EmitJ(trueLabel);

    EmitLABEL(falseLabel);
    EmitADD(S1, X0, X0);
    EmitJ(endLabel);

    EmitLABEL(trueLabel);
    EmitADDI(S1, X0, 1);

    EmitLABEL(endLabel);
    free(trueLabel);
    free(falseLabel);
    free(endLabel);
  } else {
    char* trueLabel = GenerateLocalLabel();
    char* falseLabel = GenerateLocalLabel();
    char* endLabel   = GenerateLocalLabel();
    int highest; /* true for signed, false for unsigned */
    Decl* child1_type = child1->typeDecl;
    Decl* child2_type = child2->typeDecl;

    if (child1_type == uintType || child2_type == uintType) { /* if highest priority, unsigned */
      highest = 0;
    } else if (child1_type == intType || child2_type == intType) { /* if second highest priority, signed */
      highest = 1;
    } else if (child1_type == ucharType || child2_type == ucharType) { /* if third highest priority, unsigned */
      highest = 0;
    } else { /* either 4th lowest priority or lowest priority, call it signed either way*/
      highest = 1;
    }
    if (highest) {
      EmitSLT(S1, S1, firstReg);
      EmitBEQ(S1, X0, falseLabel);
      EmitJ(trueLabel);

      EmitLABEL(trueLabel);
      EmitADDI(S1, X0, 1);
      EmitJ(endLabel);

      EmitLABEL(falseLabel);
      EmitADD(S1, X0, X0);

      EmitLABEL(endLabel);
    } else {
      EmitSLTU(S1, S1, firstReg);
      EmitBEQ(S1, X0, falseLabel);
      EmitJ(trueLabel);

      EmitLABEL(falseLabel);
      EmitADD(S1, X0, X0);
      EmitJ(endLabel);

      EmitLABEL(trueLabel);
      EmitADDI(S1, X0, 1);

      EmitLABEL(endLabel);
    }
    free(trueLabel);
    free(falseLabel);
    free(endLabel);
  }
  ApplyTypeRules(dast, S1);
}

/*
 *  Function used to handle code generation for all < expressions. As with all
 *  binary expressions it relies on the binary invariant completed in part 1.
 *
 *  For comparison operations the type of the operator is important for selecting
 *  the correct comparison instruction. In particular we are concerned with two
 *  types of comparisons:
 * signed
 * unsigned
 *
 *  To determine when to select which type of comparison, we have two conditions.
 *  First if we are working with pointers, both pointers must be the same type
 *  and c1PtrTotal/c2PtrTotal will be > 0. All pointer comparisons are unsigned.
 *  The other possibility is that we are working with an integer type. There are
 *  5 integer types and they can be determined by comparing child1/2->typedecl
 *  directly with some global variables (use ==). These types are:
 *
 *   1. unsigned int (global variable: uintType), unsigned
 *   2. int (global variable: intType), signed
 *   3. unsigned char (global variable: ucharType), unsigned
 *   4. char (global variable: charType), signed
 *   5. NULL (global variable: nullType), signed/unsigned (doesn't matter)
 *
 *  For our comparisons we select the signedness (signed or unsigned) of the
 *  highest priority operand (unsigned int is highest, NULL is lowest). So for
 *  example if we had:
 *
 *  x < y
 *
 *  where x is a char and y is an unsigned int, then we would do an unsigned
 *  comparison because unsigned int is higher priority than char. Do not worry
 *  about making the sizes of the operands the same, this is handled for you by
 *  the starter code.
 *
 *  If the condition is met then you should produce 1. If the condition is not
 *  met not then you should produce 0. Failure to return either of these two
 *  values exactly may result in a loss of credit, regardless of the truthiness
 *  of the values returned being correct.
 */
void ProcessExprBinaryLT(DAST* dast,
                         char* startLabel,
                         char* endLabel, int regBytes) {

  DAST* child1   = dast->children[0];
  int c1PtrTotal = child1->typeDeclArrCount + child1->typeDeclPtrCount;
  DAST* child2   = dast->children[1];
  int c2PtrTotal = child2->typeDeclArrCount + child2->typeDeclPtrCount;
  /* First apply our invariant. */
  enum reg firstReg = SetupBinaryInvariant (dast, startLabel, endLabel, regBytes);
  /* Make sure type sizes match. */
  MatchSizes (child1, firstReg, child2, S1);
  /* STUDENT CODE HERE. */
  //Decl* child1_type = dast->children[0]->typeDecl;
  //Decl* child2_type = dast->children[1]->typeDecl;

  if (c1PtrTotal > 0 && c2PtrTotal > 0) { /* we are working with pointers */
    char* trueLabel  = GenerateLocalLabel();
    char* falseLabel = GenerateLocalLabel();
    char* endLabel   = GenerateLocalLabel();

    EmitSLTU(S1, firstReg, S1);
    EmitBEQ(S1, X0, falseLabel);
    EmitJ(trueLabel);

    EmitLABEL(falseLabel);
    EmitADD(S1, X0, X0);
    EmitJ(endLabel);

    EmitLABEL(trueLabel);
    EmitADDI(S1, X0, 1);

    EmitLABEL(endLabel);
    free(trueLabel);
    free(falseLabel);
    free(endLabel);
  } else {
    char* trueLabel = GenerateLocalLabel();
    char* falseLabel = GenerateLocalLabel();
    char* endLabel   = GenerateLocalLabel();
    int highest; /* true for signed, false for unsigned */
    Decl* child1_type = child1->typeDecl;
    Decl* child2_type = child2->typeDecl;

    if (child1_type == uintType || child2_type == uintType) { /* if highest priority, unsigned */
      highest = 0;
    } else if (child1_type == intType || child2_type == intType) { /* if second highest priority, signed */
      highest = 1;
    } else if (child1_type == ucharType || child2_type == ucharType) { /* if third highest priority, unsigned */
      highest = 0;
    } else { /* either 4th lowest priority or lowest priority, call it signed either way*/
      highest = 1;
    }
    if (highest) {
      EmitSLT(S1, firstReg, S1);
      EmitBEQ(S1, X0, falseLabel);
      EmitJ(trueLabel);

      EmitLABEL(falseLabel);
      EmitADD(S1, X0, X0);
      EmitJ(endLabel);

      EmitLABEL(trueLabel);
      EmitADDI(S1, X0, 1);

      EmitLABEL(endLabel);
    } else {

      EmitSLTU(S1, firstReg, S1);
      EmitBEQ(S1, X0, falseLabel);
      EmitJ(trueLabel);

      EmitLABEL(falseLabel);
      EmitADD(S1, X0, X0);
      EmitJ(endLabel);

      EmitLABEL(trueLabel);
      EmitADDI(S1, X0, 1);

      EmitLABEL(endLabel);
    }
    free(trueLabel);
    free(falseLabel);
    free(endLabel);
  }
  ApplyTypeRules(dast, S1);
}

/*
 *  Function used to handle code generation for all <= expressions. As with all
 *  binary expressions it relies on the binary invariant completed in part 1.
 *
 *  For comparison operations the type of the operator is important for selecting
 *  the correct comparison instruction. In particular we are concerned with two
 *  types of comparisons:
 * signed
 * unsigned
 *
 *  To determine when to select which type of comparison, we have two conditions.
 *  First if we are working with pointers, both pointers must be the same type
 *  and c1PtrTotal/c2PtrTotal will be > 0. All pointer comparisons are unsigned.
 *  The other possibility is that we are working with an integer type. There are
 *  5 integer types and they can be determined by comparing child1/2->typedecl
 *  directly with some global variables (use ==). These types are:
 *
 *   1. unsigned int (global variable: uintType), unsigned
 *   2. int (global variable: intType), signed
 *   3. unsigned char (global variable: ucharType), unsigned
 *   4. char (global variable: charType), signed
 *   5. NULL (global variable: nullType), signed/unsigned (doesn't matter)
 *
 *  For our comparisons we select the signedness (signed or unsigned) of the
 *  highest priority operand (unsigned int is highest, NULL is lowest). So for
 *  example if we had:
 *
 *  x <= y
 *
 *  where x is a char and y is an unsigned int, then we would do an unsigned
 *  comparison because unsigned int is higher priority than char. Do not worry
 *  about making the sizes of the operands the same, this is handled for you by
 *  the starter code.
 *
 *  If the condition is met then you should produce 1. If the condition is not
 *  met not then you should produce 0. Failure to return either of these two
 *  values exactly may result in a loss of credit, regardless of the truthiness
 *  of the values returned being correct.
 */
void ProcessExprBinaryLTEq(DAST* dast,
                           char* startLabel,
                           char* endLabel, int regBytes) {

  DAST* child1   = dast->children[0];
  int c1PtrTotal = child1->typeDeclArrCount + child1->typeDeclPtrCount;
  DAST* child2   = dast->children[1];
  int c2PtrTotal = child2->typeDeclArrCount + child2->typeDeclPtrCount;
  /* First apply our invariant. */
  enum reg firstReg = SetupBinaryInvariant (dast, startLabel, endLabel, regBytes);
  /* Make sure type sizes match. */
  MatchSizes (child1, firstReg, child2, S1);

  /* STUDENT CODE HERE. */

  //Decl* child1_type = dast->children[0]->typeDecl;
  //Decl* child2_type = dast->children[1]->typeDecl;

  if (c1PtrTotal > 0 && c2PtrTotal > 0) { /* we are working with pointers */
    char* trueLabel  = GenerateLocalLabel();
    char* falseLabel = GenerateLocalLabel();
    char* endLabel   = GenerateLocalLabel();
    EmitBEQ(firstReg, S1, trueLabel);

    EmitSLTU(S1, firstReg, S1);
    EmitBEQ(S1, X0, falseLabel);
    EmitJ(trueLabel);

    EmitLABEL(falseLabel);
    EmitADD(S1, X0, X0);
    EmitJ(endLabel);

    EmitLABEL(trueLabel);
    EmitADDI(S1, X0, 1);

    EmitLABEL(endLabel);
    free(trueLabel);
    free(falseLabel);
    free(endLabel);
  } else {
    char* trueLabel = GenerateLocalLabel();
    char* falseLabel = GenerateLocalLabel();
    char* endLabel   = GenerateLocalLabel();
    int highest; /* true for signed, false for unsigned */
    Decl* child1_type = child1->typeDecl;
    Decl* child2_type = child2->typeDecl;

    if (child1_type == uintType || child2_type == uintType) { /* if highest priority, unsigned */
      highest = 0;
    } else if (child1_type == intType || child2_type == intType) { /* if second highest priority, signed */
      highest = 1;
    } else if (child1_type == ucharType || child2_type == ucharType) { /* if third highest priority, unsigned */
      highest = 0;
    } else { /* either 4th lowest priority or lowest priority, call it signed either way*/
      highest = 1;
    }
    if (highest) {
      EmitBEQ(firstReg, S1, trueLabel);

      EmitSLT(S1, firstReg, S1);
      EmitBEQ(S1, X0, falseLabel);
      EmitJ(trueLabel);

      EmitLABEL(falseLabel);
      EmitADD(S1, X0, X0);
      EmitJ(endLabel);

      EmitLABEL(trueLabel);
      EmitADDI(S1, X0, 1);

      EmitLABEL(endLabel);
    } else {
      EmitBEQ(firstReg, S1, trueLabel);

      EmitSLTU(S1, firstReg, S1);
      EmitBEQ(S1, X0, falseLabel);
      EmitJ(trueLabel);

      EmitLABEL(falseLabel);
      EmitADD(S1, X0, X0);
      EmitJ(endLabel);

      EmitLABEL(trueLabel);
      EmitADDI(S1, X0, 1);

      EmitLABEL(endLabel);
    }
    free(trueLabel);
    free(falseLabel);
    free(endLabel);
  }
  ApplyTypeRules(dast, S1);
}

/*
 *  Function used to generate call expressions. It decrements the stack to
 *  allocate space for the arguments, places the arguments on the stack and
 *  then switches control to the function it calls. On return it moves the
 *  output to our S1, our invariant register and restores the stack to its
 *  previous state.
 *
 *  To determine where to place arugments you should use the values in offsets
 *  and to determine how large of a value to store (either word or byte) you
 *  should use the results in sizes.
 */
void ProcessExprCall(DAST* dast, char* startLabel, char* endLabel, int regBytes) {
  DAST* funcID    = dast->children[0];
  DAST* params    = dast->children[1];
  char* funcLabel = funcID->data.identifier;
  int decr        = 0;
  /* move stack for all children, if they exist */
  if (params->size > 0) {

    /* First allocate memory to compute locations. */
    size_t* offsets = malloc (sizeof(size_t) * params->size);
    if (offsets == NULL) {
      AllocationFailed ();
    }
    size_t* sizes = malloc (sizeof(size_t) * params->size);
    if (sizes == NULL) {
      AllocationFailed ();
    }

    decr = ComputeArgLocations (funcID->varDecl, offsets, sizes);
    /* Decrement the stack. */
    EmitADDI(SP, SP, -1 * decr);
    for (int i = 0; i < params->size; i++) {
      /* STUDENT CODE HERE */
      Dispatch(params->children[i], startLabel, endLabel, regBytes);
      if (sizes[i] == 4) { /* use load/store word */
        EmitSW(S1, offsets[i], SP);
      } else if (sizes[i] == 1) { /* use load/store byte */
        EmitSB(S1, offsets[i], SP);
      }
    }
    /* Free the memory. */
    free (offsets);
    free (sizes);
  }

  /* STUDENT CODE HERE */
  EmitJAL(RA, funcLabel);
  EmitADD(S1, X0, A0);
  /* Restore the stack */
  if (params->size > 0) {
    EmitADDI(SP, SP, decr);
  }
  ApplyTypeRules(dast, S1);
}

/*
 *  Helper function for decrementing the stack and storing any registers
 *  that need to be saved. NOTE that you DO NOT know what code will be
 *  Generated after the function declaration and in fact we have hid some
 *  of the code from you.
 *
 *  As a result, you should save every register that calling convention
 *  dictates may need to be restored (look at lecture slides if you
 *  are not sure which registers may need to be restored).
 *
 *  The starter code has already saved the Frame Pointer (S0/FP), so you
 *  do NOT need to save that register.
 *
 *  This function returns the number of bytes the stack was decremented
 *  by. It should always return a non-negative value.
 */
size_t SaveRegisters() {
  /* STUDENT CODE HERE */
  /* Replace me with number of bytes the SP moves. */
  EmitADDI(SP, SP, -48);
  EmitSW(S1, 0, SP);
  EmitSW(S2, 4, SP);
  EmitSW(S3, 8, SP);
  EmitSW(S4, 12, SP);
  EmitSW(S5, 16, SP);
  EmitSW(S6, 20, SP);
  EmitSW(S7, 24, SP);
  EmitSW(S8, 28, SP);
  EmitSW(S9, 32, SP);
  EmitSW(S10, 36, SP);
  EmitSW(S11, 40, SP);
  EmitSW(RA, 44, SP);
  return 48;
}

/*
 *  Helper function for restoring all saved registers. You may assume that the
 *  stack pointer is currently located at the value it was set to when modified
 *  in SaveRegisters ().
 */
void RestoreRegisters() {
  /* STUDENT CODE HERE */
  EmitLW(RA, 44, SP);
  EmitLW(S11, 40, SP);
  EmitLW(S10, 36, SP);
  EmitLW(S9, 32, SP);
  EmitLW(S8, 28, SP);
  EmitLW(S7, 24, SP);
  EmitLW(S6, 20, SP);
  EmitLW(S5, 16, SP);
  EmitLW(S4, 12, SP);
  EmitLW(S3, 8, SP);
  EmitLW(S2, 4, SP);
  EmitLW(S1, 0, SP);
  EmitADDI(SP, SP, 48);
}

/*
 *  Function used for generating code for all if/else expressions. This code
 *  produces labels that should be used to navigate which code block to execute
 *  as a result of the condition provided. None of these labels should change
 *  the start or end label because those values are used for the starts and
 *  ends of loops.
 */
void ProcessIfElse(DAST* dast, char* startLabel, char* endLabel, int regBytes) {
  char* condEnd   = GenerateLocalLabel();
  char* elseStart = GenerateLocalLabel();
  DAST* condition = dast->children[0];
  DAST* ifBody    = dast->children[1];
  DAST* elseBody  = NULL;
  if (dast->size == 3) {
    elseBody = dast->children[2];
  }
  /* STUDENT CODE HERE. */
  if (elseBody == NULL) { /* no else body */
    Dispatch(condition, startLabel, endLabel, regBytes);
    EmitBEQ(S1, X0, condEnd);
    Dispatch(ifBody, startLabel, endLabel, regBytes);
    EmitLABEL(condEnd);
  } else {
    Dispatch(condition, startLabel, endLabel, regBytes);
    EmitBEQ(S1, X0, elseStart);
    Dispatch(ifBody, startLabel, endLabel, regBytes);
    EmitJ(condEnd);
    EmitLABEL(elseStart);
    Dispatch(elseBody, startLabel, endLabel, regBytes);
    EmitLABEL(condEnd);
  }
  /* Free the labels used. */
  free (condEnd);
  free (elseStart);
  ApplyTypeRules(dast, S1);
}

/*
 *  Function used for generating code for all while expressions. This code
 *  produces labels that should be used to navigate which code block to execute
 *  as a result of the condition provided return to the condition when the end
 *  of the loop is reached. In any recursive calls you should replace startLabel
 *  with the label used at the start of the loop and endWhile with the label
 *  used at the end of the loop.
 */
void ProcessWhile(DAST* dast, char* startLabel, char* endLabel, int regBytes) {
  char* startWhile = GenerateLocalLabel();
  char* endWhile   = GenerateLocalLabel();
  DAST* condition  = dast->children[0];
  DAST* whileBody  = dast->children[1];

  /* STUDENT CODE HERE. */
  EmitLABEL(startWhile);
  Dispatch(condition, startLabel, endLabel, regBytes);
  EmitBEQ(S1, X0, endWhile);
  Dispatch(whileBody, startWhile, endWhile, regBytes);
  EmitJ(startWhile);
  EmitLABEL(endWhile);
  /* Free the labels used. */
  free (startWhile);
  free (endWhile);
  ApplyTypeRules(dast, S1);
}
