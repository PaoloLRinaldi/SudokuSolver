#include "function.h"
#include <assert.h>

int (*techniques[])(cell ***, cell ***, cell ***, int L) = {clean_all_poss};

// void zerolattice(int **sudoku, int L){
void zerolattice(cell **sudoku, int L){
  int i, j, k;
  for ( i= 0 ; i< L ; i ++){
    for ( j = 0 ; j< L ; j++){
      // sudoku[i][j] = 0;
      sudoku[i][j].val = 0;
      // All'inizio ogni cella può essere
      // qualsiasi numero
      sudoku[i][j].nposs = L;
      for (k = 0; k < L; k++){
        sudoku[i][j].poss[k] = k + 1;
      }
    }
  }
}

// void beginlattice(int **sudoku, int L){
void beginlattice(cell **sudoku, int L) {
  zerolattice(sudoku, L);
  int vabene = 1;
  do {
    getlattice(sudoku, L);
    printlattice(sudoku, L);
    checkinput(&vabene);
    if ( vabene == 0 ) printf("Reinserie il Sudoku\n");
  } while ( vabene == 0);
}


void scansudoku(cell **sudoku, int L, char *name) {
  FILE *fname;
  fname = fopen(name, "r");
  int i, j;
  for (i = 0 ; i < L; i++) {
    for (j = 0; j < L; j++) {
      do {
        fscanf(fname, "%d", &sudoku[i][j].val);
        if (sudoku[i][j].val > L || sudoku[i][j].val < 0)
          printf("Valore da file errato\n");
      } while (sudoku[i][j].val > L || sudoku[i][j].val < 0);
    }
  }
}





void checkinput(int *vabene) {
  printf("E' il sudoku giusto?\n SI ------> 1 \t NO ------> 0\n");
  scanf("%d", vabene);
}

void getsize(int* L) {
  // PER NON PASSARE PUNTATORE A CHECKSIZE
  do {
    printf("Inserire lato del quadrato. Deve essere un quadrato perfetto\n");
    scanf("%d", L);
  } while ( *L <= 0 );
  //printf("Il sudoku è un quadrato di lato\t%d\t con\t %d\t numeri per uqadrato\n", L*L, L*L));
}

// void printlattice( int **sudoku, int L){
void printlattice(cell **sudoku, int L) {
  int i, j;
  for (i = 0 ; i < L; i++) {
    for (j = 0; j < L; j++) {
      // printf("\t%d", sudoku[i][j]);
      if (sudoku[i][j].val)
        printf("\t%d", sudoku[i][j].val);
      else
        printf("\t ");
    }
    printf("\n");
  }
}

// void getlattice( int **sudoku, int L){
void getlattice(cell **sudoku, int L) {
  int i, j;
  printf("Inserire i numeri iniziali > 0 e minori di %d\n", L);
  printf("Inserire 0 in caso di numero assente\n");
  for (i = 0 ; i < L ; i++) {
    for (j = 0 ; j < L ; j++) {
      do {
        // scanf("%d", &sudoku[i][j]);
        scanf("%d", &sudoku[i][j].val);
        // if ( sudoku[i][j] > L || sudoku[i][j] < 0 ) printf("Reinserire il numero\n");
        if (sudoku[i][j].val > L || sudoku[i][j].val < 0)
          printf("Reinserire il numero\n");
        // } while ( sudoku[i][j] > L || sudoku[i][j] < 0 );
      } while (sudoku[i][j].val > L || sudoku[i][j].val < 0);
    }
  }
}

int checksize(int L) {
  int i, answer;
  double p;
  if (L < 0) return 0;
  p = sqrt(L);

  for (i = 0; i < p + 1; i++) {
    answer = L % i;
    if (answer == 0) return 0;
  }
  return 1;
}

