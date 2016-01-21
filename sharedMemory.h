#ifndef _SHAREDMEMORY_H_
#define _SHAREDMEMORY_H_

#include <sys/ipc.h>
#include <sys/shm.h>

struct SM{
  char LED_Number;
  char LED_Spin;
  char StartButtonPressed;
  char UploadButtonPressed;
}typedef SM;

#define MAXSM sizeof(SM)

#endif
