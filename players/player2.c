#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <my-ipc.h>
#include <client-side.h>
#include <redundant.h>
#include <public.h>

// -- variables --
const char myName[] = "9445663156";
const char deployment[] = "Ba3a4a5a6 Cc1c2c3 Cc5c6c7 De1e2 De4e5 De7e8 Sg1 Sg3 Sg5 Sg7 ";

enum ship {
  UNKNOWN,
  ROCK,
  NOSHIP,
  BSHIP,
  CSHIP,
  DSHIP,
  SSHIP
};

int cur_x,cur_y;
int step;
int remain_b,remain_c,remain_d,remain_s;
char cur_shot;
enum ship enemy_board[BD_SIZE][BD_SIZE];    // BD_SIZE is 9 (defined in public.h)
bool possible_b[BD_SIZE][BD_SIZE];
bool possible_c[BD_SIZE][BD_SIZE];
bool possible_d[BD_SIZE][BD_SIZE];
bool possible_s[BD_SIZE][BD_SIZE];
int dx[4] = {-1,1,-1,1},dy[4] = {-1,-1,1,1};
int dx2[4] = {0,1,0,-1},dy2[4] = {-1,0,1,0};
int first_x[4] = {4,8,4,0},first_y[4] = {0,4,8,4};
int second_x[4] = {6,8,2,0},second_y[4] = {0,6,8,2};
int b_x[4] = {-3,0,3,0},b_y[4] = {0,-3,0,3};
int c_x[4] = {-2,0,2,0},c_y[4] = {0,-2,0,2};
// -- variables --

// -- prototypes --
void first_step(int *x,int *y);
void second_step(int *x,int *y);
void final_step(int *x,int *y);

// update field
void updateS(int x,int y);
void rmArround(int x,int y);
void decide_next();
// -- prototypes --

// -- check --
bool check(int x,int y){
  return (0<=x&&x<BD_SIZE&&0<=y&&y<BD_SIZE);
}

// -- check --

// -- init functions --
void init_var(){
  remain_b = 1,remain_c = 2,remain_d = 3,remain_s = 4;
  step = 0;
}
void init_possibility(){
  int ix,iy;
  for(ix = 0;ix < (BD_SIZE); ix++){
    for(iy = 0;iy < (BD_SIZE); iy++){
      possible_b[ix][iy]=possible_c[ix][iy]=possible_d[ix][iy]=possible_s[ix][iy]=true;
    }
  }
  possible_b[0][0] = possible_b[0][0] = possible_b[0][0] = possible_b[0][0] = false;
  possible_b[0][1] = possible_b[0][1] = possible_b[0][1] = possible_b[0][1] = false;
  possible_b[1][0] = possible_b[1][0] = possible_b[1][0] = possible_b[1][0] = false;

  possible_b[7][0] = possible_b[7][0] = possible_b[7][0] = possible_b[7][0] = false;
  possible_b[8][0] = possible_b[8][0] = possible_b[8][0] = possible_b[8][0] = false;
  possible_b[8][1] = possible_b[8][1] = possible_b[8][1] = possible_b[8][1] = false;

  possible_b[0][7] = possible_b[0][7] = possible_b[0][7] = possible_b[0][7] = false;
  possible_b[0][8] = possible_b[0][8] = possible_b[0][8] = possible_b[0][8] = false;
  possible_b[1][8] = possible_b[1][8] = possible_b[1][8] = possible_b[1][8] = false;

  possible_b[7][8] = possible_b[7][8] = possible_b[7][8] = possible_b[7][8] = false;
  possible_b[8][7] = possible_b[8][7] = possible_b[8][7] = possible_b[8][7] = false;
  possible_b[8][8] = possible_b[8][8] = possible_b[8][8] = possible_b[8][8] = false;
}


void init_board(void)
{
  int ix, iy;

  for(ix = 0; ix < (BD_SIZE); ix++)
  {
    for(iy = 0; iy < (BD_SIZE); iy++)
    {
      //======kokokara======

      enemy_board[ix][iy] = UNKNOWN;
      //======kokomade======
    }
  }

  //rock is out of bound

  enemy_board[0][0] = ROCK;
  enemy_board[0][1] = ROCK;
  enemy_board[1][0] = ROCK;

  enemy_board[7][0] = ROCK;
  enemy_board[8][0] = ROCK;
  enemy_board[8][1] = ROCK;

  enemy_board[0][7] = ROCK;
  enemy_board[0][8] = ROCK;
  enemy_board[1][8] = ROCK;

  enemy_board[7][8] = ROCK;
  enemy_board[8][7] = ROCK;
  enemy_board[8][8] = ROCK;
}
// -- init functions --


