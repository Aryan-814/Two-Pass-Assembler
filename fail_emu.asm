; Author : Aryan, Roll No : 2401CS48
; memory access fault

        ; Load -5 into A
        ldc -5
        
        ; Transfer A to the Stack Pointer (SP is now -5)
        a2sp        

        ; Load a dummy value into A
        ldc 42      
        
        ; Error : Attempt to store the number 42 at memory address -5.
        stl 0       
        
        ; The emulator should catch the fault before this
        HALT