void init_sudoku(cell ***sudoku, int L) {
  int i, j;
  // sudoku = (int**) malloc( sizeof(int*) * L);
  *sudoku = (cell **) malloc(sizeof(cell *) * L);

  for (i = 0; i < L; i++) {
    // sudoku[i] = (int*) malloc(sizeof(int));
    (*sudoku)[i] = (cell *) malloc(L * sizeof(cell));
    for (j = 0; j < L; j++)
      (*sudoku)[i][j].poss = (int *) malloc(L * sizeof(int));
  }
}

void init_ro_co_sq(cell **sudoku, cell ****rows, cell ****cols, cell ****sqrs, int L) {
  int i, j;
  int l = (int) sqrt(L);

  // Alloco
  *rows = (cell ***) malloc(L * sizeof(cell **));
  *cols = (cell ***) malloc(L * sizeof(cell **));
  *sqrs = (cell ***) malloc(L * sizeof(cell **));
  for (i = 0; i < L; i++) {
    (*rows)[i] = (cell **) malloc(L * sizeof(cell*));
    (*cols)[i] = (cell **) malloc(L * sizeof(cell*));
    (*sqrs)[i] = (cell **) malloc(L * sizeof(cell*));
  }

  // Assegno i valori iniziali
  for (i = 0; i < L; i++) {
     for (j = 0; j < L; j++) {
        (*rows)[i][j] = &sudoku[i][j];
        (*cols)[i][j] = &sudoku[j][i];
        (*sqrs)[i][j] = &sudoku[(i / l) * l + (j / l)][(i % l) * l + (j % l)];
     }
  }
}
// RITORNA 1 SE C'È UN DOPPIONE
int doubles_in(cell **list, int L) {
  int a, i, j;

  for (i = 0; i < L; i++) {
    a = list[i]->val;
    // Non voglio controllare che lo 0 abbia doppioni
    if (!a) continue;
    for (j = i + 1; j < L; j++) {
      if (a == list[j]->val) return 1;
    }
  }

  return 0;
}

int check_doubles(cell ***rows, cell ***cols, cell ***sqrs, int L) {
  int i;
  static char *not_good = "La tabella inserita contiene più elementi "
                          "all'interno della stessa riga/colonna/quadrato!\n";
  for (i = 0; i < L; i++) {
    if (doubles_in(rows[i], L) ||
        doubles_in(cols[i], L) ||
        doubles_in(sqrs[i], L)) {
      puts(not_good);
      return 0;
    }
  }

  return 1;
}

void free_everything(cell **sudoku, cell ***rows, cell ***cols, cell ***sqrs, int L) {
  int i, j;

  for (i = 0; i < L; i++) {
     for (j = 0; j < L; j++) {
       free(sudoku[i][j].poss);
     }
     free(sudoku[i]);
     free(rows[i]);
     free(cols[i]);
     free(sqrs[i]);
  }
  free(sudoku);
  free(rows);
  free(cols);
  free(sqrs);
}


int toggle_poss_cell(cell *a, int n) {
  if (a->val) return 0;
  int i;

  for (i = 0; i < a->nposs; i++) {
    if (a->poss[i] == n) {
      a->nposs--;
      a->poss[i] = a->poss[a->nposs];
      return 1;
    }
  }
  return 0;
}

int toggle_poss_list(cell **list, int n, int L) {
  int i, done = 0;
  for (i = 0; i < L; i++) {
    done |= toggle_poss_cell(list[i], n);
  }
  return done;
}

int clean_list_poss(cell **list, int L) {
  int i, done = 0;

  for (i = 0; i < L; i++) {
    if (!list[i]->val) continue;
    done |= toggle_poss_list(list, list[i]->val, L);
  }
  return done;
}

int clean_all_poss(cell ***rows, cell ***cols, cell ***sqrs, int L) {
  int i, done = 0;
  for (i = 0; i < L; i++) {
    done |= clean_list_poss(rows[i], L);
    done |= clean_list_poss(cols[i], L);
    done |= clean_list_poss(sqrs[i], L);
  }
  return done;
}

int is_possible(cell *a, int n) {
  int i;
  for (i = 0; i < a->nposs; i++)
    if (a->poss[i] == n) return 1;
  return 0;
}


