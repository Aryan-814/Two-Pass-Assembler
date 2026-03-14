; Author : Aryan, Roll No : 2401CS48
; Tests Pass 2 undefined labels and unused label warnings
start:
    ldc 5
    br label  ; Error: no such label 

label1:       ; Warning: defined but never used
    add
    HALT