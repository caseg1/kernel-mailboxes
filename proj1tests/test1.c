/*
  File: test1.c
  Description: Tests create/remove mbox and count mbox
 */
#include </usr/src/project1/mailbox_syscalls/mailbox_syscalls.h>

int main(void) {

  int e = 99; //encrypt value (not tested)

  printf("Count mboxes before creating any; should count 0\n");
  printf("mbox count: %u\n", count_mbox());
  
  printf("Create 5 mboxes\n");
  create_mbox(3,e);
  create_mbox(6,e);
  create_mbox(1,e);
  create_mbox(9,e);
  create_mbox(7,e);

  printf("Try create duplicate mbox\n");
  create_mbox(7,e);
  
  printf("Remove one mbox\n");
  remove_mbox(6);

  printf("Try removing mbox that never existed\n");
  remove_mbox(5);
  
  printf("Try removing mbox that was removed\n");
  remove_mbox(6);

  printf("Should count 4 mboxes\n");
  printf("mbox count: %u\n", count_mbox());

  printf("Remove 1 mbox and count again; should count 3\n");
  remove_mbox(3);
  printf("mbox count: %u\n", count_mbox());

  printf("Remove 3 mboxes and count again; should count 0\n");
  remove_mbox(7);
  remove_mbox(9);
  remove_mbox(1);
  printf("mbox count: %u\n", count_mbox());

  
  return 0;
}
