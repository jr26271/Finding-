/* FUNCTION: pthread_mutex_init */

#ifndef __ESBMC_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __ESBMC_PTHREAD_H_INCLUDED
#endif

inline int pthread_mutex_init(
  pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr)
{
  __ESBMC_HIDE:
  __ESBMC_mutex_lock_field(*mutex)=0;
  __ESBMC_mutex_count_field(*mutex)=0;
  __ESBMC_mutex_owner_field(*mutex)=0;
  return 0;
}

/* FUNCTION: pthread_mutex_lock */

#ifndef __ESBMC_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __ESBMC_PTHREAD_H_INCLUDED
#endif

inline int pthread_mutex_lock(pthread_mutex_t *mutex)
{
  __ESBMC_HIDE:
  static _Bool unlocked = 1;
  static _Bool deadlock_mutex=0;
  extern int trds_in_run, trds_count, count_lock=0;

#ifdef __ESBMC_DEADLOCK_DETECTION
  __ESBMC_yield();
  __ESBMC_atomic_begin();
  unlocked = (__ESBMC_mutex_lock_field(*mutex)==0);

  if (unlocked)
    __ESBMC_mutex_lock_field(*mutex)=1;
  else
    count_lock++;
  __ESBMC_atomic_end();

  if (__ESBMC_mutex_lock_field(*mutex)==0)
	count_lock--;

  if (!unlocked)
  {
	deadlock_mutex = (count_lock == trds_in_run);
	__ESBMC_assert(!deadlock_mutex,"deadlock detected with mutex lock");
    __ESBMC_assume(deadlock_mutex);
  }
#endif

#ifndef __ESBMC_DEADLOCK_DETECTION
  __ESBMC_yield();
  __ESBMC_assume(!__ESBMC_mutex_lock_field(*mutex));
  __ESBMC_atomic_begin();
  __ESBMC_mutex_lock_field(*mutex)=1;
  __ESBMC_atomic_end();
#endif

  return 0; // we never fail
}

/* FUNCTION: pthread_mutex_trylock */

#ifndef __ESBMC_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __ESBMC_PTHREAD_H_INCLUDED
#endif

inline int pthread_mutex_trylock(pthread_mutex_t *mutex)
{
  return 0; // we never fail
}

/* FUNCTION: pthread_mutex_unlock */

#ifndef __ESBMC_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __ESBMC_PTHREAD_H_INCLUDED
#endif

inline int pthread_mutex_unlock(pthread_mutex_t *mutex)
{
  __ESBMC_HIDE:
  __ESBMC_atomic_begin();
#ifndef __ESBMC_DEADLOCK_DETECTION
#ifdef __ESBMC_LOCK_DETECTION
  __ESBMC_assert(__ESBMC_mutex_lock_field(*mutex), "must hold lock upon unlock");
#endif
#endif
  __ESBMC_mutex_lock_field(*mutex)=0;
  __ESBMC_atomic_end();
  return 0; // we never fail
}

/* FUNCTION: pthread_mutex_destroy */

#ifndef __ESBMC_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __ESBMC_PTHREAD_H_INCLUDED
#endif

inline int pthread_mutex_destroy(pthread_mutex_t *mutex)
{ }

/* FUNCTION: pthread_exit */

#ifndef __ESBMC_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __ESBMC_PTHREAD_H_INCLUDED
#endif

void pthread_exit(void *value_ptr)
{ __ESBMC_assume(0); }

/* FUNCTION: pthread_rwlock_destroy */

#ifndef __ESBMC_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __ESBMC_PTHREAD_H_INCLUDED
#endif

inline int pthread_rwlock_destroy(pthread_rwlock_t *lock)
{ }

/* FUNCTION: pthread_rwlock_init */

#ifndef __ESBMC_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __ESBMC_PTHREAD_H_INCLUDED
#endif

inline int pthread_rwlock_init(pthread_rwlock_t *lock,
  const pthread_rwlockattr_t *attr)
{ __ESBMC_HIDE: __ESBMC_rwlock_field(*lock)=0; }

/* FUNCTION: pthread_rwlock_rdlock */

#ifndef __ESBMC_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __ESBMC_PTHREAD_H_INCLUDED
#endif

inline int pthread_rwlock_rdlock(pthread_rwlock_t *lock)
{ /* TODO */ }

/* FUNCTION: pthread_rwlock_tryrdlock */

#ifndef __ESBMC_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __ESBMC_PTHREAD_H_INCLUDED
#endif

inline int pthread_rwlock_tryrdlock(pthread_rwlock_t *lock)
{ /* TODO */ }

/* FUNCTION: pthread_rwlock_trywrlock */

#ifndef __ESBMC_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __ESBMC_PTHREAD_H_INCLUDED
#endif

inline int pthread_rwlock_trywrlock(pthread_rwlock_t *lock)
{ __ESBMC_HIDE:
  __ESBMC_atomic_begin();
  if(__ESBMC_rwlock_field(*lock)) { __ESBMC_atomic_end(); return 1; }
  __ESBMC_rwlock_field(*lock)=1;
  __ESBMC_atomic_end();
  return 0;
}

/* FUNCTION: pthread_rwlock_unlock */

#ifndef __ESBMC_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __ESBMC_PTHREAD_H_INCLUDED
#endif

inline int pthread_rwlock_unlock(pthread_rwlock_t *lock)
{ __ESBMC_HIDE: __ESBMC_rwlock_field(*lock)=0; }

