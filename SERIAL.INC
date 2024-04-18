initializeserial macro
    ;initialze port
    mov dx,3FbH         
    mov al,10000000b
    out dx,al

    mov dx,3f8h
    mov al,0ch
    out dx,al

    mov dx,3f9h
    mov al,00h
    out dx,al

    ;configuration
    mov dx,3fbh
    mov al,00011011b
    out dx,al
endm initializeserial

                SendByte PROC  ; data transferred is in BL (8 bits)
            ;Check that Transmitter Holding Register is Empty
            mov dx , 3FDH		        ; Line Status Register
            AGAIN:
                In al, dx 			    ; Read Line Status
                test al, 00100000b
            JZ AGAIN                    ; Not empty

            ;If empty put the VALUE in Transmit data register
            mov dx, 3F8H		        ; Transmit data register
            mov al, BL
            out dx, al

            ret
        SendByte ENDP
        RecieveByte PROC ; data is saved in BL

            ;Check that Data is Ready
            mov dx , 3FDH		; Line Status Register
            CHK_RecieveByte:
                in al , dx 
                test al , 1
            JZ CHK_RecieveByte              ; Not Ready

            ; If Ready read the VALUE in Receive data register
            mov dx , 03F8H
            in al , dx 
            mov bl , al

            Return_RecieveByte:
                ret
        RecieveByte ENDP
