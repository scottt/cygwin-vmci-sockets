/* cygthread.h

   Copyright 1998, 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2010
   Red Hat, Inc.

This software is a copyrighted work licensed under the terms of the
Cygwin license.  Please consult the file "CYGWIN_LICENSE" for
details. */

#ifndef _CYGTHREAD_H
#define _CYGTHREAD_H

typedef void WINAPI (*LPVOID_THREAD_START_ROUTINE) (void *) __attribute__((noreturn));		// Input queue thread

class cygthread
{
  LONG inuse;
  DWORD id;
  HANDLE h;
  HANDLE ev;
  HANDLE thread_sync;
  void *stack_ptr;
  const char *__name;
#ifdef DEBUGGING
  const char *__oldname;
  bool terminated;
#endif
  LPTHREAD_START_ROUTINE func;
  unsigned arglen;
  VOID *arg;
  bool is_freerange;
  static bool exiting;
  HANDLE notify_detached;
  bool standalone;
  void create () __attribute__ ((regparm(2)));
 public:
  bool terminate_thread ();
  static DWORD WINAPI stub (VOID *);
  static DWORD WINAPI simplestub (VOID *);
  static DWORD main_thread_id;
  static const char * name (DWORD = 0);
  void callfunc (bool) __attribute__ ((noinline, regparm (2)));
  void auto_release () {func = NULL;}
  void release (bool);
  cygthread (LPTHREAD_START_ROUTINE start, unsigned n, LPVOID param, const char *name, HANDLE notify = NULL)
  : __name (name), func (start), arglen (n), arg (param),
  notify_detached (notify), standalone (false)
  {
    create ();
  }
  cygthread (LPVOID_THREAD_START_ROUTINE start, LPVOID param, const char *name, HANDLE notify = NULL)
  : __name (name), func ((LPTHREAD_START_ROUTINE) start), arglen (0),
    arg (param), notify_detached (notify), standalone (true)
  {
    create ();
    /* This is a neverending/high-priority thread */
    ::SetThreadPriority (h, THREAD_PRIORITY_HIGHEST);
    zap_h ();
  }
  cygthread (LPTHREAD_START_ROUTINE start, LPVOID param, const char *name, HANDLE notify = NULL)
  : __name (name), func (start), arglen (0), arg (param),
  notify_detached (notify), standalone (false)
  {
    create ();
  }
  cygthread (LPVOID_THREAD_START_ROUTINE start, unsigned n, LPVOID param, const char *name, HANDLE notify = NULL)
  : __name (name), func ((LPTHREAD_START_ROUTINE) start), arglen (n),
    arg (param), notify_detached (notify), standalone (true)
  {
    create ();
    /* This is a neverending/high-priority thread */
    ::SetThreadPriority (h, THREAD_PRIORITY_HIGHEST);
    zap_h ();
  }
  cygthread () {};
  static void init ();
  bool detach (HANDLE = NULL);
  operator HANDLE ();
  void * operator new (size_t);
  static cygthread *freerange ();
  static void terminate ();
  HANDLE thread_handle () const {return h;}
  bool SetThreadPriority (int nPriority) {return ::SetThreadPriority (h, nPriority);}
  void zap_h ()
  {
    CloseHandle (h);
    h = NULL;
  }
};

#define cygself NULL
#endif /*_CYGTHREAD_H*/
