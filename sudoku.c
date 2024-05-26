#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "function.h"

// CONTROLLO SU VABENE DIVERSO DA ZERO O 1
//
//
//
//
//

int main(int argc, char *argv[]) {
  int L;
  rec *instances = 0;

  if (argc == 1) {
    puts("L'argomento deve essere 0 o 1");
    exit(1);
  }
  
  if (argv[1][0] == '0') {
    getsize(&L);
    // Si inizializzano tabella principale, righem colonne e quadrati
    instances = (rec *) malloc(1 * sizeof(rec));
    init_rec(&instances[0], L);

    //Si crea la tabella principale
    do {
      beginlattice(instances[0].sudoku, L);
    } while (!check_doubles(instances[0].rows, instances[0].cols, instances[0].sqrs, L));
  } else if (argv[1][0]) {
    L = argv[2][0];
    init_sudoku(&instances[0].sudoku, L);
    do {
      scansudoku(instances[0].sudoku, L, argv[1][3]);
    } while (!check_doubles(instances[0].rows, instances[0].cols, instances[0].sqrs, L));
  }


  if (!solve_it(&instances, 0, L)) {
    puts("La tabella non è risolvibile.\n");
  } else {
    get_solution(instances, L);
    puts("Tabella completata!\n");
    printlattice(instances[0].sudoku, L);
  }
  if (!check_doubles(instances[0].rows, instances[0].cols, instances[0].sqrs, L)) {
    puts("Ooops, sembra che invece non vada bene!\n");
  } else {
    puts("Ho ricontrollato, è giusto!\n");
  }
  free_rec(instances, L);
  puts("Tutto fatto chicco!\n");
  exit(0);
}
