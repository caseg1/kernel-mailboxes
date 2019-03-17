#include <stdlib.h>
#include <linux/kernel.h>
//#include <linux/syscalls.h>
#include <stdint.h>
//#include <linux/cred.h>
#include <unistd.h>
#include <errno.h>
#include "list.h"


typedef enum {false, true} bool;

typedef struct msgNode {
  unsigned char * msg;
  struct list_head list_node;
} msgNode_t;

typedef struct aclNode {
  pid_t pid;
  struct list_head list_node;
} aclNode_t;

typedef struct mbox {
  unsigned long id;
  int encrypt;
  // link mbox together in the mboxes list
  struct list_head list_node;
  // Each mbox can have their own list of msgs
  struct list_head msgs;
  // Each mbox can have their own acl
  struct list_head acl;
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

  //check if the user is root 
  //if (current_cred()->uid.val != 0) 
  /*if (getpid() != 0)
    return -EPERM;
  */
  
  struct list_head *pos;

  //check if ID exists
  list_for_each(pos, &mboxes) {
    mbox_t* p = NULL;
    p = list_entry(pos, mbox_t, list_node);

    if (p != NULL) {
      if (p->id == id) {
	printf("mbox %lu already exists\n", id);
	return EEXIST; //id already exists
      }
    }
  }

  //create new mailbox
  mbox_t* new_mbox = (mbox_t*)malloc(sizeof(mbox_t));
  INIT_LIST_HEAD(&new_mbox->msgs);
  INIT_LIST_HEAD(&new_mbox->acl);
  new_mbox->id = id;
  new_mbox->encrypt = enable_crypt;  
  list_add_tail(&new_mbox->list_node, &mboxes);
  printf("Created mbox %lu\n", new_mbox->id);  
  return 0;
}
/* removes mailbox with ID id, if it is empty, and returns 0.
   If the mailbox is not empty, this system call shall return
   an appropriate error and not remove the mailbox.
 */
//SYSCALL_DEFINE1(remove_mbox_421, unsigned long, id) {}
int remove_mbox(unsigned long id) {
  printf("remove_mbox\n");

  //check if the user is root 
  //if (current_cred()->uid.val != 0) 
  /*if (getpid() != 0)
    return -EPERM;
  */
  
  struct list_head* pos = NULL;
  struct list_head* tmp;

  list_for_each_safe(pos, tmp, &mboxes) {
    mbox_t* m = list_entry(pos, mbox_t, list_node);
    
    if (m->id == id) { //found mbox
      // CAUSES MEM ERRORS; NOT INIT UNTIL FIRST SND_MSG/ADD_ACL
      if (list_empty(&m->msgs)) { //true if empty
	printf("empty msgs\n");
	
	if (!list_empty(&m->acl)) { //true if ACL not empty
	  printf("non-empty acl\n");
	  //delete ACL
	  aclNode_t* the_pid;
	  aclNode_t* temp_pid;
	  
	  list_for_each_entry_safe(the_pid, temp_pid, &m->acl, list_node) {
	    printf("pid %i removed\n",the_pid->pid);
	    list_del(&the_pid->list_node);
	    //kfree(the_pid);
	    free(the_pid);
	  }
	  printf("Deleted ACL for mbox %lu\n", m->id);
	}
	
	printf("Deleted mbox %lu\n", m->id);
	list_del(pos);
	//kfree(p);
	free(m);
	return 0;
      }
      else {
	printf("mbox %lu not empty\n", id);
	return EISDIR;
      }
    }
  }
  printf("mbox %lu does not exist\n", id);
  return ENOENT;
}

/* adds the process with PID proc_id to the access control list
   for the mailbox with ID id and returns 0. If the process is
   already in the ACL for the specified mailbox, this system call
   shall return an appropriate error.
 */
//SYSCALL_DEFINE2(mbox_add_acl_421, unsigned long, id, int, proc_id) {}
int mbox_add_acl(unsigned long id, int proc_id) {
  printf("mbox_add_acl\n");
  
  //check if the user is root 
  //if (current_cred()->uid.val != 0) 
  /*if (getpid() != 0)
    return -EPERM;
  */
  
  struct list_head *pos;
  list_for_each(pos, &mboxes) { //loop mailboxes
    mbox_t* m = NULL;
    m = list_entry(pos, mbox_t, list_node);

    //search for mailbox id
    if (m->id == id) {

      if (list_empty(&m->acl)) { //check if ACL empty
	
	aclNode_t* the_pid;
	list_for_each_entry(the_pid, &m->acl, list_node) {
	
	  if (the_pid->pid == proc_id) { //error if ACL entry exists
	    printf("PID %i already exists in ACL\n", proc_id);
	    return EEXIST;
	  }
	}
      }
      //add ACL entry
      aclNode_t* p = (aclNode_t*)malloc(sizeof(aclNode_t));
      p->pid = proc_id;
      list_add_tail(&p->list_node, &m->acl);
      printf("Added PID %i to ACL of mbox %lu\n", proc_id, m->id);
      return 0;
    }
  }  
  printf("Couldn't find mailbox with %lu ID\n", id);
  return ENOENT;
}

