        ldc 0
        ldc i
        stnl 0

outerloop:
        ldc n
        ldnl 0       
        ldc 1
        sub          
        ldc i
        ldnl 0       
        sub          
        brz done      

        ldc 0
        ldc j
        stnl 0       

innerloop:
        ldc n
        ldnl 0       
        ldc i
        ldnl 0       
        sub          
        ldc 1
        sub          
        ldc j
        ldnl 0       
        sub          
        brz outernext

        ; Calculate pointer for arr[j]
        ldc arr
        ldc j
        ldnl 0       
        add          
        ldc ptr1
        stnl 0       

        ; Calculate pointer for arr[j+1]
        ldc ptr1
        ldnl 0       
        ldc 1
        add          
        ldc ptr2
        stnl 0       

        ; Load val1 = arr[j]
        ldc ptr1
        ldnl 0       
        ldnl 0       
        ldc val1
        stnl 0       

        ; Load val2 = arr[j+1]
        ldc ptr2
        ldnl 0       
        ldnl 0       
        ldc val2
        stnl 0       

        ; Compare val1 and val2
        ldc val1
        ldnl 0       
        ldc val2
        ldnl 0       
        sub          

        ; If val1 <= val2, skip the swap
        brlz innernext 
        brz innernext  

        ; Execute Swap: arr[j] = val2
        ldc val2
        ldnl 0       
        ldc ptr1
        ldnl 0       
        stnl 0       

        ; Execute Swap: arr[j+1] = val1
        ldc val1
        ldnl 0       
        ldc ptr2
        ldnl 0       
        stnl 0       

innernext:
        ldc j
        ldnl 0       
        ldc 1
        add          
        ldc j
        stnl 0       
        br innerloop

outernext:
        ldc i
        ldnl 0       
        ldc 1
        add          
        ldc i
        stnl 0       
        br outerloop

done:
        HALT

i:      data 0
j:      data 0
n:      data 5
ptr1:   data 0
ptr2:   data 0
val1:   data 0
val2:   data 0

; The Array to be Sorted
arr:    data 5
        data 1
        data 4
        data 2
        data 8