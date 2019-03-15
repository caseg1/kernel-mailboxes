#include <stdlib.h>
#include <linux/kernel.h>
//#include <linux/syscalls.h>
#include "list.h"


/*
typedef struct mbox {
  unsigned long id;
  int encrypt;
  
  //struct list_head* msgs;
  //INIT_LIST_HEAD(msgs);
  struct list_head list_node;
} mbox_t;

typedef struct mbxes {
  struct list_head list_node;
  struct list_head mb
}
*/
  
typedef struct msg {
  int id;
  // This is used to link messages together in their list
  struct list_head list_node;
} msg_t;

typedef struct mbox {
  unsigned long id;
  int encrypt;
  // This is used to link mboxs together in the mbox list
  struct list_head list_node;
  // This is used so each mbox can have their own list of msgs
  struct list_head msgs;
} mbox_t;

LIST_HEAD(mboxes);
  
/*creates a new empty mailbox with ID id, if it does not already exist,
  and returns 0. The queue shall be flagged for encryption if the
  enable_crypt option is set to anything other than 0. If enable_crypt
  is set to zero, then the key parameter in any functions including it
  shall be ignored (they may be NULL).
*/
//SYSCALL_DEFINE0(create_mbox_421, unsigned long, id, int, enable_crypt) {
int SYSCALL_DEFINE0(unsigned long id, int enable_crypt) {

  //check if ID exists
  struct list_head *pos;
  list_for_each(pos, &mboxes) {
    mbox_t* p = NULL;
    p = list_entry(pos, mbox_t, list_node);

    if (p != NULL) {
      if (p->id == id) {
	return -1; //id already exists
      }
    }
  }

  //create new mailbox
  mbox_t* new_mbox = (mbox_t*)malloc(sizeof(mbox_t));
  new_mbox->id = id;
  new_mbox->encrypt = enable_crypt;
  list_add_tail(&new_mbox->list_node, &mboxes);
  
  return 0;
}
/*
SYSCALL_DEFINE1(remove_mbox_421, unsigned long, id) {}
SYSCALL_DEFINE2(mbox_add_acl_421, unsigned long, id, int, proc_id) {}
SYSCALL_DEFINE3(mbox_del_acl_421, unsigned long, id, int, proc_id) {}
SYSCALL_DEFINE4(count_mbox_421, void) {}
*/
/* returns a list of up to k mailbox IDs that the calling process
   can access in the user-space variable mbxes. It returns the number
   of IDs written successfully to mbxes on success and an appropriate
   error code on failure.
*/
//SYSCALL_DEFINE5(list_mbox_421, unsigned long __user *, mbxes, long, k) {
int SYSCALL_DEFINE5(unsigned long * mbxes, long k) {
  
  int count = 0; //num of successfully written IDs
  
}
/*SYSCALL_DEFINE6(send_msg_421, unsigned long, id, unsigned char __user *, msg, long, n, uint32_t __user *, key) {}
SYSCALL_DEFINE7(recv_msg_421, unsigned long, id, unsigned char __user *, msg, long, n, uint32_t __user *, key) {}
SYSCALL_DEFINE8(peek_msg_421, unsigned long, id, unsigned char __user *, msg, long, n, uint32_t __user *, key) {}
SYSCALL_DEFINE9(count_msg_421, unsigned long, id) {}
SYSCALL_DEFINE10(len_msg_421, unsigned long, id) {}
*/

int main() {
  
  SYSCALL_DEFINE0(66,44);
  SYSCALL_DEFINE0(66,22);
  //SYSCALL_DEFINE5(mbxes,1);
  
  return 0;
}
