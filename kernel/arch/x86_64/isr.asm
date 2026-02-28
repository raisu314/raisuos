; ==============================================================================
; RaisuOS x86_64 ISR & IRQ Stubs
; ==============================================================================
; Handles pushing 64-bit registers, ensuring 16-byte stack alignment
; required by the x64 System V ABI, and calling C interrupt handlers.
; ==============================================================================

extern isr_handler
extern irq_handler

global idt_flush
idt_flush:
    ; rdi contains pointer to idt_ptr
    lidt [rdi]
    ret

; ==============================================================================
; I/O Port Helper
; ==============================================================================
global io_wait
io_wait:
    ; Small delay by writing to unused port
    mov al, 0
    out 0x80, al
    ret

; ==============================================================================
; Port I/O
; ==============================================================================
global inb
inb:
    mov dx, di
    in al, dx
    ret

global outb
outb:
    mov dx, di
    mov al, sil
    out dx, al
    ret

global inw
inw:
    mov dx, di
    in ax, dx
    ret

global outw
outw:
    mov dx, di
    mov ax, si
    out dx, ax
    ret

global inl
inl:
    mov dx, di
    in eax, dx
    ret

global outl
outl:
    mov dx, di
    mov eax, esi
    out dx, eax
    ret

global enable_interrupts
enable_interrupts:
    sti
    ret

global disable_interrupts
disable_interrupts:
    cli
    ret

; ==============================================================================
; Exception Stubs (0-31)
; ==============================================================================

%macro ISR_NOERRCODE 1
global isr%1
isr%1:
    cli
    push 0          ; Dummy error code
    push %1         ; Interrupt number
    jmp isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
global isr%1
isr%1:
    cli
    ; Error code is already pushed by CPU
    push %1         ; Interrupt number
    jmp isr_common_stub
%endmacro

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE   8
ISR_NOERRCODE 9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12
ISR_ERRCODE   13
ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_ERRCODE   17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_ERRCODE   30
ISR_NOERRCODE 31

; ==============================================================================
; IRQ Stubs (32-47)
; ==============================================================================

%macro IRQ 2
global irq%1
irq%1:
    cli
    push 0          ; Dummy error code
    push %2         ; Interrupt number
    jmp irq_common_stub
%endmacro

IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47

; ==============================================================================
; Common Stubs
; ==============================================================================

isr_common_stub:
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    mov rdi, rsp    ; Pass registers_t* as first arg (RDI)

    ; 16-byte stack alignment
    mov rax, rsp
    and rax, 15
    jz .aligned
    sub rsp, 8
    call isr_handler
    add rsp, 8
    jmp .done

.aligned:
    call isr_handler

.done:
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
    add rsp, 16     ; Cleanup error code and int number
    iretq

irq_common_stub:
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    mov rdi, rsp    ; Pass registers_t* as first arg (RDI)

    ; 16-byte stack alignment
    mov rax, rsp
    and rax, 15
    jz .aligned
    sub rsp, 8
    call irq_handler
    add rsp, 8
    jmp .done

.aligned:
    call irq_handler

.done:
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
    add rsp, 16     ; Cleanup error code and int number
    iretq
