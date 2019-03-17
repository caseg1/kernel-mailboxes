/*
  File: test3.c
  Description: Tests add/del acl
 */
#include </usr/src/project1/mailbox_syscalls/mailbox_syscalls.h>

int main(void) {

  int e = 99; //encrypt value (not tested)
  
  printf("Try add acl to mbox that doesn't exist; should error\n");
  mbox_add_acl(2,7);
  
  printf("Try del acl to mbox that doesn't exist; should error\n");
  mbox_del_acl(6,7);
  
  printf("Create 5 mboxes\n");
  create_mbox(8,e);
  create_mbox(5,e);
  create_mbox(9,e);
  create_mbox(1,e);
  create_mbox(4,e);
  
  printf("Add PID 2 to mbox ID 8's ACL\n");
  mbox_add_acl(8,2);

  printf("Try add duplicate PID to mbox 8; should error\n");
  mbox_add_acl(8,2);

  printf("Add PID 10,11,12 to ACL of mbox 5\n");
  mbox_add_acl(5,10);
  mbox_add_acl(5,11);
  mbox_add_acl(5,12);
  
  printf("Delete PID 12 from ACL of mbox 5\n");
  mbox_del_acl(5,12);
  
  printf("Try to again deleted PID 12 from ACL of mbox 5; should error\n");
  mbox_del_acl(5,12);

  printf("Remove mbox 5 while its ACL isn't empty; should clear ACL\n");
  remove_mbox(5);
  
  remove_mbox(4);
  remove_mbox(1);
  remove_mbox(9);
  remove_mbox(8);
  
  return 0;
}
