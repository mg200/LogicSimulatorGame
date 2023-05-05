          print macro text    ; print bufferdata
    mov ah, 9h
    mov dx, offset text        
    int 21h
endm print

textmode macro
    mov ah, 0
    mov al, 3h
    int 10h
endm textmode


exit macro
    mov ah, 4ch
    int 21h
endm exit

clearscreen macro
    MOV AH,0
    MOV AL,3
    INT 10H
endm clearscreen

getkeypress macro 
    mov ah, 0
    int 16h
endm getkeypress


GETCHAR MACRO STORAGE
    LOCAL NOTRETURN
    PUSHA 
    MOV AH,07H ;was 01H
    INT 21H
    MOV STORAGE,AL
   ;We want to check that the pressed key was not F4, in which case we want to exit the game
    CMP AL,1BH
    JNE NOTRETURN
    MOV ESCWASPRESSED,1H 
    NOTRETURN:
    POPA 
    ENDM GETCHAR

      PRINTSTR MACRO STRING ; stops printint at $
     PUSHA
     MOV SI,OFFSET STRING
     P_LOOP:
     MOV DL,[SI]
     CMP DL,0
     JE STRING_END
     INC SI
     MOV AH,02H
     INT 21H
     JMP P_LOOP
     STRING_END:
     POPA
     ENDM PRINTSTR

		     videomode macro
    mov ax,19
    int 16h
    endm videomode

  graphicalmode macro 
    mov ah,00h
    ; mov ah,2ah
    mov al,13h
    ;4h is 320*200, 3h is 
   ; mov al,07h 
int 10h
endm graphicalmode

   COLOREDCHAR MACRO CHARA
        PUSHA
        MOV AH,09H
        MOV AL,CHARA
        MOV BH,0
        ; MOV BL,4H;COMMENTED ON 12/4 REPLACED WITH 1 BELOW
        MOV BL,THECOLOR
        MOV CX,1 
        INT 10H
        POPA
        ENDM



     PRINTCHAR MACRO TheArg
     PUSHA;;ADDED ON 16/3/2023
  mov dl, TheArg
  mov ah, 02h    ; DOS.PrintCharacter
  int 21h
  POPA;;ADDED ON 16/3/2023
ENDM    

		movecursor macro x,y
PUSHA  ;;ADDED ON 16/3/2023
    MOV AH,2
           MOV Dh,x
           mov dl,y
           INT 10H
           POPA;;ADDED ON 16/3/2023
       endm movecursor

	       PRINTCHAR MACRO TheArg
     PUSHA;;ADDED ON 16/3/2023
  mov dl, TheArg
  mov ah, 02h    ; DOS.PrintCharacter
  int 21h
  POPA;;ADDED ON 16/3/2023
ENDM   
  
  


READNAME macro
                    MOV CX,15                           ; LOOP TILL THE TOTAL VALID CHARACTERS INPUT ARE 15
                    MOV SI,0                            ; INITIALIZE SI WITH 0 TO USE IT AS A POINTER 
        CHECK_FIRST_CHARACTER: MOV AH,0                  ; WAIT FOR KEY TO BE PRESSED 
                    INT 16H 
                    CMP AL,41H                          ; CHECK IF IT WITHIN THE ALPHABET LETTERS  'A' = 41H
                    JB CHECK_FIRST_CHARACTER                             
                    CMP AL,5AH                          ; IF IT WAS LESS THAN 'Z' = 5AH, SO IT IS A VALID FIRST CHARACTER 
                    JB VALID_FIRST_CHAR
                    CMP AL,61H                          ; IF IT IS LESS THAN 'a' = 61H, then it is not a valid first character
                    JB CHECK_FIRST_CHARACTER
                    CMP AL,7AH
                    JA CHECK_FIRST_CHARACTER            ; IF IT IS MORE THAN 'z' = 7AH, then it is not a valid first character
                    JBE VALID_FIRST_CHAR ; MY ADDITION
        TAKE_ANOTHER_INPUT: MOV AH,0
                    INT 16H
                    CMP AH,Enterscancode                        ; CHECK IF THE PRESSED KEY IS ENTER TO END ENTERING NAME 
                    JE END_TYPING 
                    CMP AH, 0Eh                       ; CHECK IF THE PRESSED KEY IS BACKSPACE
                    JE  DELETE_CHAR
        VALID_FIRST_CHAR: MOV [BX+SI],AL                 ; AFTER ALL VALIDATIONS ADD THE LETTER TO THE PLAYER NAME 
                    INC SI
                    INC p1nameactualsize   
                    MOV DL,AL                           ; PRINT THE VALID INPUT CHARACTER 
                    MOV AH,2
                    INT 21H
                    LOOP TAKE_ANOTHER_INPUT
                    JMP END_TYPING
        DELETE_CHAR: 
                    DEC p1nameactualsize
                    DEC SI
                    INC CX
                    MOV DL,08H                          ; IN ORDER TO REMOVE THE CHARACTER 
                    MOV AH,2                            ; BACKSPACE WAS FIRST PRINTED 
                    INT 21H
                    MOV DL,' '                          ; THEN SPACE WAS PRINTED 
                    MOV AH,2
                    INT 21H
                    MOV DL, "$"
                    MOV [BX+SI],DL                      ; OVERRIDE THE DATA SAVED PREVIUOSY 
                    MOV DL,08H                          ; THEN BACKSPACE AGAIN "NOTE: BACKSPACE ONLY BRING THE CURSOR OF TYPING ONE CHAR BACK WITHOUT DELETING" 
                    MOV AH,2
                    INT 21H
                    CMP CX, 15
                    JZ CHECK_FIRST_CHARACTER
                    JMP TAKE_ANOTHER_INPUT
        END_TYPING: 
endm READNAME



OpenFile macro Filehandle, Filename ; Open file
    MOV AH, 3Dh
    MOV AL, 0 ; read only
    LEA DX, Filename
    INT 21h
    
    ; you should check carry flag to make sure it worked correctly
    ; carry = 0 -> successful , file Handle -> AX
    ; carry = 1 -> failed , AX -> error code 
    MOV [Filehandle], AX
endm OpenFile

ReadData macro Filehandle, Width, Height, Data
    MOV AH,3Fh
    MOV BX, [Filehandle]
    MOV CX, Width* Height ; number of bytes to read
    LEA DX, Data
    INT 21h
