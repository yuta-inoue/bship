#include <stdio.h>
#include <stdlib.h>
#include <my-ipc.h>
#include <client-side.h>
#include <redundant.h>
#include <public.h>
#include <stdbool.h>
// 定数定義
#define X_SIZE 9
#define Y_SIZE 9
const char myName[] = "9445663156";
//const char deployment[] = "Ba3a4a5a6 Cc1c2c3 Cc5c6c7 De1e2 De4e5 De7e8 Sg1 Sg3 Sg5 Sg7 ";
const char deployment[] = "Bg2g3g4g5 Cc0c1c2 Ci2i3i4 Da2a3 De0f0 Dd8e8 Sa5 Sb7 Sg8 Si6 ";
int dx[4]={-1,1,-1,1},dy[4]={-1,-1,1,1};
int dx_b[4] = {};
// プロトタイプ宣言
void init();
bool check(int i,int j);
bool isValid(int i,int j);
void handle_resp(char c);
void updateS(int i,int j);
void setDiag(int x,int y);
void update();
// decide
void decideShot();
void decideB();
void decideC();
void decideD();
void decide();


// FIELD VALUE
// 0->not checked
// 1->not hit
// 2->B
// 3->C
// 4->D
// 5->S
// 6->will hit
unsigned char* field;
// 現在位置
int x,y;
bool isB,isC,isD;

// 残りの隻数
int b,c,d,s;


// 初期化メソッド
void init(){
  // 変数初期化
  x = y = 0;
  b = 1,c = 2,d = 3,s = 4;
  // 配列の初期化
  int i,j;
  for(i=0;i<X_SIZE;i++)
    for(j=0;j<Y_SIZE;j++)
      field[j*X_SIZE+i]=0;
  field[0]=field[1]=field[7]=field[8]=1;
  field[9]=field[17]=field[63]=field[71];
  field[72]=field[73]=field[79]=field[80]=1;
}

// 未探索判定
bool check(int i,int j){
  if(field[j*X_SIZE+i]==0){
    return true;
  } else {
    return false;
  }
}

// i,jが座標として有効か判定する
bool isValid(int i,int j){
  return (0<=i&&i<X_SIZE&&0<=j&&j<Y_SIZE);
}

// -- update --

void handle_resp(char c){
  bool flag=true;
  isB=isC=isD=false;
  switch(c){
    case 'B':
      field[y*X_SIZE+x]=2;
      isB=true;
      break;
    case 'C':
      field[y*X_SIZE+x]=3;
      isC=true;
      break;
    case 'D':
      field[y*X_SIZE+x]=4;
      isD=true;
      break;
    case 'S':
      field[y*X_SIZE+x]=5;
      updateS(x,y);
      break;
    default:
      field[y*X_SIZE+x]=1;
      flag = false;
      break;
  }
  if(flag){
    setDiag(x,y);
  }
}
// 結果がSの時の更新
void updateS(int i,int j){
  int dx,dy,nx,ny;
  for(dx=-1;dx<=1;dx++){
    for(dy=-1;dy<=1;dy++){
      nx = i+dx,ny = j+dy;
      if(isValid(nx,ny)&&(!(dx==0&&dy==0)))field[ny*X_SIZE+nx]=1;
    }
  }
}

void setDiag(int x,int y){
  int i;
  for(i=0;i<4;i++){
    int nx=x+dx[i],ny=y+dy[i];
    if(isValid(nx,ny))field[ny*X_SIZE+nx]=1;
  }
}
void update(){

}
// -- update --

// -- decide shot --
void decideShot(){
  if(isB){
    decideB();
  } else if(isC){
    decideC();
  } else if(isD){
    decideD();
  } else {
    decide();
  }
}

// 前回の結果がBの時のショットの決定
void decideB(){

}
// 前回の結果がCの時のショットの決定
void decideC(){

}
// 前回の結果がDの時のショットの決定
void decideD(){

}

void decide(){

}
// -- decide shot --
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
	x = n_x,y=n_y;
	return;
      }
    }
  }
}

void handle_messages(void)
{
  char line[MSG_LEN];

  srand(getpid());

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
