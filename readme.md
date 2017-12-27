# X Language Interpreter

X interpreter version 0.2 beta -- Dec 2017
Usage: xc.exe [file]     run input file

Available Commands:
INT [var_name],...                              - Define an integer variable
REAL [var_name],...                             - Define a real variable
ASSIGN [var], [value|var]                       - Assign a value to variable
READ [var],...                                  - Read a value from stdin and put it in variable
PRINT [var|literal],...                         - Print a variable or an string literal to stdout
DUMP [var],...                          - Dump a variable info
ADD [var], [var|literal], [var|literal]         - Adds second and third argument and put it on first argument
SUB [var], [var|literal], [var|literal]         - Subtracts second and third argument and put it on first argument
MULT [var], [var|literal], [var|literal]        - Multiplies second and third argument and put it on first argument
DIVI [var], [var|literal], [var|literal]        - Divides(integer) second & third argument and put it on first argument
DIVR [var], [var|literal], [var|literal]        - Divides(real) second and third argument and put it on first argument
MOD [var], [var|literal], [var|literal]         - MOD second and third argument and put it on first argument
LABEL [label_name]                              - Set a label at current line
IFGO [var], [var], [label]                      - Goto label if var1 != var2
EXIT                                            - Exit interpreter
