/*
  File: test4.c
  Description: Tests send/recv, peek, and len_msg
 */
#include </usr/src/project1/mailbox_syscalls/mailbox_syscalls.h>

int main(void) {

  int e = 99; //encrypt value (not tested)

  int len1 = 5;
  int len2 = 0;
  int len3 = 10;
  
  unsigned char* s1 = (unsigned char*)malloc(len1 * sizeof(unsigned char));
  unsigned char* s2 = (unsigned char*)malloc(len2 * sizeof(unsigned char));
  unsigned char* s3 = (unsigned char*)malloc(len3 * sizeof(unsigned char));

  unsigned char* r1 = (unsigned char*)malloc(len1 * sizeof(unsigned char));
  unsigned char* r2 = (unsigned char*)malloc(len2 * sizeof(unsigned char));
  unsigned char* r3 = (unsigned char*)malloc(len3 * sizeof(unsigned char));
  unsigned char* r4 = (unsigned char*)malloc(len3 * sizeof(unsigned char));
  unsigned char* r5 = (unsigned char*)malloc(len1 * sizeof(unsigned char));

  unsigned char* p1 = (unsigned char*)malloc(len1 * sizeof(unsigned char));
  unsigned char* p2 = (unsigned char*)malloc(len1-2 * sizeof(unsigned char));
  unsigned char* p3 = (unsigned char*)malloc(len3 * sizeof(unsigned char));
  
  s1[0] = 'h';
  s1[1] = 'e';
  s1[2] = 'l';
  s1[3] = 'l';
  s1[4] = 'o';

  s3[0] = '\0';
  s3[1] = 'b';
  s3[2] = 'e';
  s3[3] = 't';
  s3[4] = 'w';
  s3[5] = 'e';
  s3[6] = 'e';
  s3[7] = 'n';
  s3[8] = 's';
  s3[9] = '\0';
  
  create_mbox(44,e);
  mbox_add_acl(44,getpid());

  //Test send 5, recv 5
  printf("msg in s1, before send (h e l l o): ");
  for (int i=0; i<len1; i++)
    printf("%c ", s1[i]);
  printf("\n");
      
  printf("return of send_msg(5): %ld\n", send_msg(44,s1,len1,0));
  
  printf("return of peek_msg(5): %ld\n", peek_msg(44,p1,len1,0));
  printf("msg in p1, after peek (h e l l o): ");
  for (int i=0; i<len1; i++)
    printf("%c ", p1[i]);
  printf("\n");
  
  printf("return of peek_msg(3): %ld\n", peek_msg(44,p2,len1-2,0));
  printf("msg in p2, after peek (h e l): ");
  for (int i=0; i<len1-2; i++)
    printf("%c ", p2[i]);
  printf("\n");
  
  printf("return of recv_msg(5): %ld\n", recv_msg(44,r1,len1,0));

  printf("msg after recv (h e l l o): ");
  for (int i=0; i<len1; i++)
    printf("%c ", r1[i]);
  printf("\n\n");

  //Test send 0, recv 0
  printf("msg in s2, before send (): ");
  for (int i=0; i<len2; i++)
    printf("%c ", s2[i]);
  printf("\n");
  
  printf("return of send_msg(0): %ld\n", send_msg(44,s2,len2,0));
  printf("return of recv_msg(0): %ld\n\n", recv_msg(44,r2,len2,0));
  
  //Test send 10, recv 5
  printf("msg in s3, before send ( b e t w e e n s ): ");
  for (int i=0; i<len3; i++)
    printf("%c ", s3[i]);
  printf("\n");

  printf("return of send_msg(10): %ld\n", send_msg(44,s3,len3,0));

  printf("return of len_msg(10): %ld\n", len_msg(44));
   
  printf("return of recv_msg(5): %ld\n", recv_msg(44,r3,len1,0));
  
  printf("msg in r3, after recv ( b e t w): ");
  for (int i=0; i<len1; i++) {
    printf("%c ", r3[i]);
  }
  printf("\n\n");
  
  
  //Add 2 msgs; len of first; peek first
  printf("return of send_msg(10): %ld\n", send_msg(44,s3,len3,0));  
  printf("return of send_msg(5): %ld\n", send_msg(44,s1,len1,0));

  printf("return of len_msg(10): %ld\n", len_msg(44));
  
  printf("return of peek_msg(10): %ld\n", peek_msg(44,p3,len3,0));
  printf("msg in p2, after peek ( b e t w e e n s ): ");
  for (int i=0; i<len3; i++)
    printf("%c ", p3[i]);
  printf("\n\n");
  
  //Try to remove mbox with msgs; should fail
  remove_mbox(44);

  //Recv msgs and delete mbox
  printf("return of recv_msg(10): %ld\n", recv_msg(44,r4,len3,0));
  printf("return of recv_msg(5): %ld\n", recv_msg(44,r5,len1,0));
  remove_mbox(44);
  
  free(s1);
  free(s2);
  free(s3);

  free(r1);
  free(r2);
  free(r3);
  free(r4);
  free(r5);

  free(p1);
  free(p2);
  free(p3);
  
  return 0;
}
