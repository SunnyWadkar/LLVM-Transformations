# LLVM-Transformations
Loop Invariant Code Motion and Dead Code Elimination

1. DEAD CODE ELIMINATION

    * To build the DCE run the make command in the DCE folder:

    > make

    The files DeadCodeElimination.so will be generated.

    * Build the test files

    **Instructions: The DCE pass requires the variable names. The IR may contain values which are actually registers. These registers do not have values and hence DCE pass may not work for such values. It also requires mem2reg pass. **

    To make it work, instnamer and mem2reg passes needs to be run on the byte code file before running the DCE pass. The command for that
    pass is as follows:

    > opt - instnamer -mem2reg <input_byte_code> -o <output_byte_code>

    Example:

    > opt -instnamer -mem2reg ./tests/dce_test-m2r.bc  -o ./tests/dce_test-m2r.bc 

    To build the test files, run the following commands in the DCE folder

    > clang -Xclang -disable-O0-optnone -O0 -emit-llvm -c ./tests/dce_test.c -o ./tests/dce_test.bc

    > opt -mem2reg -instnamer ./tests/dce_test.bc -o ./tests/dce_test-m2r.bc

    > clang -Xclang -disable-O0-optnone -O0 -emit-llvm -c ./tests/dce_test_2.c -o ./tests/dce_test_2.bc

    > opt -mem2reg -instnamer ./tests/dce_test_2.bc -o ./tests/dce_test_2-m2r.bc

    > clang -Xclang -disable-O0-optnone -O0 -emit-llvm -c ./tests/dce_test_3.c -o ./tests/dce_test_3.bc
    > opt -mem2reg -instnamer ./tests/dce_test_3.bc -o ./tests/dce_test_3-m2r.bc

    * To run the pass, run the following command in the DCE folder:

    > opt -load ./DeadCodeElimination.so -dead-code-elimination ./tests/dce_test-m2r.bc -o /tests/dce_test-m2r_mod.bc

    > opt -load ./DeadCodeElimination.so -dead-code-elimination ./tests/dce_test_2-m2r.bc -o /tests/dce_test_2-m2r_mod.bc

    > opt -load ./DeadCodeElimination.so -dead-code-elimination ./tests/dce_test_3-m2r.bc -o /tests/dce_test_3-m2r_mod.bc

2. LOOP INVARIANT CODE MOTION

    * To build the LICM run the make command in the LICM folder:

    > make

    The files LICM.so will be generated.

    * Build the test files

    **Instructions: The LICM pass requires the variable names. The IR may contain values which are actually registers.These registers do not have values and hence LICM pass may not work for such values. It also requires mem2reg pass.**
    To make it work, instnamer and mem2reg passes needs to be run on the byte code file before running the LICM pass. The command for that
    pass is as follows:

    > opt - instnamer -mem2reg <input_byte_code> -o <output_byte_code>

    Example:

    > opt -instnamer -mem2reg ./tests/licm_test-m2r.bc  -o ./tests/licm_test-m2r.bc 

    To build the test files, run the following commands in the licm_test folder

    > clang -Xclang -disable-O0-optnone -O0 -emit-llvm -c ./tests/licm_test.c -o ./tests/licm_test.bc

    > opt -mem2reg -instnamer ./tests/licm_test.bc -o ./tests/licm_test-m2r.bc

    > clang -Xclang -disable-O0-optnone -O0 -emit-llvm -c ./tests/licm_test_2.c -o ./tests/licm_test_2.bc

    > opt -mem2reg -instnamer ./tests/licm_test_2.bc -o ./tests/licm_test_2-m2r.bc

    > clang -Xclang -disable-O0-optnone -O0 -emit-llvm -c ./tests/licm_test_3.c -o ./tests/licm_test_3.bc

    > opt -mem2reg -instnamer ./tests/licm_test_3.bc -o ./tests/licm_test_3-m2r.bc

    * To run the pass, run the following command in the DCE folder:

    > opt -load ./LICM.so -loop-invariant-code-motion ./tests/licm_test-m2r.bc -o /tests/licm_test-m2r_mod.bc

    > opt -load ./LICM.so -loop-invariant-code-motion ./tests/licm_test_2-m2r.bc -o /tests/licm_test_2-m2r_mod.bc

    > opt -load ./LICM.so -loop-invariant-code-motion ./tests/licm_test_3-m2r.bc -o /tests/licm_test_3-m2r_mod.bc

    * To run thr dominator pass, run the following command

    > opt -load ./dominator_pass.so -dominators ./tests/licm_test-m2r.bc -o /dev/null