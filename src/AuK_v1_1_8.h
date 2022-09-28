/* 
 * File:   
 * Author: norberto
 *
 * Created on 14 de diciembre de 2018, 17:13
 */

/* In this document the term Task is used as a synonym for Process.
 * This is in honor of Ada, possibly the best language 
 * for systems programming. */

#ifndef AUK
#define	AUK

#define TRUE 1
#define FALSE 0

/* Maximum number of tasks */
#define max_TCB_indx 10

/* Memory size to locate the stack of all processes */
#define max_memory_size_words 3000

/* ¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡ ATTENTION !!!!!!!!!!!!!!!!!!!!!!!
 * This routine must be used before any other routine in the microkernel. 
 * It initializes all elements inside AuK to work when needed.
 *
 * TMR1 and int2 are used inside AuK, then it could not be used by system 
 * to be executed over it.
 * 
 * This routine allows setting timer 1 to activate the scheduler with 
 * the indicated period.
 * 
 * Fcyc must be the frequency of the instruction cycle in Hz, depending on 
 * how the clock signal generation is configured.
 * 
 * Scheduler_period must indicate the desired period of the scheduler 
 * in seconds.
 * 
 * The routine returns 0 if it works well, and -1 if the expected scheduler 
 * period can not be reached with this Fcyc. 
 *
 */

int init_AuK(float Fcyc, float scheduler_period);


/* This routine creates a task (process) to run on the microkernel.
 * 
 * Code_address must contain the address of the function containing 
 * the task code. More specifically:
 * 
 * code address_page could be obtained by means of __builtin_tblpage(func)
 * which actually returns the high part of the memory address of the function,
 * bits <22:16>
 * 
 * code address_offset could be obtained by means of __builtin_tbloffset(func)
 * which actually returns the low part of the memory address of the  function, 
 * bits <15:0>
 * 
 * Where "func" is the name of the function which contains the code of 
 * the new task.
 * 
 * mem_needed must contain the nedded words (of 16 bits) for the task stack. 
 * 
 * It should be taken into consideration that:
 * 
 * 1) When a Task is removed from the processor by the scheduler, 
 * it is necessary to store 32 16-bit words on the stack, corresponding 
 * to the context of the running task (all dps registers). That must be 
 * increased in two words (4 bytes) corresponding to the return address 
 * and bits 0:7 of SR register, of the interrupted task (interrupted by 
 * the timer).
 * 
 * 2) Additionally, when the scheduler is executed it also temporally consumes 
 * 7 words (14 bytes) of stack space.
 * 
 * 3) Some services of the micro-kernel consumes stack space for local data
 * (among others needs). The "mutex-lock" service is the most demanding and 
 * it needs 13 words (26 bytes).
 *
 * 4) Be careful with standard library services, because some of them could 
 * need a great amount of stack. For example, printf could need more than 
 * 170 bytes of stack.
 * 
 * priority must contain the task priority. The larger the number.
 * the higher the priority. Priority must be greather than 0 
 * 
 * 
 * Returns the identifier of the new task if everything performed correctly.
 * 
 * Returns -1 if there is no free task control block. 
 * 
 * Returns -2 if declared priority is not greather than 0.
 * 
 * Returns -3 if there is no enought free memory to satisfy the required
 * quantity.
 * 
 */
int create_task(unsigned int code_address_page, 
                unsigned int code_address_offset,
                unsigned int mem_needed,
                unsigned int priority);


/* This routine must be invoked at the moment the microkernel must be started.
 * 
 * At least one task must exist before starting the microkernel, 
 * otherwise only the idle task will work.
 * 
 */
void start_AuK(void);



/* Returns the task identifier */
int give_me_my_id(void);

/* Returns the number of seconds between two tics of scheduler timer 
 * The tic period is correctly initialized when init_tmr1_int_frequency is
 * invoqued.
 * ¡¡¡Important!!!
 * The actual period (which is returned here) may differ slightly from 
 * the programmed period due to truncation operations on real numbers. 
 * In real time aplications it is possible that this period should be 
 * considered instead of programmed one.*/

float tic_period();


/* Semaphores.
 * Brinch Hansen inspired but including priorities. This implies that when 
 * several processes are blocked in a semaphore (due to having executed 
 * "wait" with the semaphore counter at zero), the processes, 
 * when they are unblocked, do so in order of priority, and FIFO order 
 * within each priority. 
 * 
 * (This new behavior has been introduced to facilitate the implementation 
 * of the "immediate priority ceiling" real time protocol when building 
 * mutexes inside the microkernel).
 */
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
 the highest priority task, which has been waiting in the queue the longest. */
void signal(Tsemaphore *sem);

/* Mutex are objects of exclusive access by tasks, with priority according 
 * to the immediate priority ceiling real time scheduler protocol */
typedef struct mutex
{
    Tsemaphore sem;
    int mutex_priority;
    int task_old_priority;
    int holder_task;
}Tmutex;

/* Initialize a mutex and assigns to it the priority provided in the 
 * second parameter. 
 * To adhere to the immediate priority ceiling protocol, it is necessary 
 * that the priority of the mutex is that of the task with the 
 * highest priority that can use it.*/
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

/* Returns current tic. 
 * The time between consecutive ticks is determined by the 
 * initialization of the Microkernel (init_tmr1_int_frequency). */
unsigned long clock();

/* Block the invoking process until the Tic indicated as parameter 
 * is reached. */
void delay_until(unsigned long tic);

#endif	/* AUK */

