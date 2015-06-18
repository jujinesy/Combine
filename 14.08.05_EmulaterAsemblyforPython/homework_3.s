mov	$1000, %esp
push	%ebp
mov	%esp, %ebp
mov	$0, %eax
mov	$0x0, %ebx
xor	%esi, %esi
xor	%edi, %edi
xor	%ecx, %ecx
mov	%ecx, %edx
mov     $0x100, %eax
mov     %eax, %ebx
sub     $0x10, %ebx
sub     $10, %ebx
mov	%ebx, %edx
push	%edx
add	%edx, %edx
push	%edx
pop	%ebx
xor	$0x10, %ebx
mov	$0x200, %edx
sub	$0x20, %edx
add	%edx, %edx
mov	$10, %eax
sub	%eax, %edx		
push	%edx
pop	%ecx
nop
pop	%ebp
ret