int is_n_once_in_list(cell **list, int n, int L) {
  int index = -1, i;
  for (i = 0; i < L; i++) {
    if (list[i]->val) {
      if (list[i]->val == n) return 0;
      continue;
    }
    if (is_possible(list[i], n)) {
      if (index != -1) return 0;
      index = i;
    }
  }
  if (index == -1) return 0;
  list[index]->val = n;
  return 1;
}

int any_once_in_list(cell **list, int L) {
  int i;
  for (i = 0; i < L; i++) {
    if (is_n_once_in_list(list, i + 1, L)) return 1;
  }
  return 0;
}

int any_once(cell ***rows, cell ***cols, cell ***sqrs, int L) {
  int i;
  for (i = 0; i < L; i++) {
    if (any_once_in_list(rows[i], L)) return 1;
    if (any_once_in_list(cols[i], L)) return 1;
    if (any_once_in_list(sqrs[i], L)) return 1;
  }
  return 0;
}

int alone_poss(cell *a) {
  if (!a->val && a->nposs == 1) {
    a->val = a->poss[0];
    return 1;
  }
  return 0;
}

int any_alone_in_list(cell **list, int L) {
  int i;
  for (i = 0; i < L; i++) {
    if(alone_poss(list[i])) return 1;
  }
  return 0;
}

int any_alone(cell ***lists, int L) {
  int i;
  for (i = 0; i < L; i++) {
    if (any_alone_in_list(lists[i], L)) return 1;
  }
  return 0;
}

int assign_values(cell ***rows, cell ***cols, cell ***sqrs, int L) {
  if (any_once(rows, cols, sqrs, L)) return 1;
  if (any_alone(rows, L)) return 1;
  return 0;
}

int is_list_complete(cell **list, int L) {
  int i;
  for (i = 0; i < L; i++) {
    if (!list[i]->val) return 0;
  }
  return 1;
}

int is_sudoku_complete(cell ***lists, int L) {
  int i;
  for (i = 0; i < L; i++) {
    if (!is_list_complete(lists[i], L)) return 0;
  }
  return 1;
}

int blank_cells_left(cell ***lists, int L) {
  int i, j, count = 0;;
  for (i = 0; i < L; i++) {
    for (j = 0; j < L; j++) {
      if (!lists[i][j]->val) ++count;
    }
  }
  return count;
}

int solve_it(rec **instances, int depth, int L) {
  int i = 0, tech_len = sizeof(techniques) / sizeof(techniques[0]), flag;
  while (i < tech_len) {
    flag = techniques[i]((*instances)[depth].rows, (*instances)[depth].cols, (*instances)[depth].sqrs, L);
    flag |= assign_values((*instances)[depth].rows, (*instances)[depth].cols, (*instances)[depth].sqrs, L);
    i = flag ? 0 : i + 1;
  }
  if (is_sudoku_complete((*instances)[depth].rows, L)) return 1;
  if (!is_solvable((*instances)[depth].rows, (*instances)[depth].cols, (*instances)[depth].sqrs, L)) return 0;
  return trial_moves(instances, depth, L);
}

int empty_cell(cell *a) {
  return a->val == 0 && a->nposs == 0;
}

int empty_list(cell **list, int L) {
  int i;
  for (i = 0; i < L; i++)
    if (empty_cell(list[i])) return 1;
  return 0;
}

int something_empty(cell ***lists, int L) {
  int i;
  for (i = 0; i < L; i++)
    if (empty_list(lists[i], L)) return 1;
  return 0;
}

int is_n_in_list(cell **list, int n, int L) {
  int i;
  for (i = 0; i < L; i++)
    if (list[i]->val == n || (list[i]->val == 0 && is_possible(list[i], n)))
      return 1;
  return 0;
}

int completable_list(cell **list, int L) {
  int i;
  for (i = 0; i < L; i++)
    if (!is_n_in_list(list, i + 1, L)) return 0;
  return 1;
}

