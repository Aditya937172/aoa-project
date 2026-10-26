#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char id[32];
    int deadline;
    int profit;
} Job;

static int compare_jobs(const void *a, const void *b) {
    const Job *job_a = (const Job *)a;
    const Job *job_b = (const Job *)b;

    if (job_a->profit != job_b->profit) {
        return job_b->profit - job_a->profit;
    }
    if (job_a->deadline != job_b->deadline) {
        return job_a->deadline - job_b->deadline;
    }
    return strcmp(job_a->id, job_b->id);
}

/*
 * Greedily schedules jobs in descending order of profit.
 * Each job takes one unit of time and must finish by its deadline.
 */
void runSchedule(Job jobs[], int number_of_jobs) {
    int max_deadline = 0;
    int total_profit = 0;
    int scheduled_jobs = 0;

    for (int i = 0; i < number_of_jobs; ++i) {
        if (jobs[i].deadline > max_deadline) {
            max_deadline = jobs[i].deadline;
        }
    }

    int *slots = calloc((size_t)max_deadline + 1, sizeof(int));
    if (slots == NULL) {
        fprintf(stderr, "Error: unable to allocate scheduling slots.\n");
        return;
    }

    /* slots[t] stores the index of the job scheduled at time t, or -1. */
    for (int t = 0; t <= max_deadline; ++t) {
        slots[t] = -1;
    }

    qsort(jobs, (size_t)number_of_jobs, sizeof(Job), compare_jobs);

    for (int i = 0; i < number_of_jobs; ++i) {
        int last_slot = jobs[i].deadline < max_deadline
                             ? jobs[i].deadline
                             : max_deadline;

        for (int slot = last_slot; slot >= 1; --slot) {
            if (slots[slot] == -1) {
                slots[slot] = i;
                total_profit += jobs[i].profit;
                ++scheduled_jobs;
                break;
            }
        }
    }

    printf("\nJob sequence to follow:\n");
    for (int slot = 1; slot <= max_deadline; ++slot) {
        if (slots[slot] != -1) {
            printf("Time %d: %s (deadline %d, profit %d)\n",
                   slot,
                   jobs[slots[slot]].id,
                   jobs[slots[slot]].deadline,
                   jobs[slots[slot]].profit);
        }
    }

    printf("\nScheduled jobs: %d\n", scheduled_jobs);
    printf("Maximum total profit: %d\n", total_profit);

    free(slots);
}

int main(void) {
    int number_of_jobs;

    printf("Enter the number of jobs: ");
    if (scanf("%d", &number_of_jobs) != 1 || number_of_jobs <= 0) {
        fprintf(stderr, "Error: number of jobs must be a positive integer.\n");
        return EXIT_FAILURE;
    }

    Job *jobs = malloc((size_t)number_of_jobs * sizeof(Job));
    if (jobs == NULL) {
        fprintf(stderr, "Error: unable to allocate memory for jobs.\n");
        return EXIT_FAILURE;
    }

    printf("Enter each job as: job_id deadline profit\n");
    for (int i = 0; i < number_of_jobs; ++i) {
        if (scanf("%31s %d %d", jobs[i].id, &jobs[i].deadline, &jobs[i].profit) != 3 ||
            jobs[i].deadline <= 0 || jobs[i].profit < 0) {
            fprintf(stderr,
                    "Error: each job needs an ID, a positive deadline, "
                    "and a non-negative profit.\n");
            free(jobs);
            return EXIT_FAILURE;
        }
    }

    runSchedule(jobs, number_of_jobs);
    free(jobs);
    return EXIT_SUCCESS;
}