/* FUNCTION: pthread_rwlock_wrlock */

#ifndef __ESBMC_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __ESBMC_PTHREAD_H_INCLUDED
#endif

inline int pthread_rwlock_wrlock(pthread_rwlock_t *lock)
{ __ESBMC_HIDE:
  __ESBMC_atomic_begin();
  __ESBMC_assume(!__ESBMC_rwlock_field(*lock));
  __ESBMC_rwlock_field(*lock)=1;
  __ESBMC_atomic_end();
  return 0; // we never fail
}

/* FUNCTION: pthread_join */

#ifndef __ESBMC_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __ESBMC_PTHREAD_H_INCLUDED
#endif

inline int pthread_join(pthread_t __th, void **__thread_return)
{
#if 1
  extern unsigned int trd_nr, trds_status;
  __ESBMC_assume((trds_status & trd_nr) == 0);

#endif
#if 0
  extern unsigned int trd_nr, exit0, exit1, exit2, exit3, exit4, exit5,
	  				  exit6, exit7, exit8, exit9;

  if (trd_nr==0)      __ESBMC_assume(exit0 == 1);
  else if (trd_nr==1) __ESBMC_assume(exit1 == 1);
  else if (trd_nr==2) __ESBMC_assume(exit2 == 1);
  else if (trd_nr==3) __ESBMC_assume(exit3 == 1);
  else if (trd_nr==4) __ESBMC_assume(exit4 == 1);
  else if (trd_nr==5) __ESBMC_assume(exit5 == 1);
  else if (trd_nr==6) __ESBMC_assume(exit6 == 1);
  else if (trd_nr==7) __ESBMC_assume(exit7 == 1);
  else if (trd_nr==8) __ESBMC_assume(exit8 == 1);
  else if (trd_nr==9) __ESBMC_assume(exit9 == 1);
  else __ESBMC_assert(0, "unexpected number of threads in pthread_join");

  return 0; // we never fail
#endif
}


#if 0
/* FUNCTION: pthread_cond_broadcast */

#ifndef __ESBMC_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __ESBMC_PTHREAD_H_INCLUDED
#endif

inline int pthread_cond_broadcast(pthread_cond_t *cond)
{
  //__ESBMC_HIDE:
  //printf("broadcast_counter: %d", __ESBMC_cond_broadcast_seq_field(*cond));
  //__ESBMC_cond_broadcast_seq_field(*cond)=1;
  //printf("broadcast_counter: %d", __ESBMC_cond_broadcast_seq_field(*cond));
  __ESBMC_cond_broadcast_seq_field(*cond)=1;
  __ESBMC_assert(__ESBMC_cond_broadcast_seq_field(*cond),"__ESBMC_cond_broadcast_seq_field(*cond)");
  return 0; // we never fail
}
#endif
/* FUNCTION: pthread_cond_init */

#ifndef __ESBMC_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __ESBMC_PTHREAD_H_INCLUDED
#endif

inline int pthread_cond_init(
  pthread_cond_t *cond,
  __const pthread_condattr_t *cond_attr)
{
  __ESBMC_atomic_begin();
  __ESBMC_cond_lock_field(*cond)=0;
  __ESBMC_cond_broadcast_seq_field(*cond)=0;
  __ESBMC_atomic_end();
  return 0;
}

/* FUNCTION: pthread_cond_destroy */

#ifndef __ESBMC_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __ESBMC_PTHREAD_H_INCLUDED
#endif

inline int pthread_cond_destroy(pthread_cond_t *__cond)
{
  __ESBMC_HIDE:
  __ESBMC_cond_lock_field(*__cond)=0;
  return 0;
}

/* FUNCTION: pthread_cond_signal */

#ifndef __ESBMC_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __ESBMC_PTHREAD_H_INCLUDED
#endif

extern inline int pthread_cond_signal(pthread_cond_t *__cond)
{
  __ESBMC_HIDE:
  __ESBMC_cond_lock_field(*__cond)=0;

  return 0;
}

/* FUNCTION: pthread_cond_wait */

#ifndef __ESBMC_PTHREAD_H_INCLUDED
#include <pthread.h>
#define __ESBMC_PTHREAD_H_INCLUDED
#endif

inline int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
  __ESBMC_HIDE:
  extern int trds_in_run, trds_count;
  extern int count_wait=0;
  extern static _Bool deadlock_wait=0;
  __ESBMC_atomic_begin();
  __ESBMC_cond_lock_field(*cond)=1;
#ifndef __ESBMC_DEADLOCK_DETECTION
#ifdef __ESBMC_LOCK_DETECTION
  __ESBMC_assert(__ESBMC_mutex_lock_field(*mutex),"pthread_cond_wait must hold lock upon unlock");
#endif
#endif
  __ESBMC_mutex_lock_field(*mutex)=0;
  ++count_wait;
  __ESBMC_atomic_end();

  __ESBMC_atomic_begin();
#ifndef __ESBMC_DEADLOCK_DETECTION
#ifdef __ESBMC_LOCK_DETECTION
  deadlock_wait = (count_wait == trds_in_run);
  __ESBMC_assert(!deadlock_wait,"deadlock detected with pthread_cond_wait");
#endif
#endif
  __ESBMC_assume(/*deadlock_wait ||*/ __ESBMC_cond_lock_field(*cond)==0);
  --count_wait;
  __ESBMC_atomic_end();
  __ESBMC_mutex_lock_field(*mutex)=1;

  return 0; // we never fail
}
