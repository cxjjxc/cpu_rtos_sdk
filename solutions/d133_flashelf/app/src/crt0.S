.globl __bkpt_label
.section ".text"
.align 2
.type __bkpt_label, %function
__bkpt_label:
    ebreak
    ret
__continue_label:
	mv	 	a0, a0
	ret