/* removes the process with PID proc_id from the access control
   list for the mailbox with ID id and returns 0. If the process
   is not in the ACL for the specified mailbox, this system call
   shall return an appropriate error.
 */
//SYSCALL_DEFINE3(mbox_del_acl_421, unsigned long, id, int, proc_id) {}
int mbox_del_acl(unsigned long id, int proc_id) {
  printf("mbox_del_acl\n");
  
  //check if the user is root 
  //if (current_cred()->uid.val != 0) 
  /*if (getpid() != 0)
    return -EPERM;
  */
  
  struct list_head *pos;
  list_for_each(pos, &mboxes) { //loop mailboxes
    mbox_t* m = NULL;
    m = list_entry(pos, mbox_t, list_node);

    //search for mailbox id
    if (m->id == id) {

      //loop ACL
      aclNode_t* the_pid;
      list_for_each_entry(the_pid, &m->acl, list_node) {

	if (the_pid->pid == proc_id) { //found PID, delete
	  printf("Deleted PID %i to ACL of mbox %lu\n", proc_id, m->id);
	  list_del(&the_pid->list_node);
	  free(the_pid);
	  //kfree(the_pid);
	  return 0;
	}
      }
      //couldn't find ACL entry
      printf("PID %i not in ACL\n", proc_id);
      return ESRCH;
    }
  }  
  printf("Couldn't find mailbox with %lu ID\n", id);
  return ENOENT;  
}

/* returns the number of existing mailboxes.
 */
