#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

void print_line(int c, int* line){
        int i;
        for(i=0;i<c;i++)
                printf("%d ", line[i]);
        printf("\n");
}

void bubble_sort (int *a, int n) {
    int i, t, j = n, s = 1;
    while (s) {
        s = 0;
        for (i = 1; i < j; i++) {
            if (a[i] < a[i - 1]) {
                t = a[i];
                a[i] = a[i - 1];
                a[i - 1] = t;
                s = 1;
            }
        }
        j--;
    }
}

int main(int argc, char** argv)
  {
  int i, j;
  int l=1000, c=100000;
  int (*m)[c] = malloc (l * sizeof *m);
  clock_t begin = clock();
  double time_spent;
  clock_t end;
  for(i=0;i<l;i++){
    for(j=0;j<c;j++){
        m[i][j]=c-j;
    }
    //print_line(c, m[i]);
  }
  for(i=0;i<l;i++){
        qsort(m[i], c, sizeof(int), cmpfunc);
		bubble_sort(m[i],c);
  }
  //for(i=0;i<l;i++)
    //print_line(c, m[i]);
  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Tempo de execucao: %f seconds\n", time_spent);
  free(m);
  return 0;
}


