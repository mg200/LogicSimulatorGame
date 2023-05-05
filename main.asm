.286
.Model small
.Stack 256
.Data
include vars.txt
ANDGATENAME DB "AND$"
C1 DB 0
.Code
INCLUDE PROC1.INC
MAIN PROC FAR
    MOV AX , @DATA
    MOV DS , AX
PUSHA
graphicalmode
CALL NAMESCREEN
; CALL MAINSCREEN
CALL GAMESCREEN
; drawGate ANDGATEFILENAME,400,257
; drawhorizon 20,60,500,60
XOR AX,AX
XOR BX,BX
XOR CX,CX
XOR DX,DX
POPA
CALL RECEIVECOMMAND
LOWER_TO_UPPER RECEIVED_COMMAND
LEA SI,Received_Command
LEA DI,Store_Command
MOV COPYSTRING_SIZE,7
; CALL COPYSTRING
CALL DRAWANDGATE
; CALL ParseCommand
; LEA DX,Received_Command
; CALL WSTRING 
; drawGate GATEfilename,40,100

;At this point the user has entered the mode he wishes, which is marked by a variable MODE 
; we shall now transfer control to a part were we parse the command
; CALL ParseCommand

; CMP CurMode,02H
; JNE NOT_GAMEMODE1; NOT_GAMEMODE1 IS AN INTERMEDIATE LABEL ONLY 
; drawGate GATEfilename,30,90

; drawGate GATEfilename VAR1,VAR7

NOT_GAMEMODE:

RET
HLT
MAIN ENDP


ParseCommand PROC NEAR 
PUSHA
MOV AL,Store_Command
CMP AL,'A'
JE ANDGATE
CMP AL,'a'
JE ANDGATE
CMP AL,'O'
JE ORGATE
CMP AL,'o'
JE ORGATE
CMP AL,'D'
JE NANDGATE
CMP AL,'d'
JE NANDGATE
CMP AL,'R'
JE NORGATE
CMP AL,'r'
JE NORGATE
CMP AL,'X'
JE XORGATE
CMP AL,'x'
JE XORGATE
CMP AL,'N'
JE XNORGATE
CMP AL,'n'
JE XNORGATE


ANDGATE:
; RET
CALL DRAWANDGATE
ORGATE:

NANDGATE:
NORGATE:
XORGATE:
XNORGATE:
; GATE:
POPA
RET
ParseCommand ENDP

GetLookUpValues PROC NEAR 
PUSH AX
MOV AL,[Store_Command+1]
SUB AL,30H
MOV ip1_col,AL
MOV AL,[Store_Command+2]
SUB AL,30H
MOV ip1_row,30H
MOV AL,[Store_Command+3]
SUB AL,30H
MOV ip2_col,AL
MOV AL,[Store_Command+4]
SUB AL,30H
MOV ip2_row,AL
MOV AL,[Store_Command+5]
SUB AL,30H
MOV op_col,AL
MOV AL,[Store_Command+6]
SUB AL,30H
MOV op_row,AL
POP AX
RET
GetLookUpValues ENDP

DRAWANDGATE PROC NEAR 
PUSHA
CALL GetLookUpValues
CALL GetLogicValues
MOV VAR1,30H
XOR AH,AH
MOV AL,ip1_col
MOV BX,50D
MUL BX
MOV TEMPVW1,AX
XOR AH,AH
MOV AL,ip1_row
MOV BX,50D
MUL BX
MOV TEMPVW2,AX
; MOV TEMPVW1,500
PUSH AX
textmode
POP AX
CALL WRITEINT
lea dx,Line
call wstring 
LEA DX,command_available
CALL WSTRING
RET
drawGate andgatefilename, 500,200
; LEA DX,MES4
; CALL WSTRING
POPA
RET
DRAWANDGATE ENDP

GetLogicValues PROC NEAR 
PUSH AX
PUSH BX 
MOV AL,ip1_col
MOV BL,10D
MUL BL 
ADD AL,ip1_row
XOR BX,BX
MOV BL,AL
MOV AL,[LogicValue+BX]
MOV INPUT1VALUE,AL
MOV AL,ip2_col
MOV BL,10D
MUL BL
ADD AL,ip2_row
XOR BX,BX
MOV BL,AL
MOV AL,[LogicValue+BX]
MOV INPUT2VALUE,AL
POP BX
POP AX
RET
GetLogicValues ENDP

