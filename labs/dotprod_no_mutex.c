#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define VECTOR_LEN 50000000
#define MAX_THREAD_COUNT 14

typedef struct 
{
  double *a;
  double *b;
  double sum; 
  int vector_len; 
} VECTOR_DATA;

// Thread and mutex data
VECTOR_DATA vector_data; 
pthread_t thread_list[MAX_THREAD_COUNT];
pthread_mutex_t mutex_sum;

// Dot product calculation
void *dotprod(void *arg)
{
  int i, start, end, len ;
  long offset;
  double mysum, *x, *y;
  offset = (long)arg;
    
  len = vector_data.vector_len;
  start = offset * len;
  end   = start + len;
  x = vector_data.a;
  y = vector_data.b;

  // Calculate dot product sum
  mysum = 0;
  for (i = start; i < end ; i++) 
  {
    mysum += (x[i] * y[i]);
  }
	vector_data.sum += mysum;
  // Lock and unlock mutex_sum to add current sum to overall sum
  printf("Thread %ld did %d to %d:  mysum=%f global sum=%f\n", offset, start, end, mysum, vector_data.sum);

  pthread_exit((void*) 0);
}

int main (int argc, char *argv[])
{
  long i;
  double *a, *b;
  void *status;
  pthread_attr_t thread_attr;
  int thread_count;

  if (argc < 2) {
  	printf("ERROR: Not enough threads provided!\n");
  	return 1;
  }

  thread_count = atoi(argv[1]);
  
  if (thread_count > MAX_THREAD_COUNT) {
  	printf("ERROR: Exceeded max thread count!\n");
  	return 1;
  } else if (thread_count < 1) {
  	printf("ERROR: Not enough threads allocated!\n");
  	return 1;
  }

  // Allocate vectors and initialize
  a = (double*) malloc (VECTOR_LEN * sizeof(double));
  b = (double*) malloc (VECTOR_LEN * sizeof(double));
  
  for (i = 0; i < VECTOR_LEN; i++) {
    a[i] = 1;
    b[i] = a[i];
  }

  // Assign allocated data
  vector_data.vector_len = VECTOR_LEN / thread_count; 
  vector_data.a = a; 
  vector_data.b = b; 
  vector_data.sum = 0;

  // Initialize mutex and thread
  pthread_mutex_init(&mutex_sum, NULL);
  pthread_attr_init(&thread_attr);
  pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);

  // Create threads
  for(i = 0; i < thread_count; i++)
  {
    pthread_create(&thread_list[i], &thread_attr, dotprod, (void *)i); 
  }

  pthread_attr_destroy(&thread_attr);

  // Wait for completion
  for(i = 0; i < thread_count; i++)
  {
    pthread_join(thread_list[i], &status);
  }

  // Print results and cleanup
  printf ("Sum =  %f \n", vector_data.sum);
  free (a);
  free (b);
  pthread_mutex_destroy(&mutex_sum);
  pthread_exit(NULL);
}   
