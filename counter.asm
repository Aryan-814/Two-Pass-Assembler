; ARRAY LENGTH COUNTER
; Traverses a null-terminated array to find '0'
; its total length.

        ; Initialize count = 0
        ldc 0
        ldc count
        stnl 0          

loop:
        ; Calculate address: arr + count
        ldc arr
        ldc count
        ldnl 0
        add             

        ; Dereference pointer: Load mem[arr + count] into A
        ldnl 0          

        ; If the loaded value is 0, we hit the end of the string!
        brz done

        ; Otherwise, count++
        ldc count
        ldnl 0
        ldc 1
        add
        ldc count
        stnl 0

        br loop

done:
        HALT

count:  data 0

; Simulated string "Hello" (ASCII values) followed by a 0
arr:    data 72       
        data 101      
        data 108      
        data 108      
        data 111      
        data 0