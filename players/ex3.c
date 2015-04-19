#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <my-ipc.h>
#include <client-side.h>
#include <redundant.h>
#include <public.h>
#define X_SIZE 9
#define Y_SIZE 9
const char myName[] = "ex3";
const char deployment[] = "Ba3a4a5a6 Cc1c2c3 Cc5c6c7 De1e2 De4e5 De7e8 Sg1 Sg3 Sg5 Sg7 ";

int pre_shot_x, pre_shot_y;
// FIELD VALUE
// 0->not checked
// 1->not hit
// 2->B
// 3->C
// 4->D
// 5->S
// 6->will hit
unsigned char* field;
// 初期化メソッド
void init(){
  pre_shot_x = 0;
  pre_shot_y = 0;
  // 配列の初期化
  int i,j;
  for(i=0;i<X_SIZE;i++)
    for(j=0;j<Y_SIZE;j++)
      field[j*X_SIZE+i]=0;
  field[0]=field[1]=field[7]=field[8]=1;
  field[9]=field[17]=field[63]=field[71];
  field[72]=field[73]=field[79]=field[80]=1;
}

bool check(int i,int j){
  if(field[j*X_SIZE+i]==0){
    return true;
  } else {
    return false;
  }
}

void handle_resp(char c){
  switch(c){
    case 'B':
      field[pre_shot_y*X_SIZE+pre_shot_x]=2;
      break;
    case 'C':
      field[pre_shot_y*X_SIZE+pre_shot_x]=3;
      break;
    case 'D':
      field[pre_shot_y*X_SIZE+pre_shot_x]=4;
      break;
    case 'S':
      field[pre_shot_y*X_SIZE+pre_shot_x]=5;
      break;
    default:
      field[pre_shot_y*X_SIZE+pre_shot_x]=1;
      break;
  }
}

void respond_with_name(void)
{
  char *str = (char *)malloc(sizeof(myName));
  strcpy(str, myName);
  send_to_ref(str);
  free(str);
}

void respond_with_deployment(void)
{
  char *str = (char *)malloc(sizeof(deployment));
  strcpy(str, deployment);
  send_to_ref(str);
  free(str);
}

void respond_with_shot(void)
{
  char shot_string[MSG_LEN];
  int n_x, n_y;
  for(n_y=0;n_y<9;n_y++){
    for(n_x=0;n_x<9;n_x++){
      if(check(n_x,n_y)){
      	printf("[%s] shooting at %d%d ... ",myName, n_x,n_y);
      	sprintf(shot_string, "%d%d", n_x,n_y);
      	send_to_ref(shot_string);
        pre_shot_x = n_x,pre_shot_y=n_y;
      	return;
      }
    }
  }
}

void handle_messages(void)
{
  char line[MSG_LEN];

  pre_shot_x = 0;
  pre_shot_y = 0;

  while (TRUE)
  {
    receive_from_ref(line);

    if(message_has_type(line, "name?"))
    {
      respond_with_name();
    }
    else if(message_has_type(line, "deployment?"))
    {
       respond_with_deployment();
    }
    else if(message_has_type(line, "shot?"))
    {
      respond_with_shot();
    }
    else if(message_has_type(line, "shot-result:"))
    {
      handle_resp(line[13]);
     printf("[%s] result: %c\n", myName, line[13]);
    }
    else if(message_has_type(line, "end:"))
    {
      break;
    }
    else
    {
      printf("[%s] ignoring message: %s", myName, line);
    }
  }
}

int main()
{
  if(NULL == (field=(unsigned char *)malloc(sizeof(unsigned char)*X_SIZE*Y_SIZE))){
    printf("Memory allocation failed.");return(-1);
  }
  init();
  client_make_connection();
  handle_messages();
  client_close_connection();
  free(field);
  return 0;
}