int completable_sudoku(cell ***rows, cell ***cols, cell ***sqrs, int L) {
  int i;
  for (i = 0; i < L; i++) {
    if (!completable_list(rows[i], L)) return 0;
    if (!completable_list(cols[i], L)) return 0;
    if (!completable_list(sqrs[i], L)) return 0;
  }
  return 1;
}

int is_solvable(cell ***rows, cell ***cols, cell ***sqrs, int L) {
  if (something_empty(rows, L) || !completable_sudoku(rows, cols, sqrs, L))
    return 0;
  return 1;
}

int trial_moves(rec **instances, int depth, int L) {
  int i, j, k, n;
  for (n = 2; n < L + 1; n++) {  // Cerco prima le caselle con 2 possibilità
                                 // e se non le trovo aumento
    for (i = 0; i < L; i++) {
      for (j = 0; j < L; j++) {
        if (!(*instances)[depth].rows[i][j]->val && (*instances)[depth].rows[i][j]->nposs == n) {
          for (k = 0; k < n; k++) {
            next_rec(instances, depth, L);
            (*instances)[depth + 1].rows[i][j]->val = (*instances)[depth + 1].rows[i][j]->poss[k];
            if (solve_it(instances, depth + 1, L)) {
              return 1;  // E' stata trovata una soluzione definitiva
            }
          }
          return 0;  // Quindi se nessuna delle n possibilità
                     // va bene dì "al piano di sopra" che
                     // non va bene
        }
      }
    }
  }
}

void init_rec(rec *instance, int L) {
  init_sudoku(&instance->sudoku, L);
  init_ro_co_sq(instance->sudoku, &instance->rows, &instance->cols, &instance->sqrs, L);
  instance->there_is_next = 0;
}

void next_rec(rec **instances, int depth, int L) {
  if (!(*instances)[depth].there_is_next) {
    (*instances)[depth].there_is_next = 1;
    *instances = (rec *) realloc(*instances, (depth + 2) * sizeof(rec));
    init_rec(&(*instances)[depth + 1], L);
  }
  copy_rec(&(*instances)[depth], &(*instances)[depth + 1], L);
}

void copy_cell(cell *a1, cell *a2) {
  a2->val = a1->val;
  if (a1->val) return;
  int i;
  a2->nposs = a1->nposs;
  for (i = 0; i < a2->nposs; i++)
    a2->poss[i] = a1->poss[i];
}

void copy_rec(rec *inst1, rec *inst2, int L) {
  int i, j;
  for (i = 0; i < L; i++)
    for (j = 0; j < L; j++)
      copy_cell(&inst1->sudoku[i][j], &inst2->sudoku[i][j]);
}

void free_rec(rec *insts, int L) {
  int i = 0;
  while (i >= 0) {
    if (insts[i].there_is_next) {
      insts[i++].there_is_next = 0;
      continue;
    }
    free_everything(insts[i].sudoku, insts[i].rows, insts[i].cols, insts[i].sqrs, L);
    i--;
  }
  free(insts);
}

void get_solution(rec *instances, int L) {
  int i = 0;
  while (1) {
    if (is_sudoku_complete(instances[i].rows, L)) {
      if (i != 0) copy_rec(&instances[i], &instances[0], L);
      return;
    }
    i++;
  }
}


void debug_print_poss_list(cell **list, int L) {
  int i, j;
  for (i = 0; i < L; i++) {
    if (!list[i]->val) {
      printf(" \t->");
      for (j = 0; j < list[i]->nposs; j++)
        printf("\t%d", list[i]->poss[j]);
      printf("\n");
    } else {
      printf("%d\n", list[i]->val);
    }
  }
}


void debug_print_tab_rows(cell ***rows, int L) {
  int i, j;
  for (i = 0; i < L; i++) {
    for (j = 0; j < L; j++) {
      if (rows[i][j]->val)
        printf("\t%d", rows[i][j]->val);
      else
        printf("\t ");
    }
    printf("\n");
  }
}
