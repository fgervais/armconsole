#include "syscalls.h"
#include <sys/types.h>
#include <errno.h>

register char * stack_ptr asm ("sp");

int _kill (int pid, int sig)
{
	return -1;
}

void _exit (int status)
{
  /* There is only one SWI for both _exit and _kill. For _exit, call
	the SWI with the second argument set to -1, an invalid value for
	signum, so that the SWI handler can distinguish the two calls.
	Note: The RDI implementation of _kill throws away both its
	arguments.  */
	_kill(status, -1);
}
 
caddr_t _sbrk (int incr)
{
	extern char   end asm ("end");	/* Defined by the linker.  */
	static char * heap_end;
	char *        prev_heap_end;

	if (heap_end == NULL)
		heap_end = & end;
  
	prev_heap_end = heap_end;
  
	if (heap_end + incr > stack_ptr)
	{
      /* Some of the libstdc++-v3 tests rely upon detecting
		out of memory errors, so do not abort here.  */
#if 0
		extern void abort (void);

		_write (1, "_sbrk: Heap and stack collision\n", 32);
      
						   abort ();
#else
						   errno = ENOMEM;
				   return (caddr_t) -1;
#endif
	}
  
	heap_end += incr;

	return (caddr_t) prev_heap_end;
}

int _getpid (int n)
{
	return 1;
}

int _write (int file, char * ptr, int len)
{
	return -1;
}

int _close (int file)
{
	return -1;
}

int _fstat (int file, struct stat * st)
{
	return -1;
}

int _isatty (int fd)
{
	return (fd <= 2) ? 1 : 0;  /* one of stdin, stdout, stderr */
}

int _lseek (int file, int ptr, int dir)
{
	return -1;
}

int _read (int file, char * ptr, int len)
{
	return -1;
}