endm ReadData 

CloseFile macro Filehandle
	MOV AH, 3Eh
	MOV BX, [Filehandle]
	INT 21h
endm CloseFile


draw macro Width, Height, startingcolumn, color
    LOCAL drawLoop,drawnot
    mov AH, 0CH
    XOR BH,BH 
    MOV BL,0;
    drawLoop:
    MOV AL,[SI]
    cmp al,color
    je drawnot
    INT 10h 
    drawnot:
    INC CX
    INC SI
    CMP CX,Width
    JNE drawLoop	
    MOV CX , startingcolumn
    INC DX
    CMP DX , Height
    JNE drawLoop
endm draw

drawGate MACRO GATEfilename,piecerow_par,piececol_par 
OpenFile GateFileHandle,GATEfilename
ReadData GateFileHandle,GateWidth,GateHeight,GateData
LEA SI, GateData
; MOV CH, 0D
;     MOV CL,piecerow_par; X-direction
;     MOV DH, 0D
;     MOV DL,piececol_par; Y-direction
    MOV CX,PIECEROW_PAR 
    MOV DX,piececol_par
    MOV scol, CX
    MOV HorizontalDimension, GATEWIDTH
    MOV VerticalDimension, GATEHEIGHT
    ADD HorizontalDimension, CX
    ADD VerticalDimension, DX
    draw HorizontalDimension, VerticalDimension, scol,00h
    CloseFile GateFileHandle

ENDM drawGate

DRAWSLANTEDLEFT MACRO XN,YN,XF,YF
LOCAL DRAWLOOP
PUSHA 
MOV CX,XN
MOV DX,YN
DRAWLOOP:
MOV AH,0CH
MOV AL,4
INT 10H
INC DX
DEC CX
CMP CX,XF 
JNZ DRAWLOOP
POPA 
ENDM DRAWSLANTEDLEFT

 DrawDiagonal_LEFT MACRO XNODE,YNODE,LENGTH
      LOCAL d1
      PUSHA
      MOV CX,XNODE
      MOV DX,YNODE 
      MOV xf_drawlen,LENGTH
      ADD xf_drawlen,CX
      MOV yf_drawlen,LENGTH
      ADD yf_drawlen,DX
      d1:
       MOV AH, 0CH
       MOV AL,1
       INT 10H
       DEC CX
       INC DX
       CMP CX,0
       CMP DX,yf_drawlen
       JNE d1
       MOV XF_DRAWLEN,0
       MOV YF_DRAWLEN,00h
      POPA
      ENDM DrawDiagonal_LEFT

 Drawsquare macro xnote,ynote,color,sizeofbox
   local first, second, third, fourth
    
    mov ah, 0ch
    mov al,color
    mov cx,xnote
    mov dx,ynote
    int 10h
    
   mov bl,sizeofbox
   first:
   int 10h
   inc cx
   dec bl
   jnz first 
   
     mov bl,sizeofbox
   second:
   int 10h
   inc dx
   dec bl
   jnz second
     
     
     mov bl,sizeofbox
   third:
   int 10h
   dec cx
   dec bl
   jnz third
   
     mov bl,sizeofbox
   fourth:
   int 10h
   dec dx
   dec bl
   jnz fourth
     
   
   endm Drawsquare

drawline macro   xp,yp,xf,yf,color
LOCAL DRAWLOOP
    mov cx,xp
    mov dx,yp
    mov ah,2ch
    drawloop:
    int 10h
    inc cx
    cmp cx, xf
    jnz drawloop
endm drawline

     drawhorizon macro xnode, ynode, xfinal,yfinal                       
       local BACK
               ; TO DRAW A HORIZONTAL LINE
       MOV CX, xnode
       MOV DX, ynode 
       BACK:
       MOV AH, 0CH
       MOV AL,1
       INT 10H
       INC CX
       CMP CX, xfinal
       JNZ BACK  
             endm drawhorizon

;-------------------------------------------BASSEM FUNCTIONS FOR DRAWING SCREEN-----------------------------------------------------
;function that draw horizontal line used in changebackgroundcolor
; DrawScreen PROC
;     pusha
;     ;drawing pixel
;     MOV CX,0 ; coloumn
;     ;MOV DX,0    ; row
;     mov al,0FH ; color
;     MOV AH,0ch  ; draw pixel  
    
; drawLoop:
;     INT 10h   
;     INC CX
;     CMP CX,640
; JNE drawLoop 
; 	popa
; 	RET
; DrawScreen ENDP
;  ;function that change background color but by texting white color on the screen
;  changeBackgroundColorbyText PROC
;     ;mov ax,06ffh
;     ;mov bh,05h
;     ;mov cx,0000h
;     ;mov dx,0ffffh
;    ; int 10
;     ;method 1 writing white characters in the background
;         mov ah,9
;         mov bh,0
;         mov al,0
;         mov cx,0ffffh
;         mov bl,0000
;         int 10h
;  changeBackgroundColorbyText ENDP
;  ;function that change background color to white
; changeBackgroundColor PROC    
;                  pusha 
;              MOV DX,0    ; row
;              loop1:
;                     CALL DrawScreen
;                     inc dx  
;                     cmp dx,480d
;              jne loop1     
;                 popa
;                     RET
; changeBackgroundColor ENDP 
; ;function that write guideline the statement in the middle of the screen
; WriteGuidline PROC    
;             pusha        
;            ;changing curser position 
;     mov ah,2h
;     mov dx,0067h
;     int 10h

;     ;draw statment
;     mov di,offset Guideline
;     mov si,0h
;     loopString:
;         ;draw one character 
;         mov ah,09h  ;setting mode for int 10 to draw character       
;         mov al,[di] ; Put the character in address bp
;         inc di      ;getting the next character from array of characters
;         mov bh, 00h   ;page 0
;         mov bl, 03Eh ;E is yello (background) 0 is black (font)
;         mov cx,1h  
;          int 10h
;         ;moving curser     
         
;         mov ah,2h
;         inc dx
;         int 10h
        
