/*
 * myth.h --- toplevel include file of MassiveThreads
 */
#pragma once
#ifndef MYTH_H_
#define MYTH_H_

#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#include "myth/myth_spinlock.h"
#include "myth/myth_sleep_queue.h"

#ifdef __cplusplus
extern "C" {
#endif

  /* -------------------------------------------------------
     Group: Global Attributes and Initialization Functions
     ------------------------------------------------------- */

  /*
     Type: myth_globalattr_t
   */
  typedef struct {
    size_t stacksize;
    size_t guardsize;
    int n_workers;
    int bind_workers;
    int child_first;
    int initialized;
  } myth_globalattr_t;

  /*
    Function: myth_init

    Initialize MassiveThreads library with
    default parameters (see <myth_init_ex>).
    You normally do not have to call it by
    youself, as it is automatically called
    when you first call any MassiveThreads
    function.  Internally, it invokes underlying
    workers.  You may want to call it to make
    absolutely sure that an initilization has
    taken place before a certain point.

    Returns:

    zero if the library has been successfully initialized.
    non-zero otherwise.

    See Also:
    <myth_init_ex>
  */
  int myth_init(void);

  /*
    Function: myth_init_ex

    Initialize MassiveThreads library with the
    specified global attributes.  You
    normally do not have to call it by
    youself, as <myth_init> is automatically
    called when you first call any
    MassiveThreads function.  You may want to
    call it to explicitly set various attributes.

    Parameters:

    attr - the pointer to global attribute

    Returns:

    zero if the library has been successfully initialized.
    non-zero otherwise.

    See Also:
    <myth_init>, <myth_create>, <myth_create_ex>
  */
  int myth_init_ex(myth_globalattr_t * attr);


  /*
    Function: myth_fini

    Finalize MassiveThreads.

    See Also: <myth_init>, <myth_init_ex>
  */
  void myth_fini(void);

  /*
     Function: myth_globalattr_init

     Parameters:

     attr - global attribute to initialize

     initialize global attributes of MassiveThreads to default values.
     you can then set various attributes using one of
     myth_globalattr_set_ATTRIBUTE functions.

     See Also: <myth_init_ex>,
     <myth_globalattr_destroy>,
     <myth_globalattr_get_stacksize>, <myth_globalattr_set_stacksize>,
     <myth_globalattr_get_n_workers>, <myth_globalattr_set_n_workers>,
     <myth_globalattr_get_bind_workers>, <myth_globalattr_set_bind_workers>
  */
  int myth_globalattr_init(myth_globalattr_t * attr);

  /*
     Function: myth_globalattr_destroy

     Parameters:

     attr - global attribute to destroy

     destroy global attributes of MassiveThreads.

     See Also: <myth_init>, <myth_init_ex>, <myth_globalattr_init>
  */
  int myth_globalattr_destroy(myth_globalattr_t * attr);

  /*
     Function: myth_globalattr_get_stacksize

     Parameters:

     attr - global attribute to get stack size of
     stacksize - address to get the result in

     get the stack size attribute in attr, set either by
     myth_globalattr_init or myth_globalattr_set_stacksize

     See Also:
     <myth_globalattr_init>, <myth_globalattr_set_stacksize>
  */
  int myth_globalattr_get_stacksize(myth_globalattr_t * attr,
				    size_t *stacksize);

  /*
     Function: myth_globalattr_set_stacksize

     Parameters:

     attr - global attribute to set stack size of
     stacksize - the stack size to set

     set the stack size attribute in attr to the specified stacksize.

     See Also:
     <myth_globalattr_init>, <myth_globalattr_get_stacksize>
  */
  int myth_globalattr_set_stacksize(myth_globalattr_t * attr,
				    size_t stacksize);


  /*
     Function: myth_globalattr_get_n_workers

     Parameters:

     attr - global attribute to get the number of workers of
     n_workers - the address to get the number of workers in

     get the number of workers attribute in attr, set either
     by myth_globalattr_init or myth_globalattr_set_n_workers

     See Also:
     <myth_globalattr_init>, <myth_globalattr_set_n_workers>
  */
  int myth_globalattr_get_n_workers(myth_globalattr_t * attr,
				    size_t *n_workers);

  /*
     Function: myth_globalattr_set_n_workers

     Parameters:

     attr - global attribute to set the number of workers of
     n_workers - the number of workers

     set the number of workers attribute of attr

     See Also:
     <myth_globalattr_init>, <myth_globalattr_get_n_workers>
  */
  int myth_globalattr_set_n_workers(myth_globalattr_t * attr,
				    size_t n_workers);

  /*
     Function: myth_globalattr_get_bind_workers

     Parameters:

     attr - global attribute to get the bind_workers attribute of
     bind_workers - the address to get the bind_workers attribute in

     get the bind_workers attribute in attr, set either
     by myth_globalattr_init or myth_globalattr_set_bind_workers

     See Also:
     <myth_globalattr_init>, <myth_globalattr_set_bind_workers>
  */
  int myth_globalattr_get_bind_workers(myth_globalattr_t * attr,
				       int *bind_workers);

  /*
     Function: myth_globalattr_set_bind_workers

     Parameters:

     attr - global attribute to set the bind_workers attribute of
     bind_workers - 1 or 0. 1 specified each worker should be bound to
     a core.

     set the bind_workers attribute of attr

     See Also:
     <myth_globalattr_init>, <myth_globalattr_get_bind_workers>
  */
  int myth_globalattr_set_bind_workers(myth_globalattr_t * attr,
				       int bind_workers);

  /* -------------------------------------------------------
     Group: Basic Thread Functions
     ------------------------------------------------------- */

  /*
     at this point we define myth_thread_t
     to be an opaque pointer. struct myth_thread
     is defined in myth_desc.h
  */

  /*
     Type: myth_thread_t
   */
  typedef struct myth_thread* myth_thread_t;

  /*
     Type: myth_thread_attr_t
   */
  typedef struct myth_thread_attr {
    void * stackaddr;
    size_t stacksize;
    size_t guardsize;
    int detachstate;
    int child_first;
    /* TODO: get rid of them */
    size_t custom_data_size;
    void *custom_data;
  } myth_thread_attr_t;

  /*
     Type: myth_func_t
   */
  typedef void*(*myth_func_t)(void*);

  /*
    Function: myth_create

    Create a new user-level thread executing func(arg)
    with default options.  Note that it is equivalent
    to myth_create_ex(func,arg,0);

    Parameters:

    func - a pointer to a function.
    arg - a pointer given to func.

    Returns:

    The identifier of the newly created user-level thread.

    Bug:

    Should any error occur, it terminates the
    program rather than returning an error
    code.

    See Also:
    <myth_create_ex>, <myth_join>
  */
  myth_thread_t myth_create(myth_func_t func, void* arg);

  /*
    Function: myth_create_ex

    Create a new user-level thread executing func(arg)
    with specified options.

    Parameters:

    id - a pointer, if not NULL, to which id of the created thread will be stored.
    func - a pointer to a function.
    arg - a pointer given to func.
    attr - a pointer to a data structure
    of type <myth_thread_attr_t>
    specifying thread attributes, or NULL
    to mean the deafult.

    Returns:

    0 if succeed.

    Bug:

    Should any error occur, it terminates the
    program rather than returning an error
    code.

    See Also:
    <myth_create>, <myth_join>, <myth_thread_option>
  */
  int myth_create_ex(myth_thread_t*      id,
                     myth_thread_attr_t* attr,
                     myth_func_t         func,
                     void*               arg);

  /*
    Function: myth_join

    Wait for the specified thread th to finish.

    Parameters:

    th - the identifier of the thread to wait for
    result - a pointer to a data structure receiving
    the exit value of the thread, as determined by
    <myth_exit> or the return value of the thread's
    main function.

    See Also:
    <myth_create>, <myth_create_ex>
  */
  int myth_join(myth_thread_t th, void** result);

  int myth_tryjoin(myth_thread_t th, void** result);

  int myth_timedjoin(myth_thread_t th, void** result, const struct timespec *abstime);

  /*
    Function: myth_create_join_many_ex

    Create many user-level threads executing the same function
    with various arguments and attributes and wait for them
    to complete.

    Parameters:

    ids -           base pointer to a (strided) array, to which thread ids of
                    the created threads wll be stored (may be NULL)
    attrs -         base pointer to a (strided) array specifying attributes of
                    threads to create (may be NULL)
    func -          a function to execute by each thread
    args -          base pointer to a (strided) array specifying arguments to func
    results -       base pointer to a (strided) array to which results of the
                    function call will be stored (may be NULL)
    id_stride -     the stride of the ids array, in bytes
    attr_stride -   the stride of the attrs array, in bytes
    arg_stride -    the stride of the args array, in bytes
    result_stride - the stride of the results array, in bytes
    long nthreads - number of threads to execute f

    in its simplest form,

        myth_create_join_many_ex(0, 0, f, X, 0,
                                 0, 0,    s, 0, n);

    will execute f(args), f(args+s), f(args+2*s), ..., and f(args+(n-1)*s),
    each by a separate thread and discard their return values.
    if you want to get return values, give results and result_stride. e.g.,

        myth_create_join_many_ex(0, 0, f, X,  Y,
                                 0, 0,    xs, t, n);

    is equivalent to:

    for all i = 0, ..., n - 1
        ((void **)(Y + i * t))[0] = f(args + i * s);

    note that all stride arguments must be given in bytes.
    this is to allow you to receive results in a field of
    an enclosing structure. e.g.,

     struct { char stuff[100]; void * result } args[nthreads];

    in this case you want to call this function with
    results = &args[0].result and result_stride = sizeof(args[0]);

    consistent with this policy, results is a void pointer,
    although it is internally used as (void **).

    You can similarly specify addresses of attributes and thread ids,
    using the base pointer and the stride.

    Returns:

    0 if succeed.

    Bug:

    Should any error occur, it terminates the
    program rather than returning an error
    code.

    See Also:
    <myth_create>, <myth_join>, <myth_thread_attr>
  */

  int myth_create_join_many_ex(myth_thread_t * ids,
			       myth_thread_attr_t * attrs,
			       myth_func_t func,
			       void * args,
			       void * results,
			       size_t id_stride,
			       size_t attr_stride,
			       size_t arg_stride,
			       size_t result_stride,
			       long nthreads);

  /*
    Function: myth_create_join_various_ex

    Create many user-level threads executing
    various functions with various arguments
    and attributes and wait for them to
    complete.  This is almost the same with
    myth_create_join_many_ex, except that you
    can have threads execute different
    functions.

    Parameters:

    ids -           base pointer to a (strided) array, to which thread ids of
                    the created threads wll be stored (may be NULL)
    attrs -         base pointer to a (strided) array specifying attributes of
                    threads to create (may be NULL)
    funcs -         base pointer to a (strided) array specifying functions
                    to execute
    args -          base pointer to a (strided) array specifying arguments to func
    results -       base pointer to a (strided) array to which results of the
                    function call will be stored (may be NULL)
    id_stride -     the stride of the ids array, in bytes
    attr_stride -   the stride of the attrs array, in bytes
    func_stride -   the stride of the funcs array, in bytes
    arg_stride -    the stride of the args array, in bytes
    result_stride - the stride of the results array, in bytes
    long nthreads - number of threads to execute f

    in its simplest form,

        myth_create_join_many_ex(0, 0, F,  X,  0,
                                 0, 0, fs, xs, 0, n);

    will execute f_0(args), f_1(args+xs), f_2(args+2*xs), ...,
    where f_i = *((myth_func_t *)(F + fs * i)),
    each by a separate thread and discard their return values.
    if you want to get return values, give results and result_stride. e.g.,

        myth_create_join_many_ex(0, 0, f, X, Y,
                                 0, 0,    s, t, n);

    is equivalent to:

    for all i = 0, ..., n - 1
        ((void **)(Y + i * t))[0] = f(args + i * s);

    note that all stride arguments must be given in bytes.
    this is to allow you to receive results in a field of
    an enclosing structure. e.g.,

     struct { char stuff[100]; void * result } args[nthreads];

    in this case you want to call this function with
    results = &args[0].result and result_stride = sizeof(args[0]);

    consistent with this policy, results is a void pointer,
    although it is internally used as (void **).

    You can similarly specify addresses of attributes and thread ids,
    using the base pointer and the stride.

    Returns:

    0 if succeed.

    Bug:

    Should any error occur, it terminates the
    program rather than returning an error
    code.

    See Also:
    <myth_create>, <myth_join>, <myth_thread_attr>
  */

  int myth_create_join_various_ex(myth_thread_t * ids,
				  myth_thread_attr_t * attrs,
				  myth_func_t * funcs,
				  void * args,
				  void * results,
				  size_t id_stride,
				  size_t attr_stride,
				  size_t func_stride,
				  size_t arg_stride,
				  size_t result_stride,
				  long nthreads);

  /*
    Function: myth_exit

    Terminate the calling user-level thread.

    Parameters:

    ret - exit value of the thread, which can
    be retrieved by calling <myth_join> on
    this thread.

    See Also:
    <myth_join>
  */
  void myth_exit(void *ret);

  /*
    Function: myth_detach
  */
  int myth_detach(myth_thread_t th);

  /*
    Function: myth_cleanup_thread
  */
  void myth_cleanup_thread(myth_thread_t th);

  /* -------------------------------------------------------
     Group: Worker Functions
     ------------------------------------------------------- */

  /*
    Function: myth_get_worker_num

    Returns:
    The index of the calling thread, an
    integer x satisfying
    0 <= x < myth_get_num_workers().

    See Also:
    <myth_get_num_workers>
  */
  int myth_get_worker_num(void);

  /*
    Function: myth_get_num_workers

    Returns:
    The number of underlying workers.

    See Also:
    <myth_get_worker_num>
  */
  int myth_get_num_workers(void);

  /*
    Function: myth_is_myth_worker

    Returns:
    1 if the calling OS-level thread is a massivethreads
    worker.  useful when you mix pthreads and massivethreads.

  */
  int myth_is_myth_worker(void);

  /* -------------------------------------------------------
     Group: Thread Attributes
     ------------------------------------------------------- */

  /*
     Function: myth_thread_attr_init
   */
  int myth_thread_attr_init(myth_thread_attr_t * attr);

  /*
     Function: myth_thread_attr_getdetachstate
   */
  int myth_thread_attr_getdetachstate(const myth_thread_attr_t *attr,
				      int *detachstate);

  /*
     Function: myth_thread_attr_setdetachstate
   */
  int myth_thread_attr_setdetachstate(myth_thread_attr_t *attr,
				      int detachstate);

  /*
     Function: myth_thread_attr_getguardsize
   */
  int myth_thread_attr_getguardsize(const myth_thread_attr_t *attr,
				    size_t *guardsize);

  /*
     Function: myth_thread_attr_setguardsize
   */
  int myth_thread_attr_setguardsize(myth_thread_attr_t *attr, size_t guardsize);

  /*
     Function: myth_thread_attr_getstacksize
   */
  int myth_thread_attr_getstacksize(const myth_thread_attr_t *attr,
				    size_t *stacksize);

  /*
     Function: myth_thread_attr_setstacksize
   */
  int myth_thread_attr_setstacksize(myth_thread_attr_t *attr, size_t stacksize);

  /*
     Function: myth_thread_attr_getstack
   */
  int myth_thread_attr_getstack(const myth_thread_attr_t *attr,
				void **stackaddr, size_t *stacksize);

  /*
     Function: myth_thread_attr_setstack
   */
  int myth_thread_attr_setstack(myth_thread_attr_t *attr,
				void *stackaddr, size_t stacksize);

  /*
     Function: myth_getattr_default_np
   */
  int myth_getattr_default_np(myth_thread_attr_t *attr);

  /*
     Function: myth_getattr_np
   */
  int myth_getattr_np(myth_thread_t thread, myth_thread_attr_t *attr);

  /*
     Function: myth_getconcurrency
   */
  int myth_getconcurrency(void);

  /* -------------------------------------------------------
     Group: Yield
     ------------------------------------------------------- */

  enum {
    myth_yield_option_half_half = 0,
    myth_yield_option_local_only = 1,
    myth_yield_option_local_first = 2,
    myth_yield_option_steal_only = 3,
    myth_yield_option_steal_first = 4
  };

  /*
    Function: myth_yield_ex

    Yield execution to another user-level thread.

    Parameters:
    yield_opt - take one of the following values and change
    the behavior.

      myth_yield_option_half_half :
        behave like myth_yield_option_local_first with probability 1/2
	and like myth_yield_option_steal_first with probability 1/2
      myth_yield_option_local_only :
        try to yield to another thread in the local run queue.
	if none exist, the caller keeps running.
      myth_yield_option_local_first :
        try to yield to another thread in the local run queue.
	if none exist, an attempt is made to steal another thread
	in a remote run queue; if it succeeds, yields to it. otherwise
        keep running.
      myth_yield_option_steal_only :
        an attempt is made to steal another thread
	in a remote run queue; if it succeeds, yield to it. otherwise
        keep running.
      myth_yield_option_steal_first :
        an attempt is made to steal another thread
	in a remote run queue; if it succeeds, yield to it. otherwise
        try to yield to another thread in the local run queue.
	if none exist, the caller keeps running.

    Note:
    Available options as well as detailed behaviors may change in future.

    See Also:
    <myth_yield>

  */
  void myth_yield_ex(int yield_opt);

  /*
    Function: myth_yield

    it is equivalent to myth_yield_ex(myth_yield_option_half_half);
    with probability 1/2, try to yield to a thread in the local
    queue and if none is found try to steal a thread from a remote
    queue. do the opposite with probability 1/2.

    See Also:
    <myth_yield>

    Note:
    The above describes the current implementation,
    which may change in future.  You should not rely
    on its exact behavior (other than it switches
    to another user-level thread).
  */
  void myth_yield(void);

  /*
     Function: myth_sched_yield
   */
  int myth_sched_yield(void);

  /* -------------------------------------------------------
     Group: Cancel
     ------------------------------------------------------- */

  /*
     Function: myth_setcancelstate
   */
  int myth_setcancelstate(int state, int *oldstate);

  /*
     Function: myth_setcanceltype
   */
  int myth_setcanceltype(int type, int *oldtype);

  /*
     Function: myth_cancel
   */
  int myth_cancel(myth_thread_t th);

  /*
     Function: myth_testcancel
   */
  void myth_testcancel(void);

  /* -------------------------------------------------------
     Group: Once
     ------------------------------------------------------- */

  enum {
    myth_once_state_init = 0,
    myth_once_state_in_progress = 1,
    myth_once_state_completed = 2
  };

  /*
     Type: myth_once_t
   */
  typedef struct {
    volatile int state;
  } myth_once_t;

  /*
     Function: myth_once
   */
  int myth_once(myth_once_t * once_control, void (*init_routine)(void));

  /* -------------------------------------------------------
     Group: Mutex
     ------------------------------------------------------- */

  enum {
    MYTH_MUTEX_NORMAL = 0,
    MYTH_MUTEX_ERRORCHECK = 1,
    MYTH_MUTEX_RECURSIVE = 2,
    MYTH_MUTEX_INVALID = 3,
    MYTH_MUTEX_DEFAULT = MYTH_MUTEX_NORMAL
  };

  /*
     Type: myth_mutexattr_t
   */
  typedef struct myth_mutexattr {
    int type;			/* one of the above constants */
  } myth_mutexattr_t;

  /*
     Type: myth_mutex_t
   */
  typedef struct myth_mutex {
    int magic;
    myth_mutexattr_t attr;
    myth_sleep_queue_t sleep_q[1];
    volatile long state;		/* n_waiters|locked */
  } myth_mutex_t;

  enum { myth_mutex_magic_no = 123456789,
         myth_mutex_magic_no_initializing = 987654321 };

#define MYTH_MUTEX_INITIALIZER { myth_mutex_magic_no, { MYTH_MUTEX_DEFAULT }, { MYTH_SLEEP_QUEUE_INITIALIZER }, 0 }

  /*
    Function: myth_mutex_init

    Initialize a mutex.

    Parameters:

    mutex - a pointer to a mutex data structure to initialize.
    attr - a pointer to mutex attributes.

    Returns:

    zero if suceeds or an errno.

    See Also:

    <myth_mutex_destroy>, <myth_mutex_lock>, <myth_mutex_trylock>, <myth_mutex_unlock>
  */
  int myth_mutex_init(myth_mutex_t * mutex,
		      const myth_mutexattr_t * attr);

  /*
    Function: myth_mutex_destroy

    Destroy a mutex.

    Parameters:

    mutex - a pointer to a mutex data structure to initialize.

    Returns:

    zero if suceeds or an errno.

    See Also:

    <myth_mutex_init>, <myth_mutex_lock>, <myth_mutex_trylock>, <myth_mutex_unlock>
  */
  int myth_mutex_destroy(myth_mutex_t * mutex);

  /*
    Function: myth_mutex_trylock

    Try to lock a mutex.

    Parameters:

    mutex - a mutex to try to lock.

    Returns:

    zero if it successfully acquired a lock.
    an errno otherwise.

    See Also:

    <myth_mutex_init>, <myth_mutex_destroy>, <myth_mutex_lock>, <myth_mutex_unlock>
  */
  int myth_mutex_trylock(myth_mutex_t * mtx);

  /*
    Function: myth_mutex_lock

    Lock a mutex.

    Parameters:

    mutex - a mutex to lock.

    Returns:

    zero if suceeds or an errno when an error occurred.

    See Also:

    <myth_mutex_init>, <myth_mutex_destroy>, <myth_mutex_trylock>, <myth_mutex_unlock>
  */
  int myth_mutex_lock(myth_mutex_t * mtx);

  /*
    Function: myth_mutex_timedlock

    Lock a mutex.

    Parameters:

    mutex - a mutex to lock.
    abstime - absolute time the function returns when the lock cannot be acquired

    Returns:

    zero if suceeds or an errno when an error occurred.

    See Also:

    <myth_mutex_init>, <myth_mutex_destroy>, <myth_mutex_trylock>, <myth_mutex_unlock>
  */
  int myth_mutex_timedlock(myth_mutex_t * mtx, const struct timespec * abstime); // const struct timespec *restrict abstime

  /*
    Function: myth_mutex_unlock

    Unlock a mutex.

    Parameters:

    mutex - a mutex to unlock.

    Returns:

    zero if suceeds or an errno when an error occurred.

    See Also:

    <myth_mutex_init>, <myth_mutex_destroy>, <myth_mutex_lock>, <myth_mutex_trylock>
  */
  int myth_mutex_unlock(myth_mutex_t * mtx);

  /*
     Function: myth_mutexattr_init
   */
  int myth_mutexattr_init(myth_mutexattr_t *attr);

  /*
     Function: myth_mutexattr_destroy
   */
  int myth_mutexattr_destroy(myth_mutexattr_t *attr);

  /*
     Function: myth_mutexattr_gettype
   */
  int myth_mutexattr_gettype(const myth_mutexattr_t * attr,
			     int * type);

  /*
     Function: myth_mutexattr_settype
   */
  int myth_mutexattr_settype(myth_mutexattr_t *attr, int type);

  /* -------------------------------------------------------
     Group: Reader-Writer Lock
     ------------------------------------------------------- */

  enum {
    MYTH_RWLOCK_PREFER_READER,
    MYTH_RWLOCK_PREFER_WRITER,
    MYTH_RWLOCK_PREFER_WRITER_NONRECURSIVE,
    MYTH_RWLOCK_PREFER_INVALID
  };

  /*
     Type: myth_rwlockattr_t
   */
  typedef struct myth_rwlockattr {
    int kind;
  } myth_rwlockattr_t;

  /*
     Type: myth_rwlock_t
   */
  typedef struct myth_rwlock {
    myth_sleep_queue_t sleep_q[1];
    volatile long state;
    myth_rwlockattr_t attr;
  } myth_rwlock_t;

#define MYTH_RWLOCK_INITIALIZER { { MYTH_SLEEP_QUEUE_INITIALIZER }, 0, { MYTH_RWLOCK_DEFAULT } }

  /*
     Function: myth_rwlock_init
  */
  int myth_rwlock_init(myth_rwlock_t * rwlock,
		       const myth_rwlockattr_t * attr);

  /*
     Function: myth_rwlock_destroy
  */
  int myth_rwlock_destroy(myth_rwlock_t *rwlock);

  /*
     Function: myth_rwlock_rdlock
  */
  int myth_rwlock_rdlock(myth_rwlock_t *rwlock);

  /*
     Function: myth_rwlock_tryrdlock
  */
  int myth_rwlock_tryrdlock(myth_rwlock_t *rwlock);

  /*
     Function: myth_rwlock_timedrdlock
  */
  int myth_rwlock_timedrdlock(myth_rwlock_t * rwlock,
			      const struct timespec * abstime);

  /*
     Function: myth_rwlock_wrlock
  */
  int myth_rwlock_wrlock(myth_rwlock_t *rwlock);

  /*
     Function: myth_rwlock_trywrlock
  */
  int myth_rwlock_trywrlock(myth_rwlock_t *rwlock);

  /*
     Function: myth_rwlock_timedwrlock
  */
  int myth_rwlock_timedwrlock(myth_rwlock_t * rwlock,
			      const struct timespec * abstime);

  /*
     Function: myth_rwlock_unlock
  */
  int myth_rwlock_unlock(myth_rwlock_t *rwlock);

  /*
     Function: myth_rwlockattr_init
  */
  int myth_rwlockattr_init(myth_rwlockattr_t *attr);

  /*
     Function: myth_rwlockattr_destroy
  */
  int myth_rwlockattr_destroy(myth_rwlockattr_t *attr);

  /*
     Function: myth_rwlockattr_getkind
  */
  int myth_rwlockattr_getkind(const myth_rwlockattr_t *attr, int *pref);

  /*
     Function: myth_rwlockattr_setkind
  */
  int myth_rwlockattr_setkind(myth_rwlockattr_t *attr, int pref);

  /* -------------------------------------------------------
     Group: Condition Variable
     ------------------------------------------------------- */

  /*
     Type: myth_condattr_t
   */
  typedef struct myth_condattr {
    void * unused;		/* just to suppress warning against empty struct */
  } myth_condattr_t;

  /*
     Type: myth_cond_t

     Conditional variable data structure
   */
  typedef struct myth_cond {
    myth_sleep_queue_t sleep_q[1];
    myth_condattr_t attr;
  } myth_cond_t;

#define MYTH_COND_INITIALIZER { { MYTH_SLEEP_QUEUE_INITIALIZER }, { } }

  /*
    Function: myth_cond_init

    Initialize a condition variable.

    Parameters:

    cond - a pointer to a condition variable to initialize
    attr - a pointer to condition variable attributes, or NULL

    Returns:

    Zero if succeed, or an errno when an error occurred.

    See Also:

    <myth_cond_destroy>, <myth_cond_wait>, <myth_cond_signal>, <myth_cond_broadcast>
  */
  int myth_cond_init(myth_cond_t * cond,
		     const myth_condattr_t * attr);

  /*
    Function: myth_cond_destroy

    Destroy a condition variable.

    Parameters:

    cond - a pointer to a condition variable to destroy.

    Returns:

    Zero if succeed, or an errno when an error occurred.

    See Also:

    <myth_cond_init>, <myth_cond_wait>, <myth_cond_signal>, <myth_cond_broadcast>
  */
  int myth_cond_destroy(myth_cond_t * cond);

  /*
    Function: myth_cond_signal

    Wake up at least one thread blocking on a condition variable.

    Parameters:

    cond - a pointer to a condition variable to signal.

    Returns:

    Zero if succeed, or an errno when an error occurred.

    See Also:

    <myth_cond_init>, <myth_cond_destroy>, <myth_cond_wait>, <myth_cond_broadcast>
  */
  int myth_cond_signal(myth_cond_t * c);

  /*
    Function: myth_cond_broadcast

    Wake up all threads blocking on a condition variable.

    Parameters:

    cond - a pointer to a condition variable from which threads
    are to wake up.

    Returns:

    Zero if succeed, or an errno when an error occurred.

    See Also:

    <myth_cond_init>, <myth_cond_destroy>, <myth_cond_wait>, <myth_cond_signal>
  */
  int myth_cond_broadcast(myth_cond_t * cond);

  /*
    Function: myth_cond_wait

    Atomically unlock a mutex and block on a condition variable.

    Parameters:

    cond - a pointer to a condition variable to block on.
    mutex - a pointer to a mutex to unlock

    Returns:

    Zero if succeed, or an errno when an error occurred.

    See Also:

    <myth_cond_init>, <myth_cond_destroy>, <myth_cond_signal>, <myth_cond_broadcast>
  */
  int myth_cond_wait(myth_cond_t * cond, myth_mutex_t * mutex);

  /*
     Function: myth_cond_timedwait
  */
  int myth_cond_timedwait(myth_cond_t * cond,
			  myth_mutex_t * mutex,
			  const struct timespec * abstime);

  /*
     Function: myth_condattr_init
  */
  int myth_condattr_init(myth_condattr_t *attr);

  /*
     Function: myth_condattr_destroy
  */
  int myth_condattr_destroy(myth_condattr_t *attr);

  /* -------------------------------------------------------
     Group: Barrier
     ------------------------------------------------------- */

  /*
     Type: myth_barrierattr_t
   */
  typedef struct myth_barrierattr {
    void * unused;		/* just to suppress warning against empty struct */
  } myth_barrierattr_t;

  /*
     Type: myth_barrier_t
   */
  typedef struct myth_barrier {
    long n_threads;
    volatile long state;
    //myth_sleep_queue_t sleep_q[1];
    myth_sleep_stack_t sleep_s[1];
    myth_barrierattr_t attr;
  } myth_barrier_t;

#define MYTH_BARRIER_SERIAL_THREAD 1

  /*
     Function: myth_barrier_init

     Initialize a barrier.

     Parameters:
     barrier - a pointer to a barrier data structure to initialize.
     attr - a pointer to barrier attributes
     count - the number of threads going to synchronize with this barrier

     Returns:
     Zero if succeeded.  An errno if failed.
  */
  int myth_barrier_init(myth_barrier_t * barrier,
			const myth_barrierattr_t * attr,
			unsigned int count);

  /*
     Function: myth_barrier_destroy

     Destroy a barrier.

     Parameters:
     barrier - a pointer to a barrier data structure to destroy.
  */
  int myth_barrier_destroy(myth_barrier_t *barrier);

  /*
     Function: myth_barrier_wait

     Wait on a barrier.

     Parameters:
     barrier - a pointer to a barrier data structure on which
     the calling thread synchronizes

     Returns:

     When a barrier succeeds,
     MYTH_BARRIER_SERIAL_THREAD is returned to
     a single thread whereas zeros to other
     threads.  When a barrier fails,
     an errno.
  */
  int myth_barrier_wait(myth_barrier_t *barrier);

  /*
     Function: myth_barrierattr_init
  */
  int myth_barrierattr_init(myth_barrierattr_t *attr);

  /*
     Function: myth_barrierattr_destroy
  */
  int myth_barrierattr_destroy(myth_barrierattr_t *attr);

  /* -------------------------------------------------------
     Group: Join Counter
     ------------------------------------------------------- */

  /*
     Type: myth_join_counterattr_t
   */
  typedef struct myth_join_counterattr {
    void * unused;		/* just to suppress warning against empty struct */
  } myth_join_counterattr_t;

  /*
     Type: myth_join_counter_t
   */
  typedef struct myth_join_counter {
    /* TODO: conserve space? */
    long n_threads;		/* const : number of decrements to see */
    int n_threads_bits;		/* number of bits to represent n_threads */
    long state_mask;		/* (1 << n_threads_bits) - 1 */
    volatile long state;
    myth_sleep_queue_t sleep_q[1];
    myth_join_counterattr_t attr;
  } myth_join_counter_t;

  /*
     Function: myth_join_counter_init
  */
  int myth_join_counter_init(myth_join_counter_t * jc,
			     const myth_join_counterattr_t * attr, int val);

  /*
     Function: myth_join_counter_wait
  */
  int myth_join_counter_wait(myth_join_counter_t * jc);

  /*
     Function: myth_join_counter_dec
  */
  int myth_join_counter_dec(myth_join_counter_t * jc);

  /*
     Function: myth_join_counterattr_init
  */
  int myth_join_counterattr_init(myth_join_counterattr_t * attr);

  /*
     Function: myth_join_counterattr_destroy
  */
  int myth_join_counterattr_destroy(myth_join_counterattr_t * attr);

  /* -------------------------------------------------------
     Group: Full Empty Lock
     ------------------------------------------------------- */

  /*
     Type: myth_felockattr_t
   */
  typedef struct myth_felockattr {
    void * unused;		/* just to suppress warning against empty struct */
  } myth_felockattr_t;

  /*
     Type: myth_felock_t
   */
  typedef struct myth_felock {
    myth_mutex_t mutex[1];
    myth_cond_t cond[2];
    int status;
    myth_felockattr_t attr;
  } myth_felock_t;

  /*
     Function: myth_felock_init
   */
  int myth_felock_init(myth_felock_t * fe, const myth_felockattr_t * attr);

  /*
     Function: myth_felock_destroy
   */
  int myth_felock_destroy(myth_felock_t * fe);

  /*
     Function: myth_felock_lock
   */
  int myth_felock_lock(myth_felock_t * fe);

  /*
     Function: myth_felock_unlock
   */
  int myth_felock_unlock(myth_felock_t * fe);

  /*
     Function: myth_felock_wait_and_lock
   */
  int myth_felock_wait_and_lock(myth_felock_t * fe, int status_to_wait);

  /*
     Function: myth_felock_mark_and_signal
   */
  int myth_felock_mark_and_signal(myth_felock_t * fe,int status_to_signal);

  /*
     Function: myth_felock_status
   */
  int myth_felock_status(myth_felock_t * fe);

  /*
     Function: myth_felockattr_init
   */
  int myth_felockattr_init(myth_felockattr_t * attr);

  /*
     Function: myth_felockattr_destroy
   */
  int myth_felockattr_destroy(myth_felockattr_t * attr);

  /* -------------------------------------------------------
     Group: Uncondition Variable
     ------------------------------------------------------- */

  /*
     Type: myth_uncond_t
   */
  typedef struct {
    volatile myth_thread_t th; /* the thread sleeping on it */
  } myth_uncond_t;

  /*
     Function: myth_uncond_init

     initialize an uncondition variable.

     Parameters:
     uncond - a pointer to an unconditional variable data structure
     to initialize

     Returns:
     zero if it succeeds and non-zero otherwise
   */
  int myth_uncond_init(myth_uncond_t * uncond);

  /*
     Function: myth_uncond_destroy

     destroy an uncondition variable.

     Parameters:
     uncond - a pointer to an unconditional variable data structure
     to destroy

     Returns:
     zero if it succeeds and non-zero otherwise
   */
  int myth_uncond_destroy(myth_uncond_t * u);

  /*
     Function: myth_uncond_wait

     block on an uncondition variable, to be waken up later
     by myth_uncond_signal.  there can be only one thread
     blocking on a single myth_uncond_t variable at the same time.

     this function is typically called after
     the caller checked a data structure and learned that
     it cannot proceed (e.g., a caller thread trying to
     get an element from a queue learned the queue is empty).
     unlike cond_wait, however, it does not take an extra mutex variable
     that is assumed to be held by the caller.
     thus, it is the user's responsibility to implement a means
     to resolve the race condition between the caller (P) and another
     thread (Q) that might be changing the data structure concurrently.
     unless correctly done, it might cause a deadlock bug;
     if Q changes the data structure a moment
     after P learned it cannot proceed but before
     P enters myth_uncond_wait, Q might miss the opportunity
     to wake up P.  myth_uncond_signal waits until a thread blocks
     on myth_uncond_t and wakes it up.

     In summary, a typical (correct) sequence to use myth_uncond_wait
     and myth_uncond_signal is as follows.

     P:
       1: atomically_change_data_to_indicate_I_am_sleeping;

       2: myth_uncond_wait(u);

     Q:
       3: atomically_change_data_to_indicate_none_is_sleeping;

       4: myth_uncond_signal(u);

     line 1 and 3 must be done atomically with respect to each other.
     when 1 succeds, a subsequent execution of line 3 by Q must witness
     P should be blocking.  when Q enters line 4, P might not have
     executed line 2.  myth_uncond_signal guarantees it waits for P
     to enter.

     Parameters:
     uncond - a pointer to an unconditional variable data structure
     on which the calling thread blocks.

     Returns:
     zero if it succeeds and non-zero otherwise
   */
  int myth_uncond_wait(myth_uncond_t * uncond);

  /*
     Function: myth_uncond_signal

     unblock the thread blocking on uncond.
     even if no threads are found on uncond at the moment
     of the call to this function, the caller _waits_ for
     a thread to block on it, and then wakes it up.
     in other words, this function _always_ wakes up a thread.

     this function is typically called after
     the caller checked a data structure and learned that
     a thread should be blocked waiting for a condition to be met.
     unlike cond_signal, this function does not assume there is a common
     mutex protecting the data structure. therefore it is
     the user's responsibility to implement a means for the
     caller to be able to "learn that a thread should be blocked."

     see the description of myth_uncond_wait for details.


     Parameters:
     uncond - a pointer to an unconditional variable data structure
     on which a thread is blocked.

     Returns:
     zero if it succeeds and non-zero otherwise

   */
  int myth_uncond_signal(myth_uncond_t * uncond);

  /* -------------------------------------------------------
     Group: Thread Local Storage
     ------------------------------------------------------- */

  /*
     Type: myth_key_t
   */
  typedef int myth_key_t;

  /*
    Function: myth_key_create

    Create a key for user-level thread-specific data.

    Parameters:

    key - a pointer to which the created key will be stored.
    destr_function - a pointer to a destructor function.


    Returns:

    Zero if succeed, or an errno when an error occurred.

    Bug:

    destr_function is ignored in the current implementation.

    See Also:

    <myth_key_delete>, <myth_setspecific>, <myth_getspecific>
  */
  int myth_key_create(myth_key_t *key, void (*destr_function)(void *));

  /*
    Function: myth_key_delete

    Delete a key for user-level thread-specific data.

    Parameters:

    key - key to delete

    Returns:

    Zero if succeed, or an errno when an error occurred.

    See Also:

    <myth_key_create>, <myth_setspecific>, <myth_getspecific>
  */
  int myth_key_delete(myth_key_t key);

  /*
    Function: myth_setspecific

    Associate a thread-specific data with a key.

    Parameters:

    key - a key created by myth_key_create
    data - a data to be associated with key

    Returns:

    Zero if succeed, or an errno when an error occurred.

    See Also:

    <myth_key_create>, <myth_key_delete>, <myth_getspecific>
  */
  int myth_setspecific(myth_key_t key, const void *data);

  /*
    Function: myth_getspecific

    Obtain a user-level thread-specific data
    associated with a key.

    Parameters:

    key - a key to retrieve data.

    Returns:

    a data previously associated with key via
    myth_setspecific, or NULL if no data has
    been associated with it.

    See Also:

    <myth_key_create>, <myth_key_delete>, <myth_setspecific>
  */
  void *myth_getspecific(myth_key_t key);

  /* -------------------------------------------------------
     Group: Worker Local Storage
     ------------------------------------------------------- */

  typedef pthread_key_t myth_wls_key_t;

  /*
    Function: myth_wls_key_create

    Create a key for worker-specific data.

    Parameters:

    key - a pointer to which the created key will be stored.
    destr_function - a pointer to a destructor function.

    wls_key is used to create data specific to each underlying
    worker. you can think of it as a simple wrapper to pthread_key_create.

    Returns:

    Zero if succeed, or an errno when an error occurred.

    Bug:

    destr_function is ignored in the current implementation.

    See Also:

    <myth_wls_key_delete>, <myth_wls_setspecific>, <myth_wls_getspecific>
  */
  int myth_wls_key_create(myth_wls_key_t *key, void (*destr_function)(void *));

  /*
    Function: myth_wls_key_delete

    Delete a key for worker-specific data.

    Parameters:

    key - key to delete

    Returns:

    Zero if succeed, or an errno when an error occurred.

    See Also:

    <myth_wls_key_create>, <myth_wls_setspecific>, <myth_wls_getspecific>
  */
  int myth_wls_key_delete(myth_wls_key_t key);

  /*
    Function: myth_wls_setspecific

    Associate a worker-specific data with a key.

    Parameters:

    key - a key created by myth_key_create
    data - a data to be associated with key

    Returns:

    Zero if succeed, or an errno when an error occurred.

    See Also:

    <myth_wls_key_create>, <myth_wls_key_delete>, <myth_wls_getspecific>
  */
  int myth_wls_setspecific(myth_wls_key_t key, const void *data);

  /*
    Function: myth_wls_getspecific

    Obtain a worker-specific data
    associated with a key.

    Parameters:

    key - a key to retrieve data.

    Returns:

    a data previously associated with key via
    myth_wls_setspecific, or NULL if no data has
    been associated with it.

    See Also:

    <myth_wls_key_create>, <myth_wls_key_delete>, <myth_wls_setspecific>
  */
  void *myth_wls_getspecific(myth_wls_key_t key);

  /* -------------------------------------------------------
     Group: Thread-related Functions
     ------------------------------------------------------- */

  /*
    Function: myth_self

    Returns:
    The identifier of the calling thread.

    See Also:
    <myth_get_worker_num>, <myth_get_num_workers>
  */
  myth_thread_t myth_self(void);

  /*
     Function: myth_equal
   */
  int myth_equal(myth_thread_t t1, myth_thread_t t2);

  /*
     Function: myth_sleep
   */
  unsigned int myth_sleep(unsigned int s);

  /*
     Function: myth_usleep
   */
  int myth_usleep(useconds_t usec);

  /*
     Function: myth_nanosleep
   */
  int myth_nanosleep(const struct timespec *req, struct timespec *rem);

  /* -------------------------------------------------------
     Group: Almost Deterministic Work Stealing

     Low-level APIs for Almost Deterministic Work Stealing (ADWS).
     You can schedule threads "almost" deterministically to improve data
     locality. If you enable stealing in ADWS, you can expect the scheduler
     balances the load dynamically in a locality-aware way.
     In ADWS, threads must be created in a form of a task group (i.e., nested
     fork-join model).

     An example usage is here:
     --- Code
     myth_workers_range_t workers_range;
     // fork
     myth_thread_t th1 = myth_adws_create_first(func1, arg1, w1, w_total, &workers_range);
     myth_thread_t th2 = myth_adws_create(func2, arg2, w2);
     myth_thread_t th3 = myth_adws_create(func3, arg3, w3);
     myth_thread_t th4 = myth_adws_create(func4, arg4, w4);
     // join
     myth_adws_join(th1, NULL);
     myth_adws_join(th2, NULL);
     myth_adws_join(th3, NULL);
     myth_adws_join_last(th4, NULL, workers_range);
     // free thread objects
     myth_cleanup_thread(th1);
     myth_cleanup_thread(th2);
     myth_cleanup_thread(th3);
     myth_cleanup_thread(th4);
     ---
     In this case, a task group is comprised of th1, th2, th3 and th4. A task
     group begins with <myth_adws_create_first> and ends with <myth_adws_join_last>.
     We assume w_total == w1 + w2 + w3 + w4 (a few rounding errors are allowed).

     A range of workers is given by calling <myth_adws_create_first>, and you
     must pass it to the scheduler at the end of the task group (i.e., when
     calling <myth_adws_join_last>).

     Threads created by <myth_adws_create> or <myth_adws_create_first> are
     *not* automatically freed in <myth_adws_join> or <myth_adws_join_last>.
     You *must* call <myth_cleanup_thread> immediately after they are joined.
     This is because of a performance issue. In most cases, th1, th2, th3 and
     th4 are created by the worker who calls <myth_adws_create_first>. When
     <myth_adws_join_last> is completed, the current thread is usually (not
     always) returned to the same worker who calls <myth_adws_create_first>.
     Then workers can collect all threads created by them and reuse them later
     if no steal happens.

     You can hide these boring things by using some useful wrappers for ADWS.
     For exapmle, You can use mtbb, a C++ interface for MassiveThreads, which
     resembles Intel Threading Building Blocks (TBB).
     ------------------------------------------------------- */

  /*
     Type: myth_workers_range_t

     Fields:

     left         - (double) a left boundary in number line of workers.
     right        - (double) a right boundary in number line of workers.
     left_worker  - (int) a worker at the left boundary of the range. equals to (int)left.
     right_worker - (int) a worker at the right boundary of the range. equals to (int)right.

     See Also:
     <myth_adws_create_first>, <myth_adws_join>, <myth_adws_join_last>
   */
  typedef struct myth_workers_range {
    double left;
    double right;
    int left_worker;
    int right_worker;
  } myth_workers_range_t;

  /*
    Function: myth_adws_create_first

    Create a new user-level thread executing func(arg) with default options.
    The newly created thread is scheduled by Almost Deterministic Work Stealing
    (ADWS). This function must be called at the begenning of task groups only
    once. Following task creations in the same task group must be done by
    <myth_adws_create>.

    Parameters:

    func          - a pointer to a function.
    arg           - a pointer given to func.
    work          - an amount of work in the created thread and its descendants.
    w_total       - a total amount of work in the entire task group.
    workers_range - (output) a pointer to a range of workers.
                    A range of workers is given to the task group. It should be
                    passed to <myth_adws_join_last> at the end of the task group.

    Returns:

    The identifier of the newly created user-level thread.

    See Also:
    <myth_create>, <myth_adws_create>, <myth_adws_join>, <myth_adws_join_last>
  */
  myth_thread_t myth_adws_create_first(myth_func_t           func,
                                       void*                 arg,
                                       double                work,
                                       double                w_total,
                                       myth_workers_range_t* workers_range);

  /*
    Function: myth_adws_create

    Create a new user-level thread executing func(arg) with default options.
    The newly created thread is scheduled by Almost Deterministic Work Stealing
    (ADWS). The created thread cannot be the first thread in task groups.

    Parameters:

    func - a pointer to a function.
    arg  - a pointer given to func.
    work - an amount of work in the created thread and its descendants.

    Returns:

    The identifier of the newly created user-level thread.

    See Also:
    <myth_create>, <myth_adws_create_first>, <myth_join>
  */
  myth_thread_t myth_adws_create(myth_func_t func,
                                 void*       arg,
                                 double      work);

  /*
    Function: myth_adws_create_ex_first

    Create a new user-level thread executing func(arg) with specified options.
    See <myth_adws_create_first>.

    Parameters:

    id            - a pointer, if not NULL, to which id of the created thread will be stored.
    attr          - a pointer to a data structure
    func          - a pointer to a function.
    arg           - a pointer given to func.
    work          - an amount of work in the created thread and its descendants.
    w_total       - a total amount of work in the entire task group.
    workers_range - (output) a pointer to a range of workers.
                    A range of workers is given to the task group. It should be
                    passed to <myth_adws_join_last> at the end of the task group.

    Returns:

    0 if succeed.

    See Also:
    <myth_create_ex>, <myth_adws_create_first>
  */
  int myth_adws_create_ex_first(myth_thread_t*        id,
                                myth_thread_attr_t*   attr,
                                myth_func_t           func,
                                void*                 arg,
                                double                work,
                                double                w_total,
                                myth_workers_range_t* workers_range);

  /*
    Function: myth_adws_create_ex

    Create a new user-level thread executing func(arg) with specified options.
    See <myth_adws_create>

    Parameters:

    id   - a pointer, if not NULL, to which id of the created thread will be stored.
    attr - a pointer to a data structure
    func - a pointer to a function.
    arg  - a pointer given to func.
    work - an amount of work in the created thread and its descendants.

    Returns:

    0 if succeed.

    See Also:
    <myth_create_ex>, <myth_adws_create>
  */
  int myth_adws_create_ex(myth_thread_t*      id,
                          myth_thread_attr_t* attr,
                          myth_func_t         func,
                          void*               arg,
                          double              work);

  /*
    Function: myth_adws_join

    Wait for the specified thread th to finish.
    Threads created by using <myth_adws_create> or <myth_adws_create_first> must be
    joined by <myth_adws_join> or <myth_adws_join_last> instead of <myth_join>.

    Parameters:

    th     - the identifier of the thread to wait for
    result - a pointer to a data structure receiving the exit value of
             the thread, as determined by <myth_exit> or the return value
             of the thread's main function.

    See Also:
    <myth_join>, <myth_adws_create_first>, <myth_adws_create>, <myth_adws_join_last>
  */
  int myth_adws_join(myth_thread_t th, void** result);

  /*
    Function: myth_adws_join_last

    Wait for the specified thread th to finish.
    The last join of task groups must be <myth_adws_join_last>. In ADWS,
    threads can be joined by <myth_adws_join> in an arbitrary order, but the
    last join call must be <myth_adws_join_last>.

    Parameters:

    th            - the identifier of the thread to wait for
    result        - a pointer to a data structure receiving the exit value of
                    the thread, as determined by <myth_exit> or the return value
                    of the thread's main function.
    workers_range - a range of workers given by <myth_adws_create_first>.

    See Also:
    <myth_join>, <myth_adws_create_first>, <myth_adws_create>, <myth_adws_join>
  */
  int myth_adws_join_last(myth_thread_t th, void** result, myth_workers_range_t workers_range);

  /*
    Function: myth_adws_get_stealable

    Returns:
    1 if steal is enabled in ADWS, otherwise 0.

    See Also:
    <myth_adws_set_stealable>
  */
  int myth_adws_get_stealable();

  /*
    Function: myth_adws_set_stealable

    Parameters:

    flag - set 1 to enable steals, 0 to disable steals

    See Also:
    <myth_adws_get_stealable>
  */
  void myth_adws_set_stealable(int flag);


  /*
     declare myth_pickle_t to be an opaque structure
   */
  typedef struct myth_pickle myth_pickle_t;
  void myth_serialize(myth_thread_t th,myth_pickle_t * p);
#define myth_ext_serialize(th,p) myth_serialize(th,p)
  myth_thread_t myth_deserialize(myth_pickle_t * p);
  myth_thread_t myth_ext_deserialize(myth_pickle_t * p);

  myth_thread_t myth_steal(void);
#define myth_ext_steal() myth_steal()
  void myth_import(myth_thread_t th);
  void myth_ext_import(myth_thread_t th);

  void myth_release_stack(myth_thread_t th);
  void myth_release_desc(myth_thread_t th);

  void myth_log_start(void);
  void myth_log_pause(void);
  void myth_log_flush(void);
  void myth_log_reset(void);
  void myth_log_annotate_thread(myth_thread_t th,char *name);
  //void myth_log_get_thread_annotation(myth_thread_t th,char *name);
  void myth_sched_prof_start(void);
  void myth_sched_prof_pause(void);

  /* TODO: a duplicated definition is in myth_worker.h */
  typedef struct myth_thread* (*myth_steal_func_t)(int);
  myth_steal_func_t myth_wsapi_set_stealfunc(myth_steal_func_t fn);

#ifdef __cplusplus
} //extern "C"
#endif

#endif /* MYTH_H_ */
