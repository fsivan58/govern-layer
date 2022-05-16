/* 
 * File:   
 * Author: norberto
 *
 * Created on 14 de diciembre de 2018, 17:13
 */

#ifndef AUK
#define	AUK

#define TRUE 1
#define FALSE 0

#define max_TCB_indx 10

/* TMR1 is used inside AuK, then it could not be used by system 
 * to be executed */

/* This routine allows setting timer 1 to activate the scheduler with 
 * the indicated period.
 * 
 * Fcyc must be the frequency of the instruction cycle in Hz, depending on 
 * how the clock signal generation is configured.
 * 
 * Scheduller_period must indicate the period of the scheduler in seconds.
 * 
 * The routine returns 0 if it works well, and -1 if the expected scheduler 
 * period can not be reached. */

int init_tmr1_int_frequency(float Fcyc, float scheduller_period);

/* This routine will be called before using any AuK service */
void init_task_manager();

/*-------------------------------------*/
/* Code_address must contain address of the new task code.
 * Code address_page could be obtained by means of __builtin_tblpage(func)
 * Code address_offset could be obtained by means of __builtin_tbloffset(func)
 * Where "func" is the name of the function which contains the code of 
 * the new task.
 * Mem_needed must contain  the nedded words for the task stack 
 * priority must contain the task priority. The larger the number 
 * the higher the priority. Priority must be greather than 0 */
int create_task(unsigned int code_address_page, 
                unsigned int code_address_offset,
                unsigned int mem_needed,
                unsigned int priority);

/* Returns the task identifier */
int give_me_my_id(void);

/* This routine will be called when it is needed AuK start working.
 As its name suggests, this routine also contains the idle task code. */
void idle_task(void);

/* This routine block current active task. */
void block_me(void);

/* This routine returns whether task_id is blocked. */
int is_blocked(int);

/* This routine unblock task_id. */
void unblock(int);

/* Semaphores (Brinch Hansen inspired but including priorities) */
typedef struct semaphore
{
    int semaphore_counter; // Semaphore counter in Brinch Hansen sense.
    int BlockedTasksList[max_TCB_indx]; //List of blocked Tasks
    int in, out; //Pointers for BlockedTasksList.
    int blocked_counter; // Number of blocked tasks
    int lock; // Lock variable for exclusive access to the semaphore
}Tsemaphore;

/* The semaphore is initialized and its counter is set with the value of
 the second parameter. */
void init_semaphore(Tsemaphore *sem, int init_count);


/* It decerements the counter of the semaphore. If the counter value was 
 * already zero, the task is blocked and the counter remains at zero. 
 * Tasks blocked in the semaphore are ordered by priority and FIFO 
 * within prioiries. */
void wait(Tsemaphore *sem);

/* If there are no blocked tasks in the semaphore, signal increments the 
 semaphore counter. If there exist almost a blocked Task, signal unblock
 the first task in the semaphore blocked task queue. */
void signal(Tsemaphore *sem);

/* Mutex are objects of exclusive access by tasks, with priority according 
 * to the immediate priority ceiling real time scheduller protocol */
typedef struct mutex
{
    Tsemaphore sem;
    int mutex_priority;
    int task_old_priority;
    int holder_task;
}Tmutex;

/* Initialize a mutex and assigns to it the priority provided in the 
 * second parameter. */
void mutex_init (Tmutex *new_mutex, int priority);

/* If the mutex is locked by other task, the calling task is blocked in a 
 * semaphore local to the mutex (according to its current priority).
 * If the mutex is free, then it is blocked and the task adquires de priority 
 * of the mutex if it is higher than its current priority. */
void mutex_lock(Tmutex *mutex);

/* The mutex is unlocked and the task adquires its previous priority. 
 * If there exists blocked tasks in the same mutex, there will awaken one 
 * of them according to their priorities and arrival order within priorities. 
 *
 * It is no allowed to unlock a mutex by a task that no has this mutex locked.*/
void mutex_unlock(Tmutex *mutex);

/* Returns current tic (see init_tmr1 in code) */
unsigned long clock();

/* Blocks current task until global tic.  */
void delay_until(unsigned long tic);

#endif	/* AUK */