;         inc si
;         cmp si,36
;     jne loopString
;     popa
;                     RET
; WriteGuidline ENDP 
; ;function that draw Horizontal line  like in the image of main game
; ;takes row in DX as i input just specify the row you want to draw in before calling it
; DrawHorizontal PROC
;     pusha
;     ;drawing pixel
;     MOV CX,20 ; coloumn
;     ;MOV DX,0    ; row
;     mov al,00H ; color
;     MOV AH,0ch  ; draw pixel  
    
; drawLoop1:
;     INT 10h   
;     INC CX
;     CMP CX,620
; JNE drawLoop1 
; 	popa
; 	RET
; DrawHorizontal ENDP
; ;takes coloumn and draw vertical line between the two horizontal lines takes cx as input for coloumn
; DrawVertical PROC
;     pusha
;     ;drawing pixel
;     MOV DX,20 ; row
;     ;MOV CX,0    ; col
;     mov al,00H ; color
;     MOV AH,0ch  ; draw pixel  
    
; drawLoop2:
;     INT 10h   
;     INC DX
;     CMP DX,450
; JNE drawLoop2 
; 	popa
; 	RET
; DrawVertical ENDP
; ;that function will take place col in RowNum and row in ColNum  and will draw the nodes and
; DrawNode PROC
;     pusha
;      ;drawing pixel
;      MOV DX,RowNum ; row
;      MOV CX,ColNum    ; coloumn
    
;     ;     ;draw hieght  
;     ;     MOV BX,DX         
;     ;     ADD BX,3
;     ; drawLoop4:
;     ;     INT 10h   
;     ;     INC DX
;     ;     CMP DX,BX
;     ; JNE drawLoop4
;     MOV si,3 ;THATS THE number that LINE get DRAWN AND GET DOWN TO CREATE THE THICKNESS
;     BIGLOOP:
;             ;draw width
;             MOV di,CX ; BX is just a register to compare with the relative width whatever the place
;             ADD di,15 ;width is 15 pixels
;         drawLoop5:
;              mov al,00H ; color
;             MOV AH,0ch  ; draw pixel  
;             INT 10h   
;             INC CX
;             CMP CX,di
;         JNE drawLoop5
;         INC DX
;         MOV CX,ColNum 
;         DEC si
;     JNZ BIGLOOP		
;     ;drawLoop5:
;     ;  INT 10h   
;     ;   INC DX
;     ;  CMP DX,BX
;     ;JNE drawLoop5
;     popa
; 	RET
; DrawNode ENDP
; ;draw lines of start and end with width equal 15 set row by DX
; DrawSourceAndDestination PROC
;         pusha
;         ;drawing pixel
;         MOV CX,0 ; coloumn
;         ;MOV DX,0    ; row
;         mov al,00H ; color
;         MOV AH,0ch  ; draw pixel  
        
;     drawLoop9:
;         INT 10h   
;         INC CX
;         CMP CX,15
;     JNE drawLoop9 
; 	popa
; 	RET
; DrawSourceAndDestination ENDP
; ;its literally the same function as DrawSourceAndDestination but its for the destination 
; ;thats because when i tried to set cx outside function it gets an error 
; DrawSourceAndDestination1 PROC
;         pusha
  
;         ;drawing pixel
;         MOV CX,625 ; coloumn
;         ;MOV DX,0    ; row
;         mov al,00H ; color
;         MOV AH,0ch  ; draw pixel  
        
;     drawLoop11:
;         INT 10h   
;         INC CX
;         CMP CX,640
;     JNE drawLoop11 
; 	popa
; 	RET
; DrawSourceAndDestination1 ENDP
; ;function that draw the numbers of coloumns below
; DrawNumberBelow PROC
;     pusha
;     ;printing zero 
    
;     ;changing curser position 
;     mov ah,2h
;     mov dx,1b01h ; y is dh  and x is dl
;     int 10h

;     ;printing zero
;      mov ah,09h  ;setting mode for int 10 to draw character   
;      mov bh, 00h   ;page 0    
;     mov al,'0' ; Put the character in address bp
;      mov bl, 0f2h ;E is yello (background) 0 is black (font)
;     mov cx,1h  
;     int 10h



;         ;changing curser position 
;     mov ah,2h
;     mov dx,1b07h ; y is dh  and x is dl
;     int 10h

;     ;draw statment
;     mov di,offset Numbersfrom0to9
;     mov si,0h
;     inc di
;     loopNumbers:
;         ;draw one character 
;         mov ah,09h  ;setting mode for int 10 to draw character   
;          mov bh, 00h   ;page 0    
;         mov al,[di] ; Put the character in address bp
;        inc di      ;getting the next character from array of characters
       
;         mov bl, 0f2h ;E is yello (background) 0 is black (font)
;         mov cx,1h  
;          int 10h
;         ;moving curser     
         
;         mov ah,2h
;         add dl,04d
;        int 10h
        
;         inc si
;         cmp si,17
;      jne loopNumbers
          
;     ;printing 9
;     ;changing curser position 
;     mov ah,2h
;     mov dx,1beeh ; y is dh  and x is dl
;     int 10h

;     ;printing zero
;      mov ah,09h  ;setting mode for int 10 to draw character   
;      mov bh, 00h   ;page 0    
;     mov al,'9' ; Put the character in address bp
;      mov bl, 0f2h ;E is yello (background) 0 is black (font)
;     mov cx,1h  
;     int 10h
;     popa
;                     RET
; DrawNumberBelow ENDP
; ;take the number of coloumn and draw the numbers in that coloumn takes number if coloumn at DX x axis
; DrawverticalNumbers PROC
;     pusha
;     ;changing curser position 
;     mov ah,2h
;     mov dh,01d ; y is dh  and x is dl
;     int 10h

   
    
;     mov al,48d ;changing counter to print numbers from 0 to 9
;     loopNumberss:
;         ;draw one character 
;         mov bh, 00h   ;page 0    
;     mov bl, 0f2h ;E is yello (background) 0 is black (font)
;      mov cx,1h  ; print it one time
;          mov ah,09h  ;setting mode for int 10 to draw character 
;          int 10h
        
;         ;moving curser     
;         mov ah,2h   ; setting mode for int 10 to move curser
;         add dh,003h ; shift curser down by one character
;         int 10h
        
