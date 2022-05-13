# DSA-a.k.a-Data-Structures-and-Algorithsm
### 3 Assignments in DSA course
# Assignment 1
## 1.1 Assignment’s outcome:
  After completing this assignment, students review and make good use of:<br>
• Designing and using recursion<br>
• Object Oriented Programming (OOP)<br>
• List data structures<br>
## 1.2 Introduction
  - Symbol table is a crucial data structure, made and maintained by compilers to trace semantics
of identifiers (e.g information about name, type, scope, e.t.c). <br>
  - Among the stages that the compiler takes to convert source code into executable code,
semantic analysis is one of the most critical stage to validate correctness of the source code,
such as checking whether a variable has been declared prior to being used, or whether the
assignment of a value to a variable is type matched, e.t.c. The semantic analysis stage demands
the symbol table in order to trace the information required in those checks. <br>
  - In this assignment, students are required to implement a simulation of a record table, using
list data structures.
## 1.3 Requirements 
  1. Read carefully this specification <br>
  2. Download initial.zip file and extract it. After that, students will receive files: main.h,
main.cpp, SymbolTable.h, SymbolTable.cpp, error.h. Students are not allowed to modify
the files that are not in the submission list.<br>
  3. Modify the files SymbolTable.h, SymbolTable.cpp to accomplish this assignment, but
make sure to achieve these two requirements:<br>
    • There is at least one SymbolTable class having instance method public void run(string
  testcase) because this method is the input to the solution. For each testcase, an
  instance of this class is created and the run method is called with the file name of
  the text file (containing an interaction with the symbol table) as a parameter.<br>
    • There is only one include command in the SymbolTable.h file, which is include
  ”main.h”, and one include command in the SymbolTable.cpp file, which is include
  ”SymbolTable.h”. Also, no other includes are allowed in these files.<br>
4. Students are required to design and use their data stuctures based on the acknowledged
list data structures.<br>
5. Students must release all dynamically allocated memory when the program ends.<br>
## Result:
### In this assignment I got 93/100 testcase passed
![](https://www.linkpicture.com/q/Result_1.png)
