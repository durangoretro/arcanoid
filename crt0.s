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
; [HiRes Invert S1 S0    RGB LED NC NC]
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
    
    ; Clean video mode
    LDA #$30
    STA VIDEO_MODE

    ; Clean up RAM
    LDA #$00
    LDX #$00
    STX $01
    LDY #$02
    STY $00
    loopcm:
    STA ($00), Y
    INY
    BNE loopcm
	INC $01
    BPL loopcm

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
    LDA #$3C
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
_stop:
    STP


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
    PHA
    PHX
    
    LDA VIDEO_MODE
    AND #$30
    CMP #$30
    BEQ case_0
    CMP #$00
    BEQ case_1
    CMP #$10
    BEQ case_2
    CMP #$20
    BEQ case_3
    
    case_0:
    LDX #$88
    BRA case_end
    case_1:
    LDX #$98
    BRA case_end
    case_2:
    LDX #$A8
    BRA case_end
    case_3:
    LDX #$3C
    BRA case_end
    
    case_end:
    STX VIDEO_MODE
    
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

; ---------------------------------------------------------------------------
; SEGMENT METADATA
; ---------------------------------------------------------------------------
.segment "METADATA"
.byt $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byt $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byt $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byt $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byt $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byt $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
.byt "BUILD:["
.byt $00,$00,$00,$00,$00,$00,$00
.byt "]$"
.byt "SIGNATURE:["
.byt $00,$00
.byt "]$$"

; ---------------------------------------------------------------------------
; SEGMENT HEADER
; ---------------------------------------------------------------------------
.segment "HEADER"
.byt "DURANGO CA65v0.1"
.byt "                "
.byt "                "
