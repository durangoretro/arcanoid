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
; DURANGO HARDWARE CONSTANTS
; ---------------------------------------------------------------------------
VIDEO_MODE = $df80
INT_ENABLE = $DFA0
GAMEPAD1 = $df9c
GAMEPAD2 = $df9d

; ---------------------------------------------------------------------------
; DURANGO CRT0 CONSTANTS
; ---------------------------------------------------------------------------
GAMEPAD_MODE1 = $00
GAMEPAD_MODE2 = $01
GAMEPAD_VALUE1 = $02
GAMEPAD_VALUE2 = $03

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
    STA VIDEO_MODE

    ; Enable Durango interrupts
    LDA #$01
    STA INT_ENABLE
    CLI

    ; Init gamepads
    STA GAMEPAD1
    LDX #8
    loop:
    STA GAMEPAD2
    DEX
    BNE loop
    LDA GAMEPAD1
    LDX GAMEPAD2
    STA GAMEPAD_MODE1
    STX GAMEPAD_MODE2
    
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
    STA GAMEPAD1
    LDX #8
    loop2:
    STA GAMEPAD2
    DEX
    BNE loop2
    LDA GAMEPAD1
    EOR GAMEPAD_MODE1
    STA GAMEPAD_VALUE1
    LDA GAMEPAD2
    EOR GAMEPAD_MODE2
    STA GAMEPAD_VALUE2
    ; Restore registers and return
    PLX
    PLA
    RTI 

; Non-maskable interrupt (NMI) service routine
_nmi_int:
    ; Return from all NMI interrupts
    RTI

; ---------------------------------------------------------------------------
; SEGMENT VECTTORS
; ---------------------------------------------------------------------------

.segment  "VECTORS"

.addr      _nmi_int    ; NMI vector
.addr      _init       ; Reset vector
.addr      _irq_int    ; IRQ/BRK vector


