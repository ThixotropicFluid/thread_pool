#ifndef THREAD_POOL_C
#define THREAD_POOL_C

#include "thread_pool.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"


void thread_pool_thread_executer();

// to be called by theads
void thread_pool_get_next_job(ThreadExecuter* thread_executer); // function to get the next job to be executed
void thread_pool_next_pool(); // function to find which pool needs to be executed next


ThreadExecuter* thread_pool_thread_executer_constructor() {
    ThreadExecuter thread_executer = malloc(sizeof(ThreadExecuter));
    //set flags so the executer doesn't get confused
    for(int i = 0; i < THREAD_POOL_MAX_POOLS; i ++) {
        thread_executer->thread_pools[i].flags = 0;
    }
}
void thread_pool_thread_executer_destructor(ThreadExecuter* thread_executer) {

}

ThreadPool* thread_pool_thread_pool_constructor(uint32_t priority, uint32_t flags) {
    ThreadPool* thread_pool = malloc(sizeof(ThreadPool));
    if(thread_pool == NULL) {
        return NULL;
    }
    thread_pool->id = 0;
    thread_pool->priority = priority;
    thread_pool->flags = flags;
    return thread_pool;
}
void thread_pool_thread_pool_destructor(ThreadPool* thread_pool) {
    if (thread_pool == NULL) {
        return;
    }
    linked_list_destructor(&thread_pool->jobs, &thread_pool_job_destructor);
    free(thread_pool);
}


Job* thread_pool_job_constructor(
        void* (*run_job)(const void* input, 
        void* output), void* (*free_data)(void* input, void* output), 
        void* input, 
        void* output, 
        uint32_t flags) {

    Job* job = malloc(sizeof(Job));
    if(job == NULL) {
        return NULL;
    }
    job->run_job = run_job;
    job->free_data = free_data;
    job->input = input;
    job->output = output;
    job->flags = flags;
    return job;
}
void thread_pool_job_destructor(Job* job) {
    if (job == NULL) {
        return;
    }
    if(job->free_data != NULL) {
        job->free_data(job->input, job->output);
    } else {
        free(job->input);
        free(job->output);
    }
    free(job);
}

#endif

