# OS-XV6-SCHEDULER
Implemented Scheduling using RR mechanism in xv6

Below are the 3 major areas of xv6 I worked on for this project: 

1) Compensated Round-Robin (CRR) Scheduler Requirements
The current xv6 scheduler implements a very simple Round Robin (RR) policy.  For example, if there are three processes A, B and C, then the xv6 round-robin scheduler will run the jobs in the order  A B C A B C … , where each letter represents a process.  The xv6 scheduler runs each process for at most one timer tick (10 ms); after each timer tick, the scheduler moves the previous job to the end of the ready queue and dispatches the next job in the list.   The xv6 scheduler does not do anything special when a process sleeps or blocks (and is unable to be scheduled); the blocked job is simply skipped until it is ready and it is again its turn in the RR order. 

I implemented a new Compensated RR scheduler with three new features:

- Different time-slice lengths (i.e., a different number of timer ticks) for different processes
- Compensating processes for the amount of time they were blocked by scheduling those processes for a longer time-slice when they awake
- Improving the sleep/wakeup mechanism so that processes are unblocked only after their sleep interval has expired, instead of on every 10 ms timer tick

2) Improved the Sleep/Wake Mechanism

Changed the existing implementation of sleep() syscall.  The original sleep syscall allowed processes to sleep for a specified number of ticks.  Unfortunately, that xv6 implementation of the sleep() syscall forced the sleeping process to wake on every timer tick to check if it has slept for the requested number of timer ticks or not.  This has two drawbacks.  First, it is inefficient to schedule every sleeping process and forces it to perform this check.  Second, and more importantly for this project, if the process was scheduled on a timer tick (even just to perform this check), the scheduler might not properly identify that process as sleeping and grant it compensation ticks.

Fixed this problem of extra wakeups by changing wakeup1() in proc.c. Added additional condition checking to avoid falsely waking up the sleeping process until it is the right time. Added more fields to struct proc to help wakeup1() decide whether if it is time to wake a process.

3)New system calls
Created several new system calls for this project.

- int setslice(int pid, int slice) -> This sets the time-slice of the specified pid to slice. If slice < 0, returns -1. 
- int getslice(int pid) -> This returns the time slice of the process with pid, which must be a positive interger.  If the pid is not valid, it returns -1.
- int fork2(int slice) -> This routine is exactly like the existing fork() system call, except the newly created process begins with the specified time-slice length.   Thus, fork() can be implemented as fork2(getslice(getpid())) since by default the child process inherits the time-slice length of the parent process.  If slice is not positive, then fork2() returns -1.
 -int getpinfo(struct pstat *) -> This system call returns 0 on success and -1 on failure (e.g., the argument is not a valid pointer). If success, some basic information about each process will be returned:
