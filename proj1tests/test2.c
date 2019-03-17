/*
  File: test2.c
  Description: Tests list_mbox
 */
#include </usr/src/project1/mailbox_syscalls/mailbox_syscalls.h>

int main(void) {

  int e = 99; //encrypt value (not tested)

  unsigned long arr[5]; //passed to list
  unsigned long* a = arr; 
    
  printf("Try to list 5 mboxes; should return 0\n");
  printf("%u mboxes returned\n", list_mbox(a,5));
  
  printf("Create 5 mboxes\n");
  create_mbox(8,e);
  create_mbox(5,e);
  create_mbox(9,e);
  create_mbox(1,e);
  create_mbox(4,e);
  
  //list mboxes
  printf("Try to list 1 mbox; should return 1\n");
  printf("%u mboxes returned\n", list_mbox(a,1));
  
  printf("Try to list 4 mboxes; should return 4\n");
  printf("%u mboxes returned\n", list_mbox(a,4));

  printf("Try to list 10 mboxes; should error code 14\n");
  printf("Error code: %u\n", list_mbox(a,10));

  remove_mbox(4);
  remove_mbox(1);
  remove_mbox(9);
  remove_mbox(5);
  remove_mbox(8);
  
  return 0;
}
