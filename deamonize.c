/*  gcc deamonize.c `pkg-config --cflags --libs glib-2.0` -o deamonize  */

#include <ctype.h>
#include <errno.h>
#include <glib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

#define CONFIG_DIR "/tmp/test/conf"
#define LOG_DIR "/tmp/test/logs"
#define PID_FILE "/tmp/test/pid"
#define GROUP_NAME "test"

static void daemonize();
static void exit_daemon();

int main(int argc, char** argv)
{
  bool debug;
  int input_arg, loop_count;

  opterr = 0;

  while((input_arg = getopt(argc, argv, "D")) != -1)
  {
    switch(input_arg)
    {
      case 'D':
        debug = TRUE;
        break;
      case '?':
        printf("Unrecognized option '%c'.\n", optopt);
        return 1;
      default:
        abort();
    }
  }

  printf("This is my application...\n");

  if(debug)
  {
    printf("I'm in the foreground, so you'll still see my printfs\n");
  }
  else
  {
    printf("This is the last message you'll see, since I'm going to call daemon()\n");
    daemonize();
    atexit(exit_daemon);
  }

  for(loop_count = 0; loop_count < 30; loop_count++)
  {
    printf("Loop count is %d\n", loop_count);
    sleep(2);
  }

  return 0;
}


static void daemonize()
{
   FILE *fh;
   int fd;

   openlog(GROUP_NAME, LOG_PID|LOG_CONS, LOG_DAEMON);
   syslog(LOG_NOTICE, "%s starting up", GROUP_NAME);
   if (daemon(0, 0) == -1)
   {
      printf("Cannot fork into the background\n");
      exit(EXIT_FAILURE);
   }
   fh = fopen(PID_FILE, "w");
   if (!fh)
   {
     printf("Couldn't open PID file \"%s\" for writing: %s.",PID_FILE, strerror(errno));
     exit(EXIT_FAILURE);
   }
   fprintf(fh, "%i", getpid());
   fclose(fh);

   for( fd=sysconf(_SC_OPEN_MAX); fd>0; --fd )
   {
     close(fd);
   }

   umask(027);
   chdir(LOG_DIR);
}

static void exit_daemon()
{
  openlog(GROUP_NAME, LOG_PID|LOG_CONS, LOG_DAEMON);
  syslog(LOG_NOTICE, "%s exit handler removing PID file.", GROUP_NAME);

  unlink(PID_FILE);
}
