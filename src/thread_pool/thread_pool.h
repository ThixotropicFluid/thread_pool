#ifndef THREAD_POOL_H
#define THREAD_POOL_H

/**
 * @file thread_pool.h
 * @brief Threadpool implementaion
 * 
 * Structures used for the setup and dispatch of a thread pool.
 * 
 * 
 *
 */

#include "../data_structures/lists/linked_list.h"
#include "stdint.h"

/*
#ifdef _WIN32
    #include <pthreads-w32.h>  // For Windows
#else
    #include <pthread.h>       // For Unix-based systems (Linux, macOS, etc.)
#endif
*/

/** @def THREAD_POOL_MAX_POOLS
 *  @brief Maximum number of thread pools supported.
 */
#define THREAD_POOL_MAX_POOLS 50
/** @def THREAD_POOL_MAX_THREADS
 *  @brief Maximum number of threads in a thread executer.
 */
#define THREAD_POOL_MAX_THREADS 256

#define THEAD_POOL_MAX_STRING_LENGTH 50

/**
 * @brief Flags used for Job execution.
 * 
 */
typedef enum {
    /*!    This Jobs requires input and output buffers swaped*/
    JobBufferSwap = 1 << 0, 
} JobFlags;
/**
 * @brief Flags used to ThreadPool managment
 * 
 */
typedef enum {
    /*!    This pool contains Jobs that require buffer swapping*/
    PoolBufferSwap = 1 << 0, 
    /*!  This pool can be run without regard to other pools status. */
    PoolAsynchronous = 1 << 1, 
    /*!This pool is requesting to be executed. This flag will be unset
    once the pool has been pushed into the job queue. 
    This flag is ignored if PoolScheduled flag is set*/ 
    PoolToExecute = 1 << 2, 
    /*!This pool is part of the regular execution loop. 
    It will automatically be scheduled according first in first out*/
    PoolScheduled = 1 << 3, 

} PoolFlags;
typedef enum {
    /*! Once all pools have been executed. Automaticlly re-run them. If not thread_pool_dispatch has to be called to. This is set by the user */
    AutomaticRefreash = 1 << 0,
    /*! Once all pools have been executed, this flag is set by the system, and cleared by thread_pool_dispatch. This flag is not valid if AutomaticRefreash is enabled*/
    DispatchDone = 1 << 1,
} ExecuterFlags;
typedef enum {
    /*! The pool id that is attempting to be accessed does not exist*/
    POOL_ID_DOES_NOT_EXIST = -1,
    /*! The max number of pools in a thread executer has been reached*/
    POOL_MAX_REACHED = -2,
    /*! Job cannot be found with the scope being searched*/
    JOB_ID_DOES_NOT_EXIST = -3,
    /*!Invalid Job, ensuer that input, output, and job_run pointers are not null*/
    JOB_INVALID = -4,
} ThreadPoolError;


/**
 * @struct ThreadExecuter
 * @brief Manages the control flow for spawning, locking, and serving jobs from different thread pools.
 *
 * This structure is resposible for the priority dispatch of jobs to threads.
 * 
 * This structure has owenership of all members
 */
typedef struct ThreadExecuter {
    // Resposible for contoll flow of spawing, locking, and serving jobs from different threadpools
    // TODO: actually make the multithreading lmao. I'm tryting to get this work single threaded first

    //pthread_t threads[];
    struct ThreadPool* thread_pools[THREAD_POOL_MAX_POOLS];
} ThreadExecuter;
/**
 * @brief A collection of concuretly runable jobs
 * 
 */
typedef struct ThreadPool {
    //  Repseresnts a set of concourent tasks
    uint32_t id; // Unique ID for inserting pool, up to THREAD_POOL_MAX_POOLS pools
    uint32_t priority; // used by ThreadExecuter to serve next job
    uint32_t flags; // flags used by jobs and 

    struct LinkedList jobs; // set of jobs
    
} ThreadPool;
/**
 * @brief 
 * 
 */
typedef struct Job {
    /**
     * @brief Method to run. 
     * 
     */
    void* (*run_job)(const void* input, void* output);
    /**
     * @brief Method to free input and output data if nessisary
     * 
     * Function will run at termination of job, if NULL input and output will be freed normally
     */
    void* (*free_data)(void* input, void* output);
    /**
     * @brief 
     * 
     */
    void* input;
    void* output;
    uint32_t flags;
} Job;


// public methods
/*!Constucts Thread Executer*/
ThreadExecuter* thread_pool_thread_executer_constructor();
void thread_pool_thread_executer_destructor(ThreadExecuter* thread_executer);

ThreadPool* thread_pool_thread_pool_constructor(uint32_t priority, uint32_t flags);
void thread_pool_thread_pool_destructor(ThreadPool* thread_pool);

Job* thread_pool_job_constructor(void* (*run_job)(const void* input, void* output), void* (*free_data)(void* input, void* output), void* input, void* output, uint32_t flags);
void thread_pool_job_destructor(Job* job);

/**
 * @brief Inserts a pool into the routine of the thread executer
 * 
 * @param thread_executer 
 * @param thread_pool 
 */
 
void thread_pool_insert_pool(ThreadExecuter* thread_executer, ThreadPool* thread_pool);

/**
 * @brief adds a job to a thread pool which is in a thread executer
 * 
 * @param thread_executer Thread Executer to add to 
 * @param thread_pool_id ID of pool to be added to. If the pool if the pool id does not exist, the job addition will fail
 * @param job Job to be added 
 * @return int ID of the job, or a ThreadPoolErrors error code. In the failure case, the ownership of the job stays will the caller.
 *
 * This function adds a job into a pool in a thread executer. If the pool has a Pool asycnous flag, 
 * the job will be done accordind to fisrt in fist out. Otherwise, the job will be added into an addition queue, which will
 * be resolved at the start of the next thread execution group. 
 *
 *
 */
uint32_t thread_pool_insert_job(ThreadExecuter* thread_executer, uint32_t thread_pool_id, Job* job); // takes ownership of job 


/**
 * @brief remove job from thread executer
 * 
 * @param thread_executer 
 * @param thread_pool_id 
 * @param job_id 
 * @return int 0 if removal sucessfull, reffer to ThreadPoolError otherwise.
 */
uint32_t thread_pool_remove_job(ThreadExecuter* thread_executer, uint32_t thread_pool_id, uint32_t job_id); // takes ownership of job 

void thread_pool_dispatch();


#endif