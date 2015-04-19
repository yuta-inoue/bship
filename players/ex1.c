#include <stdio.h>
#include <stdlib.h>
#include <my-ipc.h>
#include <client-side.h>
#include <redundant.h>
#include <public.h>

const char myName[] = "ex1";
const char deployment[] = "Bg2g3g4g5 Cc0c1c2 Ci2i3i4 Da2a3 De0f0 Dd8e8 Sa5 Sb7 Sg8 Si6 ";

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

  x = 1;
  y = 1;

  printf("[%s] shooting at %d%d ... ", myName, x, y);
  sprintf(shot_string, "%d%d", x, y);
  send_to_ref(shot_string);
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
  client_make_connection();
  handle_messages();
  client_close_connection();
  return 0;
}
