.global save_context
save_context:
        movl 4(%esp), %eax
        movl %esp, 4(%eax)
        movl %ebx, 8(%eax)
        movl %ecx, 12(%eax)
        movl %edx, 16(%eax)
        movl %esi, 20(%eax)
        movl %edi, 24(%eax)
        movl %ebp, 28(%eax)
        popl 0(%eax)
        pushl 0(%eax)
        ret

.global context_switch
context_switch:
        movl 8(%esp), %eax              // prev_context pointer to %eax
        popl 0(%eax)                    // %return addr to prev_context->eip
        movl %esp, 4(%eax)              // esp to prev_context->esp
        movl %ebx, 8(%eax)              // ditto
        movl %ecx, 12(%eax)             // ..
        movl %edx, 16(%eax)
        movl %esi, 20(%eax)
        movl %edi, 24(%eax)
        movl %ebp, 28(%eax)

        movl 0(%esp), %eax              // next_context pointer to %eax. why not 12(%esp)?  beacuse already run popl at line 4
        movl 8(%esp), %ebx              // next task's page directory addr to %ebx

        movl %ebx, %cr3                 // change page directory
        movl %cr0, %ebx
        orl $0x80000000, %ebx
        movl %ebx, %cr0

        movl 28(%eax), %ebp
        movl 24(%eax), %edi
        movl 20(%eax), %esi
        movl 16(%eax), %edx
        movl 12(%eax), %ecx
        movl 8(%eax), %ebx
        movl 4(%eax), %esp
        pushl 0(%eax)

        movl $0x19910611, %eax            // magic number
        ret
