#include <stdio.h>
#include <stdlib.h> 

typedef struct
{ 
  double *a;
  double *b;
  double sum;
  int veclen;
} DOTDATA;

#define VECLEN 40000000
DOTDATA dotstr;

void dotprod (void *arg)
{
  int start, end, i;
  double mysum, *x, *y;
  long offset;

  offset = (long)arg;
  
  start = 0;
  end = dotstr.veclen;
  x = dotstr.a;
  y = dotstr.b;

  mysum = 0;
  for (i=start; i < end; i++)
  {
    mysum += (x[i] * y[i]);
  }

  dotstr.sum = mysum;
  printf("Thread %ld did %d to %d:  mysum=%f global sum=%f\n", offset, start, end, mysum, dotstr.sum);
}

int main (int argc, char *argv[])
{
  int i, len;
  double *a, *b;
  len = VECLEN;
  a = (double*) malloc (len* sizeof (double));
  b = (double*) malloc (len* sizeof (double));

  for (i=0; i < len; i++) {
    a[i] = 1;
    b[i] = a[i];
  }

  dotstr.veclen = len / 8;
  dotstr.a = a;
  dotstr.b = b;
  dotstr.sum = 0;

  for (int i = 0; i < 8; ++i) {
  	dotprod((void*)i);
  }

  printf("Sum = %f \n", dotstr.sum);
  free (a);
  free (b);
}
