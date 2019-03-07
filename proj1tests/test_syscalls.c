#include <linux/kernel.h>
#include <linux/syscalls.h>

//DATA STRUCTURE FOR ALL MAILBOXES
//bool encrypt = true;

/*creates a new empty mailbox with ID id, if it does not already exist, 
and returns 0. The queue shall be flagged for encryption if the enable_crypt 
option is set to anything other than 0. If enable_crypt is set to zero, then 
the key parameter in any functions including it shall be ignored 
(they may be NULL).*/
SYSCALL_DEFINE0(create_mbox_421, unsigned long, id, int, enable_crypt) {

  //check for unused id
  mbx * tmp = mbx_head;
  for (mbxes->head; mbxes->next != NULL; tmp = mbxes->next) { 
    if (mbx->id == id)
      return -1;
  }

  //make mailbox
  struct mbx {
    unsigned long id = id;
    struct * msgs;
    
    if (enable_crypt == 0)
      encrypt = false;
    else
      encrypt = true;
  }
  
  return 0;
}

/*removes mailbox with ID id, if it is empty, and returns 0. 
If the mailbox is not empty, this system call shall return an appropriate 
error and not remove the mailbox.*/
SYSCALL_DEFINE1(remove_mbox_421, unsigned long, id) {}
//
SYSCALL_DEFINE2(mbox_add_acl_421, unsigned long, id, int, proc_id) {}
//
SYSCALL_DEFINE3(mbox_del_acl_421, unsigned long, id, int, proc_id) {}
//
SYSCALL_DEFINE4(count_mbox_421, void) {}
/*returns a list of up to k mailbox IDs that the calling process can access in the user-space variable mbxes. It returns the number of IDs written successfully to mbxes on success and an appropriate error code on failure.*/
SYSCALL_DEFINE5(list_mbox_421, unsigned long __user *, mbxes, long, k) {}
//
SYSCALL_DEFINE6(send_msg_421, unsigned long, id, unsigned char __user *, msg, long, n, uint32_t __user *, key) {}
//
SYSCALL_DEFINE7(recv_msg_421, unsigned long, id, unsigned char __user *, msg, long, n, uint32_t __user *, key) {}
//
SYSCALL_DEFINE8(peek_msg_421, unsigned long, id, unsigned char __user *, msg, long, n, uint32_t __user *, key) {}
//
SYSCALL_DEFINE9(count_msg_421, unsigned long, id) {}
//
SYSCALL_DEFINE10(len_msg_421, unsigned long, id) {}
