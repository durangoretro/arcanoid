; System procedures
.export _waitVSync
; Draw procedures
.export _fillScreen
; Debug procedures
.export _consoleLogHex
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
    LDX #80
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


; ------ DEBUG PROCEDURES

.proc  _consoleLogHex: near
    ; Set virtual serial port in hex mode
    LDX #$00
	STX $df94
    ; Send value to virtual serial port
    STA $df93
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
