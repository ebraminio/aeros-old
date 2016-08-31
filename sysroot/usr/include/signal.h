/**
 * \file
 */

#ifndef SIGNAL_H
#define SIGNAL_H

#include <sys/types.h>
#include <time.h>
#include <stdint.h>

/**
 * \TODO SIGABRT
 * \TODO SIGFPE
 * \TODO SIGILL
 * \TODO SIGINT
 * \TODO SIGSEGV
 * \TODO SIGTERM
 * \TODO SIG_DFL
 * \TODO SIG_IGN
 * \TODO SIG_ERR
 */

union sigval
{
	int sival_int;
	void* sival_ptr;
};

struct sigevent
{
	int sigev_notify;
	int sigev_signo;
	union sigval sigev_value;
	void(*sigev_notify_function)(union sigval);
	//(pthread_attr_t*) sigev_notify_attributes notification attributes
};

typedef int sig_atomic_t;

typedef struct
{
	void* ss_sp;
	size_t ss_size;
	int ss_flags;
} stack_t;

typedef struct
{
	uint64_t val;
} sigset_t;

typedef struct
{
	int si_signo;	/**< Signal number */
	int si_code;	/**< Signal code */
	int si_errno;	/**< If non-zero, an \a errno value associated with this signal, as describedin \b errno.h */
	pid_t si_pid;	/**< Sending process ID */
	uid_t si_uid;	/**< Real user ID of sending process */
	void* sid_addr;	/**< Address of faulting instruction */
	int si_status;	/**< Exit value or signal */
	union sigval si_value;	/**< Signal value */
} siginfo_t;

typedef struct sigaction
{
	union
	{
		void (*sa_handler)(int);						/**< Pointer to a signal-catching funtion or one one of the SIG_IGN or SIG_DFL. */
		void (*sa_sigaction)(int, siginfo_t*, void*);	/**< Pointer to a signal catching function. */
	};
	sigset_t sa_mask;	/**< Set of signals to be blocked during execution of the signal handling function */
	int sa_flags;		/**< Special flags */
};

#define	SIG_BLOCK	0		 /* Block signals.  */
#define	SIG_UNBLOCK	1		 /* Unblock signals.  */
#define	SIG_SETMASK	2		 /* Set the set of blocked signals.  */

#define SIG_DFL (void (*)(int))1
#define SIG_ERR (void (*)(int))2
#define SIG_IGN (void (*)(int))4

void (*signal(int sig, void (*func)(int)))(int);
int sigsetmask(int mask);
int siggetmask(void);

#define SIGABRT	1
#define SIGALRM	2
#define SIGBUS	3
#define SIGCHLD	4
#define SIGCONT	5
#define SIGFPE	6
#define SIGHUP	7
#define SIGILL	8
#define SIGINT	9
#define SIGKILL	10
#define SIGPIPE	11
#define SIGQUIT	12
#define SIGSEGV	13
#define SIGSTOP	14
#define SIGTERM	15
#define SIGTSTP	16
#define SIGTTIN	17
#define SIGTTOU	18
#define SIGUSR1	19
#define SIGUSR2	20
#define SIGSYS	23
#define SIGTRAP	24
#define SIGURG	25
#define SIGXFSZ	28
#define NSIG	29

extern const char* const sys_siglist[NSIG];
extern const char* const sys_signame[NSIG];

#endif
