; RECURSIVE FIBONACCI SEQUENCE

        ldc 0x1000  ; Initialize Stack Pointer at 4096
        a2sp
        adj -1
        ldc result
        stl 0       ; Store result array pointer on stack
        ldc count
        ldnl 0      ; Load 10 into A
        call main
        adj 1
        HALT

main:   
        adj -3
        stl 2       ; Save return address
        stl 1       ; Save loop count (10)
        ldc 0
        stl 0       ; Save index i = 0

loop:   
        ; Check if i == count
        ldl 1       ; A = count
        ldl 0       ; B = count, A = i
        sub         ; A = B - A (count - i)
        brz endmain ; If count - i == 0, we are done!

        ; Calculate fib(i)
        ldl 0       ; A = i
        call fib    ; A = fib(i)

        ; Store fib(i) into the result array
        adj -1      ; Allocate temp space
        stl 0       ; mem[SP] = fib(i)
        ldl 0       ; A = fib(i)
        ldl 4       ; B = fib(i), A = result pointer
        stnl 0      ; mem[pointer] = fib(i)

        ; Increment result pointer
        ldl 4       ; A = pointer
        adc 1       ; A = pointer + 1
        stl 4       ; Update pointer on stack
        adj 1       ; Clean up temp space

        ; Increment index (i++)
        ldl 0       ; A = i
        adc 1       ; A = i + 1
        stl 0       ; Save i
        br loop

endmain:
        ldl 2       ; Retrieve return address
        adj 3       ; Restore stack pointer
        return

fib:    
        adj -3      ; Create stack frame
        stl 2       ; Save return address
        stl 1       ; Save argument (n)

        ; BASE CASE: if n <= 1, return n
        ldl 1       ; A = n
        ldc 2       ; B = n, A = 2
        sub         ; A = n - 2
        brlz base   ; If (n - 2) < 0, branch to base case

        ; RECURSIVE CASE 1: fib(n-1)
        ldl 1       ; A = n
        ldc 1       ; B = n, A = 1
        sub         ; A = n - 1
        call fib
        stl 0       ; Save result of fib(n-1) to stack

        ; RECURSIVE CASE 2: fib(n-2)
        ldl 1       ; A = n
        ldc 2       ; B = n, A = 2
        sub         ; A = n - 2
        call fib    ; Result of fib(n-2) is now in A

        ; ADD THEM TOGETHER
        ldl 0       ; B = fib(n-2), A = fib(n-1)
        add         ; A = fib(n-2) + fib(n-1)

        ; Return the sum
        ldl 2       ; B = sum, A = return address
        adj 3       ; Destroy stack frame
        return

base:   
        ; Return n
        ldl 1       ; A = n
        ldl 2       ; B = n, A = return address
        adj 3       ; Destroy stack frame
        return

count:  data 10
result: data 0