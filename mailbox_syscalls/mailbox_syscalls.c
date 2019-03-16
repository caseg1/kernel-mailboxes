#include <stdlib.h>
#include <linux/kernel.h>
//#include <linux/syscalls.h>
#include "list.h"


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
int create_mbox(unsigned long id, int enable_crypt) {
  printf("create_mbox\n");
  
  struct list_head *pos;

  //check if ID exists
  list_for_each(pos, &mboxes) {
    mbox_t* p = NULL;
    p = list_entry(pos, mbox_t, list_node);

    if (p != NULL) {
      if (p->id == id) {
	printf("mbox %d already exists\n", id);
	return -1; //id already exists
      }
    }
  }

  //create new mailbox
  mbox_t* new_mbox = (mbox_t*)malloc(sizeof(mbox_t));
  new_mbox->id = id;
  new_mbox->encrypt = enable_crypt;
  list_add_tail(&new_mbox->list_node, &mboxes);

  printf("Created mbox %d\n", new_mbox->id);
  
  return 0;
}
/* removes mailbox with ID id, if it is empty, and returns 0.
   If the mailbox is not empty, this system call shall return
   an appropriate error and not remove the mailbox.
 */
//SYSCALL_DEFINE1(remove_mbox_421, unsigned long, id) {}
int remove_mbox(unsigned long id) {
  printf("remove_mbox\n");
  
  struct list_head* pos = NULL;
  struct list_head* tmp;

  list_for_each_safe(pos, tmp, &mboxes) {
    mbox_t* p = list_entry(pos, mbox_t, list_node);

    if (p->id == id) {
      if (!list_empty(&p->msgs)) { //check if empty
	printf("Deleted mbox %d\n", p->id);
	list_del(pos);
	//kfree(p);
	free(p);
	return 0;
      }
      else {
	printf("mbox %d not empty", id);
	return -1;
      }
    }
  }
  printf("mbox %d does not exist\n", id);
  return -1;
}

/* adds the process with PID proc_id to the access control list for the mailbox with ID id and returns 0. If the process is already in the ACL for the specified mailbox, this system call shall return an appropriate error.
 */
//SYSCALL_DEFINE2(mbox_add_acl_421, unsigned long, id, int, proc_id) {}

/* removes the process with PID proc_id from the access control list for the mailbox with ID id and returns 0. If the process is not in the ACL for the specified mailbox, this system call shall return an appropriate error.
 */
//SYSCALL_DEFINE3(mbox_del_acl_421, unsigned long, id, int, proc_id) {}

/* returns the number of existing mailboxes.
 */
//SYSCALL_DEFINE4(count_mbox_421, void) {
unsigned int count_mbox(void) {
  printf("count_mbox\n");
  
  struct list_head *pos;
  unsigned int count = 0;

  list_for_each(pos, &mboxes) {
    mbox_t* p = NULL;
    p = list_entry(pos, mbox_t, list_node);

    if (p != NULL) {
      count++;
    }
  }
  return count;
}

/* returns a list of up to k mailbox IDs that the calling process
   can access in the user-space variable mbxes. It returns the number
   of IDs written successfully to mbxes on success and an appropriate
   error code on failure.
*/
//SYSCALL_DEFINE5(list_mbox_421, unsigned long __user *, mbxes, long, k) {
int list_mbox(unsigned long * mbxes, long k) {
  printf("list_mbox\n");
  
  struct list_head *pos;
  int count = 0; //num of successfully written IDs

  list_for_each(pos, &mboxes) {
    mbox_t* p = NULL;
    p = list_entry(pos, mbox_t, list_node);
    if (p != NULL && count < k) {
      printf("Mailbox %d\n", p->id);
      mbxes[count] = p->id;
      count++;
    }
  }
  return count;
}
/* encrypts the message msg (if appropriate), adding it to the already existing mailbox identified. Returns the number of bytes stored (which shall be equal to the message length n) on success, and an appropriate error code on failure. Messages with negative lengths shall be rejected as invalid and cause an appropriate error to be returned, however messages with a length of zero shall be accepted as valid.
 */
//SYSCALL_DEFINE6(send_msg_421, unsigned long, id, unsigned char __user *, msg, long, n, uint32_t __user *, key) {}

/* copies up to n characters from the next message in the mailbox id to the user-space buffer msg, decrypting with the specified key (if appropriate), and removes the entire message from the mailbox (even if only part of the message is copied out). Returns the number of bytes successfully copied (which shall be the minimum of the length of the message that is stored and n) on success or an appropriate error code on failure. 
 */
//SYSCALL_DEFINE7(recv_msg_421, unsigned long, id, unsigned char __user *, msg, long, n, uint32_t __user *, key) {}

/* performs the same operation as recv_msg_421() without removing the message from the mailbox.
 */
//SYSCALL_DEFINE8(peek_msg_421, unsigned long, id, unsigned char __user *, msg, long, n, uint32_t __user *, key) {}

/* returns the number of messages in the mailbox id on success or an appropriate error code on failure.
 */
//SYSCALL_DEFINE9(count_msg_421, unsigned long, id) {

/* returns the length of the next message that would be returned by calling recv_msg_421() with the same id value (that is the number of bytes in the next message in the mailbox). If there are no messages in the mailbox, this shall return an appropriate error value.
 */
//SYSCALL_DEFINE10(len_msg_421, unsigned long, id) {}


int main() {

  long k = 3;
  unsigned long a[k];
  unsigned long* mbxes = a;

  
  create_mbox(66,44);
  create_mbox(66,22);
  printf("num of mboxes: %u\n", count_mbox());
  create_mbox(24,22);
  create_mbox(324,22);
  create_mbox(676,22);
  create_mbox(21,22);
  printf("num of mboxes: %u\n", count_mbox());
  list_mbox(mbxes, k);
  //SYSCALL_DEFINE6()
  remove_mbox(66);
  remove_mbox(24);
  remove_mbox(22);
  printf("num of mboxes: %u\n", count_mbox());
  
  return 0;
}
