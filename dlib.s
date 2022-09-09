.export _setVideoMode, _fillScreen, _waitVSync, _startStopwatch, _stopStopwatch

.zeropage
VMEM_POINTER: .res 2, $00


.segment "CODE"

.proc _setVideoMode: near
    STA $df80
    RTS
.endproc

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
