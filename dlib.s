; System procedures
.export _waitVSync
; Draw procedures
.export _fillScreen
.export _draw_rect
; Debug procedures
.export _consoleLogHex
.export _consoleLogWord
.export _consoleLogBinary
.export _consoleLogDecimal
.export _consoleLogChar
.export _consoleLogStr
.export _startStopwatch
.export _stopStopwatch

.zeropage
VMEM_POINTER: .res 2, $00
DATA_POINTER: .res 2, $00


.segment "CODE"
.PC02

; ----- SYSTEM PROCEDURES ---

.proc _waitVSync: near
    ; Wait for vsync end.
    loop1:
    BIT $DF88
    BVS loop1
    ; Wait for vsync start
    loop2:
    BIT $DF88
    BVC loop2
    RTS
.endproc

; ---- DRAW PROCEDURES ---
.proc _fillScreen: near
    ; Init video pointer
    LDX #$60
    STX VMEM_POINTER+1
    LDY #$00
    STY VMEM_POINTER
    ; Load current color
loop:
    STA (VMEM_POINTER), Y
    INY
    BNE loop
	INC VMEM_POINTER+1
    BPL loop
    RTS
.endproc

.proc _draw_rect: near
    JMP _convert_coords_to_mem
.endproc

; Converts x,y coord into memory pointer.
; X_COORD, Y_COORD pixel coords
; VMEM_POINTER VMEM_POINTER+1 current video memory pointer
.proc _convert_coords_to_mem: near
    ; Read pointer location
    STA DATA_POINTER
    STX DATA_POINTER+1
    
    ; Calculate Y coord
    ; Clear VMEM_POINTER
    STZ VMEM_POINTER
    ; Multiply y coord by 64 (64 bytes each row)
    ; LDA Y_COORD
    LDY 1
    LDA (DATA_POINTER),y
    LSR
    ROR VMEM_POINTER
    LSR
    ROR VMEM_POINTER
    
    ; Add base memory address
    ADC #$80
    STA VMEM_POINTER+1
        
    ; Calculate X coord
    ; Divide x coord by 2 (2 pixel each byte)
    ;LDA X_COORD
    LDA (DATA_POINTER)
    LSR
    ; Add to memory address
    CLC
    ADC VMEM_POINTER
    STA VMEM_POINTER
    BCC skip_upper
    INC VMEM_POINTER+1
    skip_upper:
    
    LDA VMEM_POINTER
    LDY 2
    STA DATA_POINTER,y
    STA $df93
    
    LDA VMEM_POINTER+1
    LDY 3
    STA DATA_POINTER,y
    STA $df93
    
	RTS
.endproc
; --- end convert_coords_to_mem ---

; ------ DEBUG PROCEDURES

.proc  _consoleLogHex: near
    ; Set virtual serial port in hex mode
    LDX #$00
	STX $df94
    ; Send value to virtual serial port
    STA $df93
    RTS
.endproc

.proc  _consoleLogWord: near
    ; Set virtual serial port in hex mode
    LDY #$00
	STY $df94
    ; Send value to virtual serial port
    STA $df93
    STX $df93
    RTS
.endproc

.proc  _consoleLogBinary: near
    ; Set virtual serial port in hex mode
    LDX #$02
	STX $df94
    ; Send value to virtual serial port
    STA $df93
    RTS
.endproc

.proc  _consoleLogDecimal: near
    ; Set virtual serial port in hex mode
    LDX #$03
	STX $df94
    ; Send value to virtual serial port
    STA $df93
    RTS
.endproc

.proc  _consoleLogChar: near
    ; Set virtual serial port in ascii mode
    LDX #$01
    STX $df94
    ; Send value to virtual serial port
    STA $df93
    RTS
.endproc

.proc  _consoleLogStr: near
    ; Get data pointer from procedure args
    STA DATA_POINTER
    STX DATA_POINTER+1
    ; Set virtual serial port in ascii mode
    LDA #$01
    STA $df94
    ; Iterator
    LDY #$00
    loop:
    LDA (DATA_POINTER),Y
    BEQ end
    STA $df93
    INY
    BNE loop
    end:
    RTS
.endproc

.proc _startStopwatch: near
    LDA #$FB
    STA $DF94
    RTS
.endproc

.proc _stopStopwatch: near
    LDA #$FC
    STA $DF94
    RTS
.endproc
