#include <linux/kernel.h>
#include <linux/syscalls.h>


SYSCALL_DEFINE0(create_mbox_421, unsigned long, id, int, enable_crypt) {}
SYSCALL_DEFINE1(remove_mbox_421, unsigned long, id) {}
SYSCALL_DEFINE2(mbox_add_acl_421, unsigned long, id, int, proc_id) {}
SYSCALL_DEFINE3(mbox_del_acl_421, unsigned long, id, int, proc_id) {}
SYSCALL_DEFINE4(count_mbox_421, void) {}
SYSCALL_DEFINE5(list_mbox_421, unsigned long __user *, mbxes, long, k) {}
SYSCALL_DEFINE6(send_msg_421, unsigned long, id, unsigned char __user *, msg, long, n, uint32_t __user *, key) {}
SYSCALL_DEFINE7(recv_msg_421, unsigned long, id, unsigned char __user *, msg, long, n, uint32_t __user *, key) {}
SYSCALL_DEFINE8(peek_msg_421, unsigned long, id, unsigned char __user *, msg, long, n, uint32_t __user *, key) {}
SYSCALL_DEFINE9(count_msg_421, unsigned long, id) {}
SYSCALL_DEFINE10(len_msg_421, unsigned long, id) {}
