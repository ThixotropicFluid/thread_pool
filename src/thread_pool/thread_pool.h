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

#ifdef _WIN32
    #include <pthreads-w32.h>  // For Windows
#else
    #include <pthread.h>       // For Unix-based systems (Linux, macOS, etc.)
#endif


/** @def THREAD_POOL_MAX_POOLS
 *  @brief Maximum number of thread pools supported.
 */
#define THREAD_POOL_MAX_POOLS 50
/** @def THREAD_POOL_MAX_THREADS
 *  @brief Maximum number of threads in a thread executer.
 */
#define THREAD_POOL_MAX_THREADS 256

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
    POOL_ID_DOES_NOT_EXIST = -1,
} ThreadPoolErrors;


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
    pthread_t threads[];
    struct ThreadPool* thread_pools[THREAD_POOL_MAX_POOLS];
} ThreadExecuter;
/**
 * @brief A collection of concuretly runable jobs
 * 
 */
typedef struct ThreadPool {
    //  Repseresnts a set of concourent tasks
    
    char* name; // optional name, for debugging purpouses
    int id; // Unique ID for inserting pool, up to THREAD_POOL_MAX_POOLS pools
    int pool_priority; // used by ThreadExecuter to serve next job
    int pool_flags; // flags used by jobs and 

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
    int flags;
} Job;


// public methods
/*!Constucts Thread Executer*/
ThreadExecuter* thread_pool_thread_executer_constructor();
void thread_pool_thread_executer_destructor(ThreadExecuter* thread_executer);

ThreadPool* thread_pool_thread_pool_constructor();
void thread_pool_thread_pool_destructor(ThreadPool* thread_pool);

Job* thread_pool_job_constructor();
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
 * @return int ID of the job, -1 if the job was failed to be added. In the failure case, the ownership of the job stays will the caller.
 *
 * This function adds a job into a pool in a thread executer. If the pool has a Pool asycnous flag, 
 * the job will be done accordind to fisrt in fist out. Otherwise, the job will be added into an addition queue, which will
 * be resolved at the start of the next thread execution group. 
 * The return of this fucntion will either be an error code, or 
 *
 *
 */
int thread_pool_insert_job(ThreadExecuter* thread_executer, int thread_pool_id, Job* job); // takes ownership of job 

// private methods
// TODO: move to .c file. 
void thread_pool_get_next_job(ThreadExecuter* thread_executer); // function to get the next job to be executed

#endif