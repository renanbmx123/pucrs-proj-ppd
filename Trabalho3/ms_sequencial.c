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
  }
  //for(i=0;i<l;i++)
    //print_line(c, m[i]);
  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Tempo de execucao: %f seconds\n", time_spent);
  free(m);
  return 0;
}