RECEIVECOMMAND PROC NEAR 
; note this must be edited to allow the user to press F1 and F2 and perform the respective action
INPUTLOOP: 
getkeypress ; makes AH=scancode, AL=ASCII code 
CMP AH,Escscancode
JNE NO_ESC_PRESSED
exit; Returns control to the operating system
NO_ESC_PRESSED:
MOV C1,AL 
CALL ISVALIDCOMMAND 
CMP VALID_COMMAND,1H
JNE INPUTLOOP
; reset VALID_COMMAND for next time 
MOV VALID_COMMAND,0H
MOV DL,C1 
MOV Received_Command,DL
INC BX 
DIGITSLOOP:
getkeypress
CMP AH,Escscancode
JNE NO_ESC_PRESSED2
exit; Returns control to the operating system
NO_ESC_PRESSED2:
MOV C1,AL 
CALL ISVALIDDIGIT 
CMP VALID_DIGIT,1H
JNE DIGITSLOOP
MOV VALID_DIGIT,0H
MOV DL,C1 
MOV [Received_Command+BX],DL
INC BX 
CMP BX,07H
JNZ DIGITSLOOP
; at this point Received_Command has the user command stored in it, but don't forget to reset 
RET
RECEIVECOMMAND ENDP 
ISVALIDCOMMAND PROC NEAR 
        CMP AL,[command_available]
        JE RECEIVED_FIRST
        CMP [ command_available+1],AL  
        JE RECEIVED_FIRST
        CMP [ command_available+2],AL 
        JE RECEIVED_FIRST  
        CMP [ command_available+3],AL 
        JE RECEIVED_FIRST
        CMP [ command_available+4],AL 
        JE RECEIVED_FIRST
        CMP [ command_available+5],AL  
        JE RECEIVED_FIRST
        CMP [ command_available+6],AL
        JE RECEIVED_FIRST
        CMP [ command_available+7],AL
        JE RECEIVED_FIRST
        CMP [ command_available+8],AL
        JE RECEIVED_FIRST
        CMP [ command_available+9],AL
        JE RECEIVED_FIRST
        CMP [ command_available+10],AL
        JE RECEIVED_FIRST
        CMP [ command_available+11],AL
        JE RECEIVED_FIRST
        CMP [ command_available+12],AL
        JE RECEIVED_FIRST
        CMP [ command_available+13],AL
        JE RECEIVED_FIRST
        CMP [ command_available+14],AL
        JE RECEIVED_FIRST
        CMP [ command_available+15],AL 
        JE RECEIVED_FIRST         
        CMP [ command_available+16],AL
        JE RECEIVED_FIRST
        CMP [ command_available+17],AL
        JE RECEIVED_FIRST
        ; IF NOT EQUAL ANY SO IT'S NOT A VALID CHARACTER
        MOV VALID_COMMAND,0H
        JMP ISVALIDCOMMANDEND
        RECEIVED_FIRST:
        MOV VALID_COMMAND,1H
        ISVALIDCOMMANDEND:
        RET
    ISVALIDCOMMAND ENDP

ISVALIDDIGIT PROC NEAR 
        CMP AL,30H
        JL NOTA_VALID_DIGIT
        CMP AL,39H 
        JG NOTA_VALID_DIGIT
        JMP VALIDDIGIT_LABEL
        NOTA_VALID_DIGIT:
        MOV VALID_DIGIT,0H
        JMP ISVALIDDIGIT_END
        VALIDDIGIT_LABEL:
        MOV VALID_DIGIT,1H 
        ISVALIDDIGIT_END:
        RET 
ISVALIDDIGIT ENDP 
    
        

NAMESCREEN PROC near
    movecursor 01h, 01h
    print enternamemes
    movecursor 02h, 05h
    MOV bx, offset p1name
    READNAME
    MOV bh, 0h ; necessary since bx changes in readname function ; default is bh = 0 for moving cursor

    movecursor 10h, 01h
    print presstocontinuemes

    waitforuseractionNS:
    MOV ah, 0
    int 16h

    ; CHECK IF KEY IS ESC
    cmp ah, Escscancode
    je exitnamescreen

    ; CHECK IF KEY IS ENTER
    cmp ah, Enterscancode
    jne waitforuseractionNS
    clearscreen
    RET
    exitnamescreen:
    clearscreen
    exit
NAMESCREEN ENDP