// -- response to server--
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
  int x, y;

  while (TRUE)
  {
    if(step < 4){
      first_step(&x,&y);
    } else if(step < 8){

    } else {

    }
    x = rand() % BD_SIZE;
    y = rand() % BD_SIZE;
    //=====kokokara=====
    if(enemy_board[x][y] == 0)break;

    //=====kokomade=====
  }
  printf("[%s] shooting at %d%d ... ", myName, x, y);
  sprintf(shot_string, "%d%d", x, y);
  send_to_ref(shot_string);
  cur_x = x;
  cur_y = y;
}

// -- response to server--

void record_result(int x,int y,char line[])
{
  cur_shot = line[13];
  if(line[13]=='B')
  {
    //====kokokara====
    enemy_board[cur_x][cur_y] = BSHIP;
    remain_b--;
    rmArround(cur_x,cur_y);
    //====kokomade====
  }
  else if(line[13]=='C')
  {
    //====kokokara====
    enemy_board[cur_x][cur_y] = CSHIP;
    remain_c--;
    rmArround(cur_x,cur_y);
    //====kokomade====
  }
  else if(line[13]=='D')
  {
    //====kokokara====
    enemy_board[cur_x][cur_y] = DSHIP;
    remain_d--;
    rmArround(cur_x,cur_y);
    //====kokomade====
  }
  else if(line[13]=='S')
  {
    //====kokokara====
    enemy_board[cur_x][cur_y] = SSHIP;
    remain_s--;
    rmArround(cur_x,cur_y);
    updateS(cur_x,cur_y);
    // Sの場合でも次に移る
    step++;
    //====kokomade====
  }
  else if(line[13]=='R')
  {
    //====kokokara====
    enemy_board[cur_x][cur_y] = ROCK;
    //====kokomade====
  }
  else
  {
    //====kokokara====
    enemy_board[cur_x][cur_y] = NOSHIP;
    // 存在しなかった場合は次に移る
    step++;
    //====kokomade====
  }
}

void print_board(void){
  int ix, iy;

  for (iy = BD_SIZE - 1; iy >= 0; iy--)
  {
    printf("%2d ", iy);
    for (ix = 0; ix < BD_SIZE; ix++)
    {
      switch(enemy_board[ix][iy])
      {
        case UNKNOWN:
          printf("U ");
          break;
        case NOSHIP:
          printf("N ");
          break;
        case ROCK:
          printf("R ");
          break;
        case BSHIP:
          printf("B ");
          break;
        case CSHIP:
          printf("C ");
          break;
        case DSHIP:
          printf("D ");
          break;
        case SSHIP:
          printf("S ");
          break;
        default:
          break;
      }
    }
    printf("\n");
  }

  printf("  ");
  for (ix = 0; ix < BD_SIZE; ix++)
  {
    printf("%2d", ix);
  }
  printf("\n\n");
}

void handle_messages(void)
{
  char line[MSG_LEN];

  srand(getpid());
  init_board();

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
      record_result(cur_x,cur_y,line);
      printf("[%s] result: %c\n", myName, line[13]);
      print_board();
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

// -- step 1 --
void first_step(int *x,int *y){
  switch(cur_shot){
    case 'B':
      first_b(x,y);
      break;
    case 'C':
      break;
    case 'D':
      break;
    default:
      break;
  }
}

void fist_b(int *x, iny *y){
  int i;
  for(i=0;i<4;i++){
    int center = (step + 3) % 4;
    int right  = (step + 2) % 4;
    int px = first_x[step],py = first_y[step];
    if(possible_b[px+b_x[center]][py+b_y[center]]){
      *x=px+b_x[center],*y=py+b_y[center];
    } else {
      *x=px+b_x[right], *y=py+b_y[center];
    }
  }
}
// -- step 1 --

// -- step 2 --
void second_step(int *x,int *y){

}
// -- step 2 --

// -- step 3 --
void final_step(int *x,int *y){

}
// -- step 3 --

// -- update --
void setFalse(int x,int y){
  if(!(enemy_board[x][y]==ROCK)){
    enemy_board[x][y] = NOSHIP;
    possible_b[x][y]  = false;
    possible_c[x][y]  = false;
    possible_d[x][y]  = false;
    possible_s[x][y]  = false;
  }
}
void updateS(int x,int y){
  int i,nx,ny;
  for(i=0;i<4;i++){
    nx = x + dx2[i],ny = y + dy2[i];
    if(check(nx,ny))setFalse(nx,ny);
  }
}

void rmArround(int x,int y){
  int i,nx,ny;
  for(i=0;i<4;i++){
    nx = x+dx[i],ny=y+dy[i];
    if(check(nx,ny))setFalse(nx,ny);
  }
}
// -- update --

// -- main --
int main()
{
  init_possibility();
  init_var();
  client_make_connection();
  handle_messages();
  client_close_connection();
  return 0;
}
