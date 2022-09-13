; ---------------------------------------------------------------------------
; crt0.s
; ---------------------------------------------------------------------------
;
; Startup code for cc65 (Single Board Computer version)

.import   _main

.export   __STARTUP__ : absolute = 1        ; Mark as startup
.import __STACKSTART__, __STACKSIZE__
.import    copydata, zerobss, initlib, donelib

.include  "zeropage.inc"

; Enable 65C02 instruction set
.PC02


; ---------------------------------------------------------------------------
; SEGMENT STARTUP
; ---------------------------------------------------------------------------
.segment  "STARTUP"


; Initialize Durango X
_init:
    ; Disable interrupts
    SEI

    ; Initialize stack pointer to $01FF
    LDX #$FF
    TXS

    ; Clear decimal mode
    CLD
   
    ; Initialize cc65 stack pointer
    LDA #<(__STACKSTART__ + __STACKSIZE__)
    STA sp
    LDA #>(__STACKSTART__ + __STACKSIZE__)
    STA sp+1

    ; Initialize memory storage
    JSR zerobss
    JSR copydata
    JSR initlib

    ; Initialize Durango Video
    LDA #$3c
    STA $df80

    ; Enable Durango interrupts
    LDA #$01
    STA $DFA0
    CLI

    ; Init gamepads
    STA $df9c
    LDX #8
    loop:
    STA $df9d
    DEX
    BNE loop
    LDA $df9c
    LDX $df9d
    STA $00
    STX $01
    
    ; Call main()
    JSR _main

; Back from main (also the _exit entry):
_exit:
    ; Run destructors
    JSR donelib

; Stop
.proc _stop: near
    STP
.endproc

; Non-maskable interrupt (NMI) service routine
_nmi_int:
    ; Return from all NMI interrupts
    RTI

; Maskable interrupt (IRQ) service routine
_irq_int:  
    ; Save registres and filter BRK
    PHA
    PHX
    TSX
    LDA $103,X
    AND #$10
    BNE _stop
    ; Read controllers
    STA $df9c
    LDX #8
    loop2:
    STA $df9d
    DEX
    BNE loop2
    LDA $df9c
    EOR $00
    STA $02
    LDA $df9d
    EOR $01
    STA $03       
    ; Restore registers and return
    PLX
    PLA
    RTI 

; ---------------------------------------------------------------------------
; SEGMENT VECTTORS
; ---------------------------------------------------------------------------

.segment  "VECTORS"

.addr      _nmi_int    ; NMI vector
.addr      _init       ; Reset vector
.addr      _irq_int    ; IRQ/BRK vector


