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

.zeropage
GAMEPADS_DATA: .res 4, $00

; ---------------------------------------------------------------------------
; Place the startup code in a special segment

.segment  "STARTUP"
; Force 65C02 assembly mode
.PC02

; ---------------------------------------------------------------------------
; A little light 6502 housekeeping

_init:
; ---------------------------------------------------------------------------  
; Disable hardware periodic interrupt
          SEI                          ; Disable interrupts

; ---------------------------------------------------------------------------    
; Initialize 6502 stack
          LDX     #$FF                 ; Initialize stack pointer to $01FF
          TXS
          CLD                          ; Clear decimal mode

;----------------------------------------------------------------------------
; Init gamepads
    STA $df9c
    LDX #8
    loop:
    STA $df9d
    DEX
    BNE loop
    LDA $df9c
    LDX $df9d
    STA GAMEPADS_DATA
    STX GAMEPADS_DATA+1

; ---------------------------------------------------------------------------
; Set cc65 argument stack pointer

          LDA     #<(__STACKSTART__ + __STACKSIZE__)
          STA     sp
          LDA     #>(__STACKSTART__ + __STACKSIZE__)
          STA     sp+1

; ---------------------------------------------------------------------------
; Initialize memory storage

          JSR     zerobss              ; Clear BSS segment
          JSR     copydata             ; Initialize DATA segment
          JSR     initlib              ; Run constructors

; --------------------------------------------------------------------------
; Initialize Durango Video
          LDA #$3c
          STA $df80
; ----------------------------------------------------------------------------
; Call main()

          JSR     _main

; ---------------------------------------------------------------------------
; Back from main (this is also the _exit entry):  force a software break

_exit:    JSR     donelib              ; Run destructors
          BRK

; ---------------------------------------------------------------------------
; Wait for interrupt:  Forces the assembler to emit a WAI opcode ($CB)
; ---------------------------------------------------------------------------
.proc _wait: near

           CLI                    ; Enable interrupts
.byte      $CB                    ; Inserts a WAI opcode
           RTS                    ; Return to caller

.endproc

; ---------------------------------------------------------------------------
; Stop:  Forces the assembler to emit a STP opcode ($DB)
; ---------------------------------------------------------------------------

.proc _stop: near
     CLC
	forever:
	BCC forever
.endproc

; ---------------------------------------------------------------------------
; Non-maskable interrupt (NMI) service routine

_nmi_int:  RTI                    ; Return from all NMI interrupts

; ---------------------------------------------------------------------------
; Maskable interrupt (IRQ) service routine

_irq_int:  PHX                    ; Save X register contents to stack
           TSX                    ; Transfer stack pointer to X
           PHA                    ; Save accumulator contents to stack
           INX                    ; Increment X so it points to the status
           INX                    ;   register value saved on the stack
           LDA $100,X             ; Load status register contents
           AND #$10               ; Isolate B status bit
           BNE break              ; If B = 1, BRK detected
; Actual interrupt code
;----------------------------------------------------------------------------
           ; 1. write into $DF9C
           STA $df9c
           ; 2. write into $DF9D 8 times
           LDX #8
           loop2:
           STA $df9d
           DEX
           BNE loop2
           LDA $df9c
           EOR GAMEPADS_DATA
           STA GAMEPADS_DATA+2
           LDA $df9d
           EOR GAMEPADS_DATA+1
           STA GAMEPADS_DATA+3
; ---------------------------------------------------------------------------
; IRQ detected, return

irq:       PLA                    ; Restore accumulator contents
           PLX                    ; Restore X register contents
           RTI                    ; Return from all IRQ interrupts

; ---------------------------------------------------------------------------
; BRK detected, stop

break:     JMP _stop              ; If BRK is detected, something very bad
                                  ;   has happened, so stop running

; ---------------------------------------------------------------------------
; vectors
; ---------------------------------------------------------------------------

.segment  "VECTORS"

.addr      _nmi_int    ; NMI vector
.addr      _init       ; Reset vector
.addr      _irq_int    ; IRQ/BRK vector