;         inc al  ;increment counter to print the next number
;         cmp al,58 ; if its 10 then dont print it
;      jne loopNumberss
;     popa
;     RET  
; DrawverticalNumbers ENDP
; ;GIVEN CX AND DX DRAW NODE AT THIS
; draw_labels proc
;     pusha
;                                                mov bx,ColNum
;                                                 mov bp,RowNum
;                                                 add bx,15
;                                                 add bp,3
;                                                  mov   al, 00h
;                                                 mov   ah, 0ch
;     draw_label1_y:                              

;     draw_label1_x:                              

;                                                 int   10h
;                                                 inc   cx
;                                                 cmp   cx, bx 
;                                                 jnz   draw_label1_x

;                                                 inc   dx
;                                                 mov   cx, ColNum
;                                                 cmp   dx, bp
;                                                 jnz   draw_label1_y
;                                                 popa
;   draw_labels ENDP                          




;-------------------------------------------BASSEM FUNCTIONS FOR DRAWING SCREEN-----------------------------------------------------
    ;function that draw horizontal line used in changebackgroundcolor
    DrawScreen PROC
        pusha
        ;drawing pixel
        MOV CX,0 ; coloumn
        ;MOV DX,0    ; row
        mov al,0FH ; color
        MOV AH,0ch  ; draw pixel  
        
    drawLoop:
        INT 10h   
        INC CX
        CMP CX,640
    JNE drawLoop 
        popa
        RET
    DrawScreen ENDP
    ;function that change background color but by texting white color on the screen
    changeBackgroundColorbyText PROC
        ;mov ax,06ffh
        ;mov bh,05h
        ;mov cx,0000h
        ;mov dx,0ffffh
    ; int 10
        ;method 1 writing white characters in the background
            mov ah,9
            mov bh,0
            mov al,0
            mov cx,0ffffh
            mov bl,0000
            int 10h
    changeBackgroundColorbyText ENDP
    ;function that change background color to white
    changeBackgroundColor PROC    
                    pusha 
                MOV DX,0    ; row
                loop1:
                        CALL DrawScreen
                        inc dx  
                        cmp dx,480d
                jne loop1     
                    popa
                        RET
    changeBackgroundColor ENDP 
    ;function that write guideline the statement in the middle of the screen
    WriteGuidline PROC    
                pusha        
            ;changing curser position 
        mov ah,2h
        mov dx,0067h
        int 10h

        ;draw statment
        mov di,offset Guideline
        mov si,0h
        loopString:
            ;draw one character 
            mov ah,09h  ;setting mode for int 10 to draw character       
            mov al,[di] ; Put the character in address bp
            inc di      ;getting the next character from array of characters
            mov bh, 00h   ;page 0
            mov bl, 03Eh ;E is yello (background) 0 is black (font)
            mov cx,1h  
            int 10h
            ;moving curser     
            
            mov ah,2h
            inc dx
            int 10h
            
            inc si
            cmp si,36
        jne loopString
        popa
                        RET
    WriteGuidline ENDP 
    ;function that draw Horizontal line  like in the image of main game
    ;takes row in DX as i input just specify the row you want to draw in before calling it
    DrawHorizontal PROC
        pusha
        ;drawing pixel
        MOV CX,20 ; coloumn
        ;MOV DX,0    ; row
        mov al,00H ; color
        MOV AH,0ch  ; draw pixel  
        
    drawLoop1:
        INT 10h   
        INC CX
        CMP CX,620
    JNE drawLoop1 
        popa
        RET
    DrawHorizontal ENDP
    ;takes coloumn and draw vertical line between the two horizontal lines takes cx as input for coloumn
    DrawVertical PROC
        pusha
        ;drawing pixel
        MOV DX,20 ; row
        ;MOV CX,0    ; col
        mov al,00H ; color
        MOV AH,0ch  ; draw pixel  
        
    drawLoop2:
        INT 10h   
        INC DX
        CMP DX,450
    JNE drawLoop2 
        popa
        RET
    DrawVertical ENDP
    ;that function will take place col in RowNum and row in ColNum  and will draw the nodes and
    DrawNode PROC
        pusha
        ;drawing pixel
        MOV DX,RowNum ; row
        MOV CX,ColNum    ; coloumn
        
        ;     ;draw hieght  
        ;     MOV BX,DX         
        ;     ADD BX,3
        ; drawLoop4:
        ;     INT 10h   
        ;     INC DX
        ;     CMP DX,BX
        ; JNE drawLoop4
        MOV si,3 ;THATS THE number that LINE get DRAWN AND GET DOWN TO CREATE THE THICKNESS
        BIGLOOP:
                ;draw width
                MOV di,CX ; BX is just a register to compare with the relative width whatever the place
                ADD di,15 ;width is 15 pixels
            drawLoop5:
                mov al,00H ; color
                MOV AH,0ch  ; draw pixel  
                INT 10h   
                INC CX
                CMP CX,di
            JNE drawLoop5
            INC DX
            MOV CX,ColNum 
            DEC si
        JNZ BIGLOOP		
        ;drawLoop5:
        ;  INT 10h   
        ;   INC DX
        ;  CMP DX,BX
        ;JNE drawLoop5
        popa
        RET
    DrawNode ENDP
    ;draw lines of start and end with width equal 15 set row by DX
    DrawSourceAndDestination PROC
            pusha
            ;drawing pixel
            MOV CX,0 ; coloumn
            ;MOV DX,0    ; row
            mov al,00H ; color
            MOV AH,0ch  ; draw pixel  
            
        drawLoop9:
            INT 10h   
            INC CX
            CMP CX,15
        JNE drawLoop9 
        popa
        RET
    DrawSourceAndDestination ENDP
    ;its literally the same function as DrawSourceAndDestination but its for the destination 
    ;thats because when i tried to set cx outside function it gets an error 
    DrawSourceAndDestination1 PROC
            pusha
    
            ;drawing pixel
            MOV CX,625 ; coloumn
            ;MOV DX,0    ; row
            mov al,00H ; color
            MOV AH,0ch  ; draw pixel  
            
        drawLoop11:
            INT 10h   
            INC CX
            CMP CX,640
        JNE drawLoop11 
        popa
        RET
    DrawSourceAndDestination1 ENDP
    ;function that draw the numbers of coloumns below
    DrawNumberBelow PROC
        pusha
        ;printing zero 
        
        ;changing curser position 
        mov ah,2h
        mov dx,1b01h ; y is dh  and x is dl
        int 10h

        ;printing zero
        mov ah,09h  ;setting mode for int 10 to draw character   
        mov bh, 00h   ;page 0    
        mov al,'0' ; Put the character in address bp
        mov bl, 0f2h ;E is yello (background) 0 is black (font)
        mov cx,1h  
        int 10h



            ;changing curser position 
        mov ah,2h
        mov dx,1b07h ; y is dh  and x is dl
        int 10h

        ;draw statment
        mov di,offset Numbersfrom0to9
        mov si,0h
        inc di
        loopNumbers:
            ;draw one character 
            mov ah,09h  ;setting mode for int 10 to draw character   
            mov bh, 00h   ;page 0    
            mov al,[di] ; Put the character in address bp
        inc di      ;getting the next character from array of characters
        
            mov bl, 0f2h ;E is yello (background) 0 is black (font)
            mov cx,1h  
            int 10h
            ;moving curser     
            
            mov ah,2h
            add dl,04d
        int 10h
            
            inc si
            cmp si,17
        jne loopNumbers
            
        ;printing 9
        ;changing curser position 
        mov ah,2h
        mov dx,1beeh ; y is dh  and x is dl
        int 10h

        ;printing zero
        mov ah,09h  ;setting mode for int 10 to draw character   
        mov bh, 00h   ;page 0    
        mov al,'9' ; Put the character in address bp
        mov bl, 0f2h ;E is yello (background) 0 is black (font)
        mov cx,1h  
        int 10h
        popa
                        RET
    DrawNumberBelow ENDP
    ;take the number of coloumn and draw the numbers in that coloumn takes number if coloumn at DX x axis
    DrawverticalNumbers PROC
        pusha
        ;changing curser position 
        mov ah,2h
        mov dh,01d ; y is dh  and x is dl
        int 10h

    
        
        mov al,48d ;changing counter to print numbers from 0 to 9
        loopNumberss:
            ;draw one character 
            mov bh, 00h   ;page 0    
        mov bl, 0f2h ;E is yello (background) 0 is black (font)
        mov cx,1h  ; print it one time
            mov ah,09h  ;setting mode for int 10 to draw character 
            int 10h
            
            ;moving curser     
            mov ah,2h   ; setting mode for int 10 to move curser
            add dh,003h ; shift curser down by one character
            int 10h
            
            inc al  ;increment counter to print the next number
            cmp al,58 ; if its 10 then dont print it
        jne loopNumberss
        popa
        RET  
    DrawverticalNumbers ENDP                   
    ;DRAW numbers
    draw_timer_0 proc

                                                    pusha

                                                ; add   di, 33d
                                                ;  add   si, 31d

                                                    mov   bp, di
                                                    mov   bx, si

                                                    add   bp, 08d
                                                    add   bx, 9d

                                                    mov   cx, di
                                                    mov   dx, si
                                                    mov   al, 00h
                                                    mov   ah, 0ch

                                                    mov   si, 0

        timer_0_y:                                  
        timer_0_x:                                  
                                                    cmp   timer_0 + [si],1
                                                    je    draw_the_timer_0
        back_from_timer_0:                          
                                                    inc   si
                                                    inc   cx
                                                    cmp   cx, bp
                                                    jnz   timer_0_x
                                                    mov   cx, di
                                                    inc   dx
                                                    cmp   dx, bx
                                                    jnz   timer_0_y

                                                    jmp   end_timer_0

        draw_the_timer_0:                           
                                                    int   10h
                                                    jmp   back_from_timer_0

        end_timer_0:                                

                                                    popa

                                                    ret

    draw_timer_0 endp
    draw_timer_1 proc

                                                    pusha

                                                ; add   di, 33d
                                                ;  add   si, 31d

                                                    mov   bp, di
                                                    mov   bx, si

                                                    add   bp, 08d
                                                    add   bx, 9d

                                                    mov   cx, di
                                                    mov   dx, si
                                                    mov   al, 00h
                                                    mov   ah, 0ch

                                                    mov   si, 0

        timer_1_y:                                  
        timer_1_x:                                  
                                                    cmp   timer_1 + [si],1
                                                    je    draw_the_timer_1
        back_from_timer_1:                          
                                                    inc   si
                                                    inc   cx
                                                    cmp   cx, bp
                                                    jnz   timer_1_x
                                                    mov   cx, di
                                                    inc   dx
                                                    cmp   dx, bx
                                                    jnz   timer_1_y

                                                    jmp   end_timer_1

        draw_the_timer_1:                           
                                                    int   10h
                                                    jmp   back_from_timer_1

        end_timer_1:                                

                                                    popa

                                                    ret

    draw_timer_1 endp
    draw_timer_2 proc

                                                    pusha

                                                ; add   di, 33d
                                                ;  add   si, 31d

                                                    mov   bp, di
                                                    mov   bx, si

                                                    add   bp, 08d
                                                    add   bx, 9d

                                                    mov   cx, di
                                                    mov   dx, si
                                                    mov   al, 00h
                                                    mov   ah, 0ch

                                                    mov   si, 0

        timer_2_y:                                  
        timer_2_x:                                  
                                                    cmp   timer_2 + [si],1
                                                    je    draw_the_timer_2
        back_from_timer_2:                          
                                                    inc   si
                                                    inc   cx
                                                    cmp   cx, bp
                                                    jnz   timer_2_x
                                                    mov   cx, di
                                                    inc   dx
                                                    cmp   dx, bx
                                                    jnz   timer_2_y

                                                    jmp   end_timer_2

        draw_the_timer_2:                           
                                                    int   10h
                                                    jmp   back_from_timer_2

        end_timer_2:                                

                                                    popa

                                                    ret

    draw_timer_2 endp
    draw_timer_3 proc

                                                    pusha

                                                ; add   di, 33d
                                                ;  add   si, 31d

                                                    mov   bp, di
                                                    mov   bx, si

                                                    add   bp, 08d
                                                    add   bx, 9d

                                                    mov   cx, di
                                                    mov   dx, si
                                                    mov   al, 00h
                                                    mov   ah, 0ch

                                                    mov   si, 0

        timer_3_y:                                  
        timer_3_x:                                  
                                                    cmp   timer_3 + [si],1
                                                    je    draw_the_timer_3
        back_from_timer_3:                          
                                                    inc   si
                                                    inc   cx
                                                    cmp   cx, bp
                                                    jnz   timer_3_x
                                                    mov   cx, di
                                                    inc   dx
                                                    cmp   dx, bx
                                                    jnz   timer_3_y

                                                    jmp   end_timer_3

        draw_the_timer_3:                           
                                                    int   10h
                                                    jmp   back_from_timer_3

        end_timer_3:                                

                                                    popa

                                                    ret

    draw_timer_3 endp
    draw_timer_4 proc

                                                    pusha

                                                ; add   di, 33d
                                                ;  add   si, 31d

                                                    mov   bp, di
                                                    mov   bx, si

                                                    add   bp, 08d
                                                    add   bx, 9d

                                                    mov   cx, di
                                                    mov   dx, si
                                                    mov   al, 00h
                                                    mov   ah, 0ch

                                                    mov   si, 0

        timer_4_y:                                  
        timer_4_x:                                  
                                                    cmp   timer_4 + [si],1
                                                    je    draw_the_timer_4
        back_from_timer_4:                          
                                                    inc   si
                                                    inc   cx
                                                    cmp   cx, bp
                                                    jnz   timer_4_x
                                                    mov   cx, di
                                                    inc   dx
                                                    cmp   dx, bx
                                                    jnz   timer_4_y

                                                    jmp   end_timer_4

        draw_the_timer_4:                           
                                                    int   10h
                                                    jmp   back_from_timer_4

        end_timer_4:                                

                                                    popa

                                                    ret

    draw_timer_4 endp
    draw_timer_5 proc

                                                    pusha

                                                ; add   di, 33d
                                                ;  add   si, 31d

                                                    mov   bp, di
                                                    mov   bx, si

                                                    add   bp, 08d
                                                    add   bx, 9d

                                                    mov   cx, di
                                                    mov   dx, si
                                                    mov   al, 00h
                                                    mov   ah, 0ch

                                                    mov   si, 0

        timer_5_y:                                  
        timer_5_x:                                  
                                                    cmp   timer_5 + [si],1
                                                    je    draw_the_timer_5
        back_from_timer_5:                          
                                                    inc   si
                                                    inc   cx
                                                    cmp   cx, bp
                                                    jnz   timer_5_x
                                                    mov   cx, di
                                                    inc   dx
                                                    cmp   dx, bx
                                                    jnz   timer_5_y

                                                    jmp   end_timer_5

        draw_the_timer_5:                           
                                                    int   10h
                                                    jmp   back_from_timer_5

        end_timer_5:                                

                                                    popa

                                                    ret

    draw_timer_5 endp
    draw_timer_6 proc

                                                    pusha

                                                ; add   di, 33d
                                                ;  add   si, 31d

                                                    mov   bp, di
                                                    mov   VarToCompare, si

                                                    add   bp, 08d
                                                    add   VarToCompare, 9d

                                                    mov   cx, di
                                                    mov   dx, si
                                                    mov   al, 00h
                                                    mov   ah, 0ch

                                                    mov   si, 0

        timer_6_y:                                  
        timer_6_x:                                  
                                                    cmp   timer_6 + [si],1
                                                    je    draw_the_timer_6
        back_from_timer_6:                          
                                                    inc   si
                                                    inc   cx
                                                    cmp   cx, bp
                                                    jnz   timer_6_x
                                                    mov   cx, di
                                                    inc   dx
                                                    cmp   dx, VarToCompare
                                                    jnz   timer_6_y

                                                    jmp   end_timer_6

        draw_the_timer_6:                           
                                                    int   10h
                                                    jmp   back_from_timer_6

        end_timer_6:                                

                                                    popa

                                                    ret

    draw_timer_6 endp
    draw_timer_7 proc

                                                    pusha

                                                ; add   si, 33d
                                                ;  add   di, 31d

                                                    mov   bp, si
                                                    mov   VarToCompare, di

                                                    add   bp, 08d
                                                    add   VarToCompare, 9d

                                                    mov   cx, si
                                                    mov   dx, di
                                                    mov   al, 00h
                                                    mov   ah, 0ch

                                                    mov   di, 0

        timer_7_y:                                  
        timer_7_x:                                  
                                                    cmp   timer_7 + [di],1
                                                    je    draw_the_timer_7
        back_from_timer_7:                          
                                                    inc   di
                                                    inc   cx
                                                    cmp   cx, bp
                                                    jnz   timer_7_x
                                                    mov   cx, si
                                                    inc   dx
                                                    cmp   dx, VarToCompare
                                                    jnz   timer_7_y

                                                    jmp   end_timer_7

        draw_the_timer_7:                           
                                                    int   10h
                                                    jmp   back_from_timer_7

        end_timer_7:                                

                                                    popa

                                                    ret

    draw_timer_7 endp
    draw_timer_8 proc

                                                    pusha

                                                ; add   si, 33d
                                                ;  add   di, 31d

                                                    mov   bp, si
                                                    mov   VarToCompare, di

                                                    add   bp, 08d
                                                    add   VarToCompare, 9d

                                                    mov   cx, si
                                                    mov   dx, di
                                                    mov   al, 00h
                                                    mov   ah, 0ch

                                                    mov   di, 0

        timer_8_y:                                  
        timer_8_x:                                  
                                                    cmp   timer_8 + [di],1
                                                    je    draw_the_timer_8
        back_from_timer_8:                          
                                                    inc   di
                                                    inc   cx
                                                    cmp   cx, bp
                                                    jnz   timer_8_x
                                                    mov   cx, si
                                                    inc   dx
                                                    cmp   dx, VarToCompare
                                                    jnz   timer_8_y

                                                    jmp   end_timer_8

        draw_the_timer_8:                           
                                                    int   10h
                                                    jmp   back_from_timer_8

        end_timer_8:                                

                                                    popa

                                                    ret

    draw_timer_8 endp
    draw_timer_9 proc

                                                    pusha

                                                ; add   si, 33d
                                                ;  add   di, 31d

                                                    mov   bp, si
                                                    mov   VarToCompare, di

                                                    add   bp, 08d
                                                    add   VarToCompare, 9d

                                                    mov   cx, si
                                                    mov   dx, di
                                                    mov   al, 00h
                                                    mov   ah, 0ch

                                                    mov   di, 0

        timer_9_y:                                  
        timer_9_x:                                  
                                                    cmp   timer_9 + [di],1
                                                    je    draw_the_timer_9
        back_from_timer_9:                          
                                                    inc   di
                                                    inc   cx
                                                    cmp   cx, bp
                                                    jnz   timer_9_x
                                                    mov   cx, si
                                                    inc   dx
                                                    cmp   dx, VarToCompare
                                                    jnz   timer_9_y

                                                    jmp   end_timer_9

        draw_the_timer_9:                           
                                                    int   10h
                                                    jmp   back_from_timer_9

        end_timer_9:                                

                                                    popa

                                                    ret

    draw_timer_9 endp
    ;function that calls all drawnumbers
    DrawAllNumber PROC
        pusha
        ;FOR ODD COLOUMN
        mov di,85 ; mov right or left
        mov si,35 ; mov up or down
        LOOP12:
            CALL draw_timer_1
            add di,140
            cmp di,645
        jne LOOP12

        ;FOR ODD COLOUMN
        mov di,85 ; mov right or left
        mov si,35+40 ; mov up or down
        LOOP13:
            CALL draw_timer_2
            add di,140
            cmp di,645
        jne LOOP13
        ;FOR ODD COLOUMN
        mov di,85 ; mov right or left
        mov si,35+40+40 ; mov up or down
        LOOP14:
            CALL draw_timer_3
            add di,140
            cmp di,645
        jne LOOP14
            ;FOR ODD COLOUMN
        mov di,85 ; mov right or left
        mov si,35+40+40+40 ; mov up or down
        LOOP15:
            CALL draw_timer_4
            add di,140
            cmp di,645
        jne LOOP15

        mov di,85 ; mov right or left
        mov si,35+40+40+40+40; mov up or down
        LOOP16:
            CALL draw_timer_5
            add di,140
            cmp di,645
        jne LOOP16
        mov di,85 ; mov right or left
        mov si,35+40+40+40+40+40; mov up or down
        LOOP17:
            CALL draw_timer_6
            add di,140
            cmp di,645
        jne LOOP17
    pusha
        mov si,85 ; mov right or left
        mov di,35+40+40+40+40+40+40; mov up or down
        LOOP18:
            CALL draw_timer_7
            add si,140
            cmp si,645
        jne LOOP18
    popa
    pusha
        mov si,85 ; mov right or left
        mov di,35+40+40+40+40+40+40+40; mov up or down
        LOOP19:
            CALL draw_timer_8
            add si,140
            cmp si,645
        jne LOOP19
    popa
    pusha
        mov si,85 ; mov right or left
        mov di,35+40+40+40+40+40+40+40+40; mov up or down
        LOOP20:
            CALL draw_timer_9
            add si,140
            cmp si,645
        jne LOOP20
    popa




        ;FOR EVEN COLOUMN
        mov di,155 ; mov right or left
        mov si,55 ; mov up or down
        LOOP014:
            CALL draw_timer_1
            add di,140
            cmp di,715
        jne LOOP014
            ;FOR EVEN COLOUMN
        mov di,155 ; mov right or left
        mov si,55+40 ; mov up or down
        LOOP015:
            CALL draw_timer_2
            add di,140
            cmp di,715
        jne LOOP015
            ;FOR EVEN COLOUMN
        mov di,155 ; mov right or left
        mov si,55+40+40 ; mov up or down
        LOOP0153:
            CALL draw_timer_3
            add di,140
            cmp di,715
        jne LOOP0153
            ;FOR EVEN COLOUMN
        mov di,155 ; mov right or left
        mov si,55+40+40+40 ; mov up or down
        LOOP0154:
            CALL draw_timer_4
            add di,140
            cmp di,715
        jne LOOP0154
                ;FOR EVEN COLOUMN
        mov di,155 ; mov right or left
        mov si,55+40+40+40+40 ; mov up or down
        LOOP0155:
            CALL draw_timer_5
            add di,140
            cmp di,715
        jne LOOP0155
                ;FOR EVEN COLOUMN
        mov di,155 ; mov right or left
        mov si,55+40+40+40+40+40 ; mov up or down
        LOOP0156:
            CALL draw_timer_6
            add di,140
            cmp di,715
        jne LOOP0156
    
        mov si,155 ; mov right or left
        mov di,55+40+40+40+40+40+40 ; mov up or down
        LOOP0157:
            CALL draw_timer_7
            add si,140
            cmp si,715
        jne LOOP0157
        mov si,155 ; mov right or left
        mov di,55+40+40+40+40+40+40+40 ; mov up or down
        LOOP0158:
            CALL draw_timer_8
            add si,140
            cmp si,715
        jne LOOP0158
        mov si,155 ; mov right or left
        mov di,55+40+40+40+40+40+40+40+40 ; ; mov up or down
        LOOP0159:
            CALL draw_timer_9
            add si,140
            cmp si,715
        jne LOOP0159
    ;drawing numbers at soucre and index
        ;FOR ODD COLOUMN
        mov di,20 ; mov right or left
        mov si,25 ; mov up or down
        
        LOOP0z:
            CALL draw_timer_0
            add di,590
            cmp di,1200
        jne LOOP0z
    
        mov di,20 ; mov right or left
        mov si,25+40 ; mov up or down
        
        LOOP1z:
            CALL draw_timer_1
            add di,590
            cmp di,1200
        jne LOOP1z

        mov di,20 ; mov right or left
        mov si,25+40+40 ; mov up or down
        LOOP2z:
            CALL draw_timer_2
            add di,590
            cmp di,1200
        jne LOOP2z

            mov di,20 ; mov right or left
        mov si,25+40+40+40 ; mov up or down
        LOOP3z:
            CALL draw_timer_3
            add di,590
            cmp di,1200
        jne LOOP3z
        
            mov di,20 ; mov right or left
        mov si,25+40+40+40+40 ; mov up or down
        LOOP4z:
            CALL draw_timer_4
            add di,590
            cmp di,1200
        jne LOOP4z

            mov di,20 ; mov right or left
        mov si,25+40+40+40+40+40 ; mov up or down
        LOOP5z:
            CALL draw_timer_5
            add di,590
            cmp di,1200
        jne LOOP5z

        mov di,20 ; mov right or left
        mov si,25+40+40+40+40+40+40 ; mov up or down
        LOOP6z:
            CALL draw_timer_6
            add di,590
            cmp di,1200
        jne LOOP6z

        mov si,20 ; mov right or left
        mov di,25+40+40+40+40+40+40+40 ; mov up or down
        LOOP7z:
            CALL draw_timer_7
            add si,590
            cmp si,1200
        jne LOOP7z

                mov si,20 ; mov right or left
        mov di,25+40+40+40+40+40+40+40+40; mov up or down
        LOOP8z:
            CALL draw_timer_8
            add si,590
            cmp si,1200
        jne LOOP8z

                mov si,20 ; mov right or left
        mov di,25+40+40+40+40+40+40+40+40+40 ; mov up or down
        LOOP9z:
            CALL draw_timer_9
            add si,590
            cmp si,1200
        jne LOOP9z
        popa
        ret
    DrawAllNumber ENDP



