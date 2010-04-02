#include <sys/types.h>
#include <errno.h>

#ifndef __SYSCALLS_H
#define __SYSCALLS_H

#ifdef __cplusplus
 extern "C" {
#endif

int _kill (int pid, int sig);
void _exit (int status);
caddr_t _sbrk (int incr);
int _getpid (int n);
int _write (int file, char * ptr, int len);
int _close (int file);
int _fstat (int file, struct stat * st);
int _isatty (int fd);
int _lseek (int file, int ptr, int dir);
int _read (int file, char * ptr, int len);
 
#ifdef __cplusplus
 }
#endif
 
#endif  // __SYSCALLS_H