NAMESCREEN2 PROC NEAR 
movecursor 01h, 01h
LEA DX, enternamemes
CALL WSTRING 
PUSHA 
MOV CX,16 ;max length of name-1 
LEA DX, p1name 
MOV CX,16 
POPA 
RET 
NAMESCREEN2 ENDP 

MAINSCREEN PROC NEAR
    
    displaymainscreen:
    movecursor 09h, 10h
    print mes1      
    
    movecursor 0bh,10h
    print mes2
    
    movecursor 0dh,10h
    print mes3

    movecursor 15h,00h
    print mes5
    
    waitforuseractionMS:
    getkeypress
                
    ; check if key is ESC                    
    cmp ah, Escscancode        
    jne checkf1
    clearscreen
    exit
    RET            
    
    ; check if key is F1                    
    checkf1:
    cmp ah, F1scancode
    jne checkf2
    clearscreen
    call chatSCREEN
    ; jmp displaymainscreen
    JMP MAINSCREENEND

    ; check if key is F2
    checkf2:
    cmp ah, F2scancode
    jne waitforuseractionMS
    clearscreen
    PUSHA
    Call GAMESCREEN; gamescreen is drawn here
    MOV CurMode,02H
    POPA
    ; jmp displaymainscreen
    MAINSCREENEND:
    RET
MAINSCREEN ENDP


GAMESCREEN PROC NEAR 
; graphicalmode ; I COMMENTED THIS OUT ON 24/4/2023 3:11 PM
   ;change video mode to 640x480
    MOV AH, 00h
    MOV AL, 12h ;12
    INT 10h
	
 ;RELATED TO SCREEN GAME   
        ;CALL changeBackgroundColorbyText
        CALL changeBackgroundColor 
        CALL WriteGuidline

        ;set row 
        MOV dx, 20
        CALL DrawHorizontal
        ;set row 
        MOV dx, 450
        CALL DrawHorizontal
        
        MOV cx,75d ;thats the first coloumn
        CALL DrawVertical

        MOV cx,565d ; thats the last coloumn
        CALL DrawVertical
        drawLoop3:
            CALL DrawVertical
            sub cx,70d         
            cmp cx, 75d ;first coloum
        jne drawLoop3
    

    ;drawing sources & Destination nodes
    MOV dx,30
    drawLoop8:
            CALL DrawSourceAndDestination
            add dx,40       
            cmp dx, 430 ;last coloum
        jne drawLoop8

    MOV dx,30
    drawLoop10:
            CALL DrawSourceAndDestination1
            add dx,40       
            cmp dx, 430 ;last coloum
    jne drawLoop10

    ;drawing numbers below the nodes
    CALL DrawNumberBelow
    ;drawing numbers at source and destination node
   ; MOV dl,3h
  ;  CALL DrawverticalNumbers
   ; MOV dl,04ch ;choose which coloumn it will set to it
   ; CALL DrawverticalNumbers

    ;testing
    ;MOV cx,250d ;coloumn move right or left
    ;MOV dx,250d
    ;MOV AL,0
    ;MOV ah,0ch 
    ;back: int 10h
    ;   inc cx
    ;   cmp cx,640
    ;  jnz back

        ; ;testing
        ;   MOV RowNum,40d
        ;     MOV ColNum,350 ; coloumn
        ;     MOV dx,RowNum
        ;     ;drawing pixel
        ;     MOV CX,ColNum ; coloumn
        ;     MOV si,ColNum
        ;     add si,15d
        ;     MOV di,0
        ; loop213:
        ;         MOV AL,00H ; color
        ;         MOV AH,0ch  ; draw pixel        
        ;     drawLoop23:
        ;         INT 10h   
        ;         INC CX
        ;         CMP CX,si
        ;     JNE drawLoop23 
        ;     MOV cx,ColNum
        ;    inc dx
        ;    inc di
        ;    cmp di,3d
        ; jne loop213

        
                            ;a5rna mmkn ytrsm 240 mn3rf4 lieh
    ;  MOV ColNum ,241d ; move right or left note its 67 not 75 because i want to center the nodes so shifted 8px to the left
    ; MOV RowNum,40d ; move up or down
    ; MOV cx,ColNum
    ; MOV dx,RowNum
        ;CALL draw_labels
    ; CALL DrawNode
        pusha
        ;FOR ODD COLOUMN
        ;you should specify the RowNum and ColNum  before calling DrawNode
        MOV ColNum ,67d ; move right or left note its 67 not 75 because i want to center the nodes so shifted 8px to the left
        MOV RowNum,40d ; move up or down
        ;thats has an issue will be just fixed with imperative way    
        BIGLOOP1:
            MOV di,0d ; counter

            drawLoop6:
                CALL DrawNode
                add RowNum,40d
                inc di
                cmp di, 9d ;last coloumn
            jne drawLoop6

            MOV RowNum,40d ; reset to start drawing nodes from the top
            add ColNum,140d ; shifting every loop to the next odd coloumn          
            CMP ColNum,627d ;627
        JNE BIGLOOP1
        popa
        pusha
        ;FOR EVEN COLOUMN
    ;you should specify the RowNum and ColNum  before calling DrawNode
        MOV ColNum ,137d ; move right or left note its 67 not 75 because i want to center the nodes so shifted 8px to the left
        MOV RowNum,60d ; move up or down
        ;thats has an issue will be just fixed with imperative way    
        BIGLOOP2:
            MOV di,9d
            drawLoop7:
                CALL DrawNode
                add RowNum,40d
                dec di
                cmp di, 0d ;last coloumn
            jne drawLoop7

            MOV RowNum,60d ; reset to start drawing nodes from the top
            add ColNum,70*2d ; shifting every loop to the next odd coloumn
                
            CMP ColNum,697d
        JNE BIGLOOP2
        popa


        CALL DrawAllNumber

