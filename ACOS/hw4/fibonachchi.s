.data  					#put in the data segment
	num: .quad 10 
	buffer: 
        .rept 20 
            .ascii " "
        .endr


.text 					# put in the code segment
	.globl fib_rec
	.type fib_rec, @function

fib_rec:
# Function argument is passed in RAX, and the value returned by RCX 
	cmpq $2, %rax
	ja f				# if n>1 go to flag f
	movq $1, %rcx
	ret
f:      pushq %rax  			#save rax
	decq %rax   			#RAX = n-1
	call fib_rec
	pushq %rcx   			#save RCX=F(n-1)
	decq %rax   			#RAX = n-2
	call fib_rec    		#RCX = F(n-2)
	popq %rax   			#RAX = F(n-2)
	addq %rax, %rcx    		#RCX = F(n)=F(n-2)+F(n-1)
	popq %rax    			#restore RAX
	ret


.text
	.globl fib_it
	.type fib_it, @function

fib_it:
	
l:	addq %rcx, %rdx 		#d=d+c
	movq %rbx, %rcx			#c=b
	movq %rdx, %rbx			#b=d
	decq %rax
	cmpq $2, %rax
	ja l
	ret

.text
	.globl number2str
	.type number2str, @function

number2str:
    	movb $0x0a, (%rbx)
    	decq %rbx
write_loop:
    	movq $0, %rdx
   	movq $10, %rcx
    	divq %rcx
    	addq $0x30, %rdx
    	movb %dl, (%rbx)
    	decq %rbx
    	cmpq $0, %rax
    	ja   write_loop
    	ret
.text
	.globl main
	.type main, @function

main:
    	#fib_rec(n)
	movq num, %rax
	call fib_rec
	
	# number2str(number, buffer + 20)
   	movq %rcx, %rax
    	movq $buffer, %rbx
    	addq $19, %rbx
    	call number2str

	#write(1, message, msgend - message);
	movq $4, %rax			#number of syscall 'write' = 4
	movq $1, %rbx			#number of file descriptor 'stdout' = 1
	movq $buffer, %rcx
	movq $20, %rdx			#string length
	int $0x80 			#call interruption
	


	#############
	
	movq num, %rax
	movq $1, %rbx
	movq $1, %rcx
	movq $1, %rdx
	call fib_it

	# number2str(number, buffer + 20)
        movq %rbx, %rax
        movq $buffer, %rbx
        addq $19, %rbx
        call number2str

        #write(1, message, msgend - message);
        movq $4, %rax                   #number of syscall 'write' = 4
        movq $1, %rbx                   #number of file descriptor 'stdout' = 1
        movq $buffer, %rcx
        movq $20, %rdx                  #string length
        int $0x80                       #call interruption

	#printf(printf_format, %rcx)
	#pushq %rcx
	#pushq $printf_format
	#call printf
	#addq $16, %rsp			#align stack   addl $8, %esp
	#movq %rbp, %rsp
	#popq %rbp

	#exit(0);
	movq $1, %rax     		#numder of syscall 'exit' = 1
	movq $0, %rbx			#pass 0 as a parameter
	int $0x80			#call interruption

	



	
