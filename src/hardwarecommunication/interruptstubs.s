.set IRQ_BASE, 0x20

.section .text

.extern handle_interrupt
.global ignore_interrupt_request

# defines how to handle exceptions
.macro handle_exception num
.global handle_exception_\num\()
handle_exception_\num\():
    movb $\num, (interrupt_number)
    jmp int_bottom
.endm

# defines how to handle interrupt requests
.macro handle_interrupt_request num
.global handle_interrupt_request_\num\()
handle_interrupt_request_\num\():
    movb $\num + IRQ_BASE, (interrupt_number)
    jmp int_bottom
.endm

# initialize interrupts
handle_interrupt_request 0x00
handle_interrupt_request 0x01

int_bottom:
    # values could be overwritten, so store
    pusha
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs
    
    pushl %esp
    push (interrupt_number)
    call handle_interrupt
    # addl $5, %esp
    movl %eax, %esp

    # restore register values
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa

ignore_interrupt_request:

    iret

.data
    interrupt_number: .byte 0