RET
GAMESCREEN ENDP


;--------chatting procedures--------------

Send Proc near 
	MOV Xsend,AL;if esc was clicked so exit
	cmp AL,27
    jnz continue
	MOV dx ,3FDH		; Line Status Register, to send data check if THR empty or not
	AGAIN11:
  	In AL,dx 			;Read Line Status
	AND AL , 00100000b
	jz AGAIN11

	MOV dx , 3F8H		; (if empty)Transmit data register
    MOV  AL,Xsend
  	out dx , AL 
	MOV Exit_Chat, 1
	RET
	
	CMP AX, 0E08H
	JNZ continue
	MOV Xsend, 08H
	JMP sDisplay
	
	
    continue:	
	MOV ah,79
	cmp byte ptr firs_half,ah
	jb snot_end_x

	MOV ah,09h
	cmp byte ptr firs_half[1],ah
	jb sDisplay

	MOV word ptr firs_half,0900h
	MOV ah,2
	MOV dx,word ptr firs_half   ;setting cursor
	int 10h 
	MOV bl,0
	MOV AX,0601h
	MOV bh,00Fh       ;scrolling one line
	MOV cx,0300h
	MOV dx,094fh
	int 10h
	jmp sDisplay

    snot_end_x:
	MOV ah,09h
	cmp byte ptr firs_half[1],ah 
	jb scheck_enter
	cmp AL,0Dh
	jne sDisplay
	MOV word ptr firs_half,0D00h
	MOV bl,0
	MOV AX,0601h
	MOV bh,00Fh
	MOV cx,0300h
	MOV dx,094fh
    ;add dl,8
	int 10h
	jmp sDisplay

    scheck_enter:
	cmp AL,0dh
	jne sDisplay
	MOV byte ptr firs_half,00h	
	inc byte ptr firs_half[1]
    	
	jmp sDisplay
	
    sDisplay:
	MOV ah,2
	MOV dx,word ptr firs_half
	int 10h 
	CMP Xsend, 08H
	JNZ PRINT_CHAR_MES
	MOV DL,08H                          ; IN ORDER TO REMOVE THE CHARACTER 
    MOV AH,2                            ; BACKSPACE WAS FIRST PRINTED 
    INT 21H
    MOV DL,' '                          ; THEN SPACE WAS PRINTED 
    MOV AH,2
    INT 21H
	MOV DL,08H                          ; IN ORDER TO REMOVE THE CHARACTER 
    MOV AH,2                            ; BACKSPACE WAS FIRST PRINTED 
    INT 21H
	JMP NEXT_STEP
    PRINT_CHAR_MES:
	MOV dl , Xsend;print char
	MOV ah ,2 
  	int 21h
    NEXT_STEP:
	MOV ah,3h 
	MOV bh,0h    ;getting cursor position
	int 10h
	MOV word ptr firs_half,dx

	MOV dx ,3FDH		; Line Status Register, to send data check if THR empty or not
	AGAIN2:
  	In AL,dx 			;Read Line Status
	AND AL , 00100000b
	jz AGAIN2

	MOV dx , 3F8H		; (if empty)Transmit data register
    MOV  AL,Xsend
  	out dx , AL 
	ret
