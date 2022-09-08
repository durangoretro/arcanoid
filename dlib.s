.export _setVideoMode
.export _fillScreen

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