;   ReadString PROC FAR
; ; Reads a string from standard input and places the
; ; characters in a buffer. Reads past end of line,
; ; and removes the CF/LF from the string.
; ; Receives: DS:DX points to the input buffer,
; ;           CX = maximum input chars, plus 1.
; ; Returns:  AX = size of the input string.
; ; Comments: Stops either when Enter (0Dh) is pressed,
; ; or (CX-1) characters have been read.
; ; Bug fixed 9/22/03: return value was too large by 1
; ;--------------------------------------------------------
; 	push cx		; save registers
; 	push si
; 	push cx		; save digit count again
; 	mov  si,dx		; point to input buffer
; 	dec  cx		; save room for null byte

; L1:	mov  ah,1		; function: keyboard input
; 	int  21h		; DOS returns char in AL
; 	cmp  al,0Dh		; end of line?
; 	je   L2		; yes: exit
; 	mov  [si],al		; no: store the character
; 	inc  si		; increment buffer pointer
; 	loop L1		; loop until CX=0

; L2:	mov  byte ptr [si],0		; end with a null byte
; 	pop  ax		; original digit count
; 	sub  ax,cx		; AX = size of input string
; 	dec  ax		; ADDED 9/22/03	
; 	pop  si		; restore registers
; 	pop  cx
; 	ret
; ReadString ENDP 
wString proc FAR; this was near in original,edited on 21/4/2023
;expects a string's offset to be loaded into dx and it terminated with a $
    pusha 
    mov ah,09h
    mov al,02h
    mov bx,1
    int 21h
    popa
    RET 
    wString endp

