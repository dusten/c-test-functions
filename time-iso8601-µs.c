#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

void get_time(char cBuffer[])
{
  time_t zaman;
  struct tm *ltime;
  static struct timeval _t;
  static struct timezone tz;

  time(&zaman);
  ltime = (struct tm *) localtime(&zaman);
  gettimeofday(&_t, &tz);

  strftime(cBuffer,40,"%Y%m%dT%H:%M:%S",ltime);
  sprintf(cBuffer, "%s.%d", cBuffer,(int)_t.tv_usec);
}

int main(int argc, char *argv[]) 
{
    char cBuffer[25];

    get_time(cBuffer);

    printf(" %s \n",cBuffer);

    return 0;
}