Send endp


Receive proc near 
        MOV dx , 03F8H
        IN AL , dx 
        MOV Yrecieve , AL
        cmp AL,27               ; esc was clicked 
        jz buffer
        
        MOV ah,79
        cmp byte ptr sec_half,ah
        jb rnot_end_x

        MOV ah,15h
        cmp byte ptr sec_half[1],ah
        jb rDisplay

        MOV word ptr sec_half,1500h
        MOV ah,2
        MOV dx,word ptr sec_half
        int 10h 

        MOV bl,0
        MOV AX,0601h
        MOV bh,00Fh
        MOV cx,0C00h      ;scrolling one line
        MOV dx,154fh
        int 10h
        jmp rDisplay
    buffer:
        MOV Exit_Chat, 1
        RET
    rnot_end_x:

        MOV ah,15h
        cmp byte ptr sec_half[1],ah
        jb rcheck_enter
        cmp AL,0Dh
        jne rDisplay
        MOV word ptr sec_half,1800h
        MOV bl,0
        MOV AX,0601h
        MOV bh,00Fh
        MOV cx,0C00h
        MOV dx,154fh
        int 10h
        jmp rDisplay

    rcheck_enter:
        cmp AL,0Dh
        jne rDisplay
        MOV byte ptr sec_half,00h	
        inc byte ptr sec_half[1]	
        jmp rDisplay
        
    rDisplay:
        MOV ah,2
        MOV dx,word ptr sec_half
        int 10h 
        CMP Yrecieve, 08H
        JNZ PRINT_CHAR_2
        MOV DL,08H                          ; IN ORDER TO REMOVE THE CHARACTER 
        MOV AH,2                            ; BACKSPACE WAS FIRST PRINTED 
        INT 21H
        MOV DL,' '                          ; THEN SPACE WAS PRINTED 
        MOV AH,2
        INT 21H
        MOV DL,08H                          ; IN ORDER TO REMOVE THE CHARACTER 
        MOV AH,2                            ; BACKSPACE WAS FIRST PRINTED 
        INT 21H
        JMP NEXT_STEP_2
    PRINT_CHAR_2:
    MOV dl , Yrecieve
        MOV ah ,2 
        int 21h
    NEXT_STEP_2:	
        MOV ah,3h 
        MOV bh,0h 
        int 10h
        MOV word ptr sec_half,dx
        ret
		Receive ENDP


chatSCREEN PROC NEAR
        MOV firs_half, 0300h
        MOV sec_half,  0d00h

        movecursor 18h,02h
        print mes4

        movecursor 00h, 00h
        print border

        movecursor 01h, 00h
        print p1name
        print space
        movecursor 0AH, 00h  
        print line
        movecursor 0bh,00h

        print p2name
        print space

        movecursor 03h, 00h
        
        MOV bl,0
        MOV AX,0600h
        MOV bh,00fh         ; font and screen colour
        MOV cx,0300H        ;first half
        MOV dx,094fh		
        int 10h	

        MOV bl,0
        MOV AX,0600h
        MOV bh,00Fh       ;font and screen colour
        MOV cx,0C00h      ;second half
        MOV dx,154fh
        int 10h
        MOV bh,0  
        ; call configuration;first thing to do
    IsSenT:
        MOV dx , 3FDH    ; check Line Status Register 
        IN AL , dx 
        AND AL , 1
        JZ next          ; There is no recieved data
        call Receive     ;if ready read the value in received data register
        CMP Exit_Chat, 1
        JZ exitchat
        MOV AL,1 
        MOV dx , 3FDH 
        out dx,AL   
    next:
        MOV ah,1
        int 16h			 ;check if character available in buffer
        jz IsSenT        ; no char is written
        getkeypress ;lw buffer not empty asci in AL,scan in ah
        cmp ah, F3scancode
        JE exitchat
        call Send
        CMP Exit_Chat, 1
        JZ exitchat
        jmp IsSenT
  	
    exitchat:
        ; a3tkd hena hykon fe shwyt cleanup
        CLEARSCREEN 	
		ret  	
chatSCREEN ENDP


END MAIN



;----------------------- Old screen game -------------------
;     ;change video mode to 640x480
;     MOV AH, 00h
;     MOV AL, 12h ;12
;     INT 10h
	
