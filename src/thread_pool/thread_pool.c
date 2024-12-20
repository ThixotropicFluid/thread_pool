#ifndef THREAD_POOL_C
#define THREAD_POOL_C

#include "thread_pool.h"
#include "stdio.h"
#include "stdlib.h"




// to be called by theads
void thread_pool_get_next_job(ThreadExecuter* thread_executer); // function to get the next job to be executed

Job* thread_pool_job_constructor(void* (*run_job)(const void* input, void* output), void* (*free_data)(void* input, void* output), void* input, void* output, int flags) {
    Job* job = malloc(sizeof(Job));
    job->run_job = run_job;
    job->free_data = free_data;
    job->input = input;
    job->output = output;
    job->flags = flags;
}
void thread_pool_job_destructor(Job* job) {
    if(job->free_data != NULL) {
        job->free_data(job->input, job->output);
    } else {
        free(job->input);
        free(job->output);
    }
}

#endif

