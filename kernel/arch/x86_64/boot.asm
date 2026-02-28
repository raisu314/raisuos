; ==============================================================================
; RaisuOS x86_64 Bootstrap
; ==============================================================================
; 1. Defines Multiboot2 header to be loaded by GRUB.
; 2. Enters 32-bit protected mode.
; 3. Sets up 4-level paging (identity mapping first 2MB for kernel).
; 4. Transitions into 64-bit Long Mode.
; 5. Jumps to kernel_main passing Multiboot info.
; ==============================================================================

global _start
extern kernel_main

section .multiboot
align 8
header_start:
    dd 0xE85250D6                ; Multiboot2 magic number
    dd 0                         ; Architecture 0 (protected mode i386)
    dd header_end - header_start ; Header length
    ; Checksum
    dd 0x100000000 - (0xE85250D6 + 0 + (header_end - header_start))

    ; Framebuffer tag (Type 5)
    align 8
    dw 5                         ; Type: Framebuffer
    dw 0                         ; Flags
    dd 20                        ; Size
    dd 1024                      ; Width
    dd 768                       ; Height
    dd 32                        ; Depth
    dd 0                         ; Reserved

    ; End tag
    align 8
    dw 0
    dw 0
    dd 8
header_end:

section .bss
align 4096
p4_table:
    resb 4096
p3_table:
    resb 4096
p2_table:
    resb 4096 * 4 ; 4 tables to map 4GB
stack_bottom:
    resb 16384 ; 16 KB stack
stack_top:

section .rodata
gdt64:
    dq 0 ; zero entry
.code: equ $ - gdt64
    ; CS: Exec/Read, Long Mode (L=1, D=0)
    dq (1<<43) | (1<<44) | (1<<47) | (1<<53)
.data: equ $ - gdt64
    ; DS: Read/Write
    dq (1<<41) | (1<<44) | (1<<47)
gdt64_pointer:
    dw $ - gdt64 - 1
    dq gdt64

section .text
bits 32
_start:
    mov esp, stack_top
    mov edi, eax    ; Multiboot magic
    mov esi, ebx    ; Multiboot info pointer

    ; Set up 4-level paging
    ; p4[0] -> p3
    mov eax, p3_table
    or eax, 0b11 ; present + writable
    mov [p4_table], eax

    ; Map 4GB in p3
    ; p3[0..3] -> p2_table[0..3]
    mov eax, p2_table
    or eax, 0b11
    mov [p3_table], eax
    
    add eax, 4096
    mov [p3_table + 8], eax
    
    add eax, 4096
    mov [p3_table + 16], eax
    
    add eax, 4096
    mov [p3_table + 24], eax

    ; Identity map 4GB using 2MB pages (2048 entries)
    mov ecx, 0
.map_p2:
    mov eax, 0x200000
    mul ecx ; edx:eax = 2MB * ecx
    or eax, 0b10000011 ; present + writable + huge
    mov [p2_table + ecx * 8], eax
    mov [p2_table + ecx * 8 + 4], edx ; Write upper 32 bits
    inc ecx
    cmp ecx, 2048
    jne .map_p2

    ; Enable PAE
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; Load CR3
    mov eax, p4_table
    mov cr3, eax

    ; Enable Long Mode inside EFER MSR
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; Enable Paging
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ; Load 64-bit GDT
    lgdt [gdt64_pointer]

    ; Jump to 64-bit
    jmp gdt64.code:long_mode_start

bits 64
long_mode_start:
    mov ax, gdt64.data
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    xor rbp, rbp
    call kernel_main

.hlt:
    cli
    hlt
    jmp .hlt