;     ;CALL changeBackgroundColorbyText
;     CALL changeBackgroundColor 
;     CALL WriteGuidline

;     ;set row 
;     MOV dx, 20
;     CALL DrawHorizontal
;     ;set row 
;     MOV dx, 450
;     CALL DrawHorizontal
    
;     MOV cx,75d ;thats the first coloumn
;     CALL DrawVertical

;     MOV cx,565d ; thats the last coloumn
;     CALL DrawVertical
;     drawLoop3:
;         CALL DrawVertical
;         sub cx,70d         
;         cmp cx, 75d ;first coloum
;     jne drawLoop3
  

; ;drawing sources & Destination nodes
; MOV dx,30
; drawLoop8:
;         CALL DrawSourceAndDestination
;         add dx,40       
;         cmp dx, 430 ;last coloum
;     jne drawLoop8

; MOV dx,30
; drawLoop10:
;         CALL DrawSourceAndDestination1
;         add dx,40       
;         cmp dx, 430 ;last coloum
; jne drawLoop10

;  ;drawing numbers below the nodes
;  CALL DrawNumberBelow
;    ;drawing numbers at source and destination node
;   MOV dl,3h
;   CALL DrawverticalNumbers
;   MOV dl,04ch ;choose which coloumn it will set to it
;   CALL DrawverticalNumbers
;     pusha
;     ;FOR ODD COLOUMN
;     ;you should specify the RowNum and ColNum  before calling DrawNode
;     MOV ColNum ,67d ; move right or left note its 67 not 75 because i want to center the nodes so shifted 8px to the left
;     MOV RowNum,40d ; move up or down
;     ;thats has an issue will be just fixed with imperative way    
;     BIGLOOP1:
;         MOV di,0d ; counter

;         drawLoop6:
;             CALL DrawNode
;             add RowNum,40d
;             inc di
;             cmp di, 9d ;last coloumn
;         jne drawLoop6

;         MOV RowNum,40d ; reset to start drawing nodes from the top
;         add ColNum,140d ; shifting every loop to the next odd coloumn          
;         CMP ColNum,627d ;627
;     JNE BIGLOOP1
;     popa
;     pusha
;     ;FOR EVEN COLOUMN
;    ;you should specify the RowNum and ColNum  before calling DrawNode
;     MOV ColNum ,137d ; move right or left note its 67 not 75 because i want to center the nodes so shifted 8px to the left
;     MOV RowNum,60d ; move up or down
;     ;thats has an issue will be just fixed with imperative way    
;     BIGLOOP2:
;          MOV di,9d
;         drawLoop7:
;             CALL DrawNode
;             add RowNum,40d
;             dec di
;             cmp di, 0d ;last coloumn
;         jne drawLoop7

;         MOV RowNum,60d ; reset to start drawing nodes from the top
;         add ColNum,70*2d ; shifting every loop to the next odd coloumn
            
;         CMP ColNum,697d
;     JNE BIGLOOP2
;     popa

;     ; ; Press any key to exit
;     ; MOV AH , 00h
;     ; INT 16h

; ; textmode


ISVALIDDIGIT PROC NEAR 

;KENZY CODE FOR VALID DIGIT
    ; CMP  [digit],AL 
    ; JE VALIDDIGIT_LABEL
    ; CMP [ digit+1],AL  
    ; JE VALIDDIGIT_LABEL
    ; CMP [ digit+2],AL 
    ; JE VALIDDIGIT_LABEL  
    ; CMP [ digit+3],AL
    ; JE VALIDDIGIT_LABEL  
    ; CMP [ digit+4],AL 
    ; JE VALIDDIGIT_LABEL 
    ; CMP [ digit+5],AL
    ; JE VALIDDIGIT_LABEL  
    ; CMP [ digit+6],AL
    ; JE VALIDDIGIT_LABEL 
    ; CMP [ digit+7],AL 
    ; JE VALIDDIGIT_LABEL
    ; CMP [ digit+8],AL 
    ; JE VALIDDIGIT_LABEL
    NOTA_VALID_DIGIT:
    MOV VALID_DIGIT,0H
    JMP ISVALIDDIGIT_END
VALIDDIGIT_LABEL:
MOV VALID_DIGIT,1H 
ISVALIDDIGIT_END:
RET 
ISVALIDDIGIT ENDP 