COPYSTRING PROC NEAR; EXPECTS SI TO HAVE OFFSET TO SOURCE AND DI TO HAVE OFFSET TO DESTINATION
    PUSHA
    COPY101:
    MOV CL,[SI]
    MOV [BX+DI],CL
     INC BX
    INC SI
    CMP BL,COPYSTRING_SIZE
    JNZ COPY101
    POPA
    RET
    COPYSTRING ENDP    


    LOWER_TO_UPPER MACRO STRING
    LOCAL NOTVALID
    LOCAL ASPIRE
        PUSHA          
        MOV AL,[STRING]
        CMP AL,97
        JL NOTVALID
        ASPIRE:
        CMP AL,122
        JG NOTVALID
        SUB AL,32D
        MOV [STRING],AL
        NOTVALID:
        POPA
        ENDM LOWER_TO_UPPER

 WriteInt PROC FAR ;;NOT REALLY WORKING OR I DON'T GET WHERE THE DISPLAY SHOULD BE 
;
; Writes a 32-bit signed binary integer to standard output
; in ASCII decimal.
; Receives: ax = the integer
; Returns:  nothing
; Comments: Displays a leading sign, no leading zeros.
;-----------------------------------------------------

	 ;pushad ; REPLACED BY THE LINE BELOW, COMMENT BY ME 27/3/2023
	 PUSH SP

	 mov   neg_flag,false    ; assume neg_flag is false
	 or    ax,ax             ; is AX positive?
	 jns   WIS1              ; yes: jump to B1
	 neg   ax                ; no: make it positive
	 mov   neg_flag,true     ; set neg_flag to true