//SYSCALL_DEFINE4(count_mbox_421, void) {
unsigned int count_mbox(void) {
  printf("count_mbox\n");

  //check if the user is root 
  /*if (current_cred()->uid.val == 0) 
   for true, skip ACL check*/
  
  struct list_head *pos;
  unsigned int count = 0;

  list_for_each(pos, &mboxes) {
    mbox_t* p = NULL;
    p = list_entry(pos, mbox_t, list_node);

    if (p != NULL)
      count++;
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

  //check if the user is root 
  /*if (current_cred()->uid.val == 0) 
   for true, skip ACL check*/

  //check if passed in pointer is valid
  if (mbxes == NULL)
    return EFAULT;
  
  struct list_head *pos;
  int count = 0; //num of successfully written IDs

  list_for_each(pos, &mboxes) {
    mbox_t* p = NULL;
    p = list_entry(pos, mbox_t, list_node);
    if (p != NULL && count < k) {
      printf("Mailbox %lu\n", p->id);
      mbxes[count] = p->id;
      count++;
    }
  }
  return count;
}
/* encrypts the message msg (if appropriate), adding it to the
   already existing mailbox identified. Returns the number of bytes
   stored (which shall be equal to the message length n) on success,
   and an appropriate error code on failure. Messages with negative
   lengths shall be rejected as invalid and cause an appropriate error
   to be returned, however messages with a length of zero shall be
   accepted as valid.
 */
//SYSCALL_DEFINE6(send_msg_421, unsigned long, id, unsigned char __user *, msg, long, n, uint32_t __user *, key) {}
long send_msg(unsigned long id, unsigned char * msg, long n, uint32_t * key) {

  if (msg == NULL) //check passed in pointer
    return EFAULT;
  if (n < 0) //check msg length n for negative
    return EIO;
  
  struct list_head *pos;
  list_for_each(pos, &mboxes) { //loop mailboxes
    mbox_t* m = NULL;
    m = list_entry(pos, mbox_t, list_node);

    //search for mailbox id
    if (m->id == id) {

      //not root, check ACL
      //if (current_cred()->uid.val != 0) {
      if (getpid() != 0) {
	aclNode_t* the_pid;
	list_for_each_entry(the_pid, &m->acl, list_node) { //loop ACL
	  //if (the_pid->pid == current->pid) { //found PID in ACL
	  if (the_pid->pid == getpid()) {
	    goto addMail; //sorry
	  }
	}
	//printf("PID %i not in ACL\n", current->pid);
	return EPERM;
      }
      
    addMail:
      //encrypts if != 0
      if (m->encrypt != 0) {}

      //initialize empty msg list
      if (!list_empty(&m->msgs))  //check if ACL empty
	INIT_LIST_HEAD(&m->msgs);
	
      //add to mailbox
      msgNode_t * s = (msgNode_t*)malloc(sizeof(msgNode_t));
      s->msg = msg;
      list_add_tail(&s->list_node, &m->msgs);
      return n;
      //return number of bytes stored (equal to n) on success
      //error code on failure
    }
  }
  printf("Couldn't find mailbox with %lu ID\n", id);
  return ENOENT;  
}

/* copies up to n characters from the next message in the mailbox id
   to the user-space buffer msg, decrypting with the specified key
   (if appropriate), and removes the entire message from the mailbox
   (even if only part of the message is copied out). Returns the number
   of bytes successfully copied (which shall be the minimum of the length
   of the message that is stored and n) on success or an appropriate error
   code on failure. 
 */
//SYSCALL_DEFINE7(recv_msg_421, unsigned long, id, unsigned char __user *, msg, long, n, uint32_t __user *, key) {}
int recv_msg(unsigned long id, unsigned char * msg, long n, uint32_t * key) {
  printf("recv_msg");
  //check if the user is root 
  /*if (current_cred()->uid.val == 0) 
    for true, skip ACL check
  */
  
  //loop mboxes
  struct list_head *pos;
  list_for_each(pos, &mboxes) { //find mbox id
    mbox_t* m = NULL;
    m = list_entry(pos, mbox_t, list_node);    

    if (m->id == id) { // find first message  
      if (!list_empty(&m->msgs)) { //check if empty
	printf("No msg in ID %lu\n", id);
	return ENOENT; //return error if no messages
      }
      //find first msg
      //copy n bytes from head to user * msg
      //decrypt if necessary
      //remove message from mbox
      //return min(n, len(msg @ id))
    }
  }
  printf("mbox %lu does not exist\n", id);
  return ENOENT;
}

/* performs the same operation as recv_msg_421() without
   removing the message from the mailbox.
 */
//SYSCALL_DEFINE8(peek_msg_421, unsigned long, id, unsigned char __user *, msg, long, n, uint32_t __user *, key) {}
int peek_msg(unsigned long id, unsigned char * msg, long n, uint32_t * key) {
  printf("peek_msg");

  //check if the user is root 
  /*if (current_cred()->uid.val == 0) 
    for true, skip ACL check
  */
  
  //loop mboxes
  struct list_head *pos;
  list_for_each(pos, &mboxes) { //find mbox id
    mbox_t* m = NULL;
    m = list_entry(pos, mbox_t, list_node);    

    if (m->id == id) { // find first message  
      if (!list_empty(&m->msgs)) { //check if empty
	printf("No msg in ID %lu\n", id);
	return ENOENT; //return error if no messages
      }
      //find first msg
      //copy n bytes from head to user * msg
      //decrypt if necessary
      //return min(n, len(msg @ id))
    }
  }
  printf("mbox %lu does not exist\n", id);
  return ENOENT;
}

/* returns the number of messages in the mailbox id on
   success or an appropriate error code on failure.
 */
//SYSCALL_DEFINE9(count_msg_421, unsigned long, id) {}
unsigned int count_msg(unsigned long id) {
  printf("count_msg\n");
  
  unsigned int count = 0;
  
  //loop mboxes
  struct list_head *pos;
  list_for_each(pos, &mboxes) {
    mbox_t* m = NULL;
    m = list_entry(pos, mbox_t, list_node);    

    if (m->id == id) {    
      
      //loop msgs
      msgNode_t* the_msg;
      list_for_each_entry(the_msg, &m->msgs, list_node) {
	count++;
      }
      return count;
    }
  }
  printf("mbox %lu does not exist\n", id);
  return ENOENT;
}

/* returns the length of the next message that would be returned
   by calling recv_msg_421() with the same id value (that is the
   number of bytes in the next message in the mailbox). If there
   are no messages in the mailbox, this shall return an appropriate
   error value.
 */
//SYSCALL_DEFINE10(len_msg_421, unsigned long, id) {}
int len_msg(unsigned long id) {
  printf("len_msg");
  
  //loop mboxes
  struct list_head *pos;
  list_for_each(pos, &mboxes) { //find mbox id
    mbox_t* m = NULL;
    m = list_entry(pos, mbox_t, list_node);    

    if (m->id == id) { // find first message  
      if (!list_empty(&m->msgs)) { //check if empty
	printf("No msg in ID %lu\n", id);
	return ENOENT; //return error if no messages
      }
      //find first msg
      //return bytes
    }
  }
  printf("mbox %lu does not exist\n", id);
  return ENOENT;
}


int main(void) {

  long k = 3;
  unsigned long a[k];
  unsigned long* mbxes = a;

  create_mbox(66,44);
  mbox_add_acl(66,5);
  mbox_add_acl(66,2);
  remove_mbox(66);
  /*create_mbox(66,22);
  printf("num of mboxes: %u\n", count_mbox());
  create_mbox(24,22);
  create_mbox(324,22);
  create_mbox(676,22);
  create_mbox(21,22);
  mbox_del_acl(66,5);
  printf("num of mboxes: %u\n", count_mbox());
  list_mbox(mbxes, k);
  remove_mbox(66);
  remove_mbox(24);
  remove_mbox(22);
  printf("num of mboxes: %u\n", count_mbox());
  remove_mbox(324);
  remove_mbox(676);
  remove_mbox(324);
  remove_mbox(21);
  */  
  return 0;
}
