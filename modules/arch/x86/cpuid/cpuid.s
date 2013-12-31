.code32

.global cpuid_check_cpuid
cpuid_check_cpuid:
    pushf
    popl %eax
    movl %eax, %ecx
    xorl $0x200000, %eax
    pushl %eax
    popf
    pushf
    popl %eax
    xorl %ecx, %eax
    shrl $21, %eax
    andl $1, %eax
    ret

.global cpuid_get_data
cpuid_get_data:
    pushl %ebp
    movl %esp, %ebp
    pusha
    movl 8(%ebp), %eax
    movl 12(%ebp), %edi
    cpuid
    movl %eax, (%edi)
    movl %ebx, 4(%edi)
    movl %ecx, 8(%edi)
    movl %edx, 12(%edi)
    popa
    popl %ebp
    ret

.global cpuid_get_vendor
cpuid_get_vendor:
    pushl %ebp
    movl %esp, %ebp
    pusha
    movl $0x00, %eax
    movl 8(%ebp), %edi
    cpuid
    movl %ebx, (%edi)
    movl %edx, 4(%edi)
    movl %ecx, 8(%edi)
    popa
    popl %ebp
    ret

.global cpuid_get_brand
cpuid_get_brand:
    pushl %ebp
    movl %esp, %ebp
    pusha
    movl $0x80000002, %eax
    movl 8(%ebp), %edi
    cpuid
    movl %eax, (%edi)
    movl %ebx, 4(%edi)
    movl %ecx, 8(%edi)
    movl %edx, 12(%edi)
    movl $0x80000003, %eax
    cpuid
    movl %eax, 16(%edi)
    movl %ebx, 20(%edi)
    movl %ecx, 24(%edi)
    movl %edx, 28(%edi)
    movl $0x80000004, %eax
    cpuid
    movl %eax, 32(%edi)
    movl %ebx, 36(%edi)
    movl %ecx, 40(%edi)
    movl %edx, 44(%edi)
    popa
    popl %ebp
    ret