WIS1:
	 mov   cx,0              ; digit count = 0
	 mov   di,OFFSET buffer_B
	 add   di,(WI_Bufsize-1)
	 mov   bx,10             ; will divide by 10

WIS2:
	 mov   dx,0              ; set dividend to 0
	 div   bx                ; divide AX by 10
	 or    dl,30h            ; convert remainder to ASCII
	 dec   di                ; reverse through the buffer
	 mov   [di],dl           ; store ASCII digit
	 inc   cx                ; increment digit count
	 or    ax,ax             ; quotient > 0?
	 jnz   WIS2              ; yes: divide again

	 ; Insert the sign.
; jmp WIS3:;;ADDED ON 28/3/2023 to avoid the +/- sign
; 	 dec   di	; back up in the buffer
; 	 inc   cx               	; increment counter
; 	 mov   byte ptr [di],'+' 	; insert plus sign
; 	 cmp   neg_flag,false    	; was the number positive?
; 	 jz    WIS3              	; yes
; 	 mov   byte ptr [di],'-' 	; no: insert negative sign

WIS3:	; Display the number
	mov  dx,di
	;call WriteString
    ; MOV BX,CX
    ; ADDITIONLOOPLOCAL:
    ; ADD [DI+BX],7
PUSH BX
MOV BX,CX
    ADDEDLOOP:;ADDED BY ME 28/3/2023
    cmp colorbool,1 ;ADDED BY ME 28/3/2023
    jne normal101print;ADDED BY ME 28/3/2023
    COLOREDCHAR [di]
    inc col
    movecursor row,col
    jmp h56
    normal101print:;ADDED BY ME 28/3/2023
    PRINTCHAR [DI]
    h56:
    INC DI
    LOOP ADDEDLOOP
    mov colorbool,0
    POP BX 
;call wstr2; ADDED BY ME 27/3/2023
	;popad ; REPLACED BY THE LINE BELOW, COMMENT BY ME 27/3/2023
    POP SP
	ret
WriteInt ENDP
