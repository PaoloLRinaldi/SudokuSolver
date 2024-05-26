#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

// CONTROLLO SU VABENE DIVERSO DA ZERO O 1
//
//
//
//
//

typedef struct info_cell {
  int val;
  int *poss;
  int nposs;
} cell;

// void zerolattice(int **sudoku, int L);
void zerolattice(cell **sudoku, int L);

void checkinput(int *vabene);
void getsize(int *L);

// void printlattice( int **sudoku, int L);
void printlattice(cell **sudoku, int L);

// void getlattice( int **sudoku, int L);
void getlattice(cell **sudoku, int L);

// void beginlattice(int **sudoku, int L);
void beginlattice(cell **sudoku, int L);

int checksize(int L);

void init_sudoku(cell ***sudoku, int L);
void init_ro_co_sq(cell **sudoku, cell ****rows, cell ****cols, cell ****sqrs, int L);
int check_doubles(cell ***rows, cell ***cols, cell ***sqrs, int L);
int doubles_in(cell **list, int L);
void free_everything(cell **sudoku, cell ***rows, cell ***cols, cell ***sqrs, int L);

int main(){

  int L, i, j;
  // int **sudoku;
  cell **sudoku;
  // I prossimo sono array di array di puntatori, leggi l'esempio per capire.
  // Ad esempio se volgio accedere al valore del terzo elemento
  // della seconda colonna farò:
  // cols[2][3]->val
  cell ***rows, ***cols, ***sqrs;

  getsize( &L);

  //Si inizializza la tabella principale
  init_sudoku(&sudoku, L);

  //Si inizializzano righe, colonne e quadrati
  init_ro_co_sq(sudoku, &rows, &cols, &sqrs, L);
  
  //Si crea la tabella principale
  do {
    beginlattice(sudoku, L);
  } while (!check_doubles(rows, cols, sqrs, L));

  free_everything(sudoku, rows, cols, sqrs, L);
  puts("Tutto fatto chicco!\n");
  exit(0);
}



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
void beginlattice(cell **sudoku, int L){
  zerolattice(sudoku, L);
  int vabene = 1;
  do{
    getlattice(sudoku, L);
    printlattice(sudoku, L);
    checkinput(&vabene);
    if ( vabene == 0 ) printf("Reinseriee il Sudoku\n");
  } while ( vabene == 0);
}


void checkinput(int *vabene){
  printf("E' il sudoku giusto?\n SI ------> 1 \t NO ------> 0\n");
  scanf("%d", vabene);
}

void getsize( int* L){
  // PER NON PASSARE PUNTATORE A CHECKSIZE
  do{
    printf("Inserire lato del quadrato. Deve essere un quadrato perfetto\n");
    scanf("%d", L);
  } while ( *L <= 0 );
  //printf("Il sudoku è un quadrato di lato\t%d\t con\t %d\t numeri per uqadrato\n", L*L, L*L));
}

// void printlattice( int **sudoku, int L){
void printlattice(cell **sudoku, int L){
  int i, j;
  for( i = 0 ; i < L ; i++){
    for ( j = 0 ; j< L ; j++){
      // printf("\t%d", sudoku[i][j]);
      printf("\t%d", sudoku[i][j].val);
    }
    printf("\n");
  }

}

// void getlattice( int **sudoku, int L){
void getlattice(cell **sudoku, int L){
  int i, j;
    printf("Inserire i numeri iniziali > 0 e minori di %d\n", L);
    printf("Inserire 0 in caso di numero assente\n");
    for(i = 0 ; i < L ; i++){
      for ( j = 0 ; j< L ; j++){
        do{
          // scanf("%d", &sudoku[i][j]);
          scanf("%d", &sudoku[i][j].val);
          // if ( sudoku[i][j] > L || sudoku[i][j] < 0 ) printf("Reinserire il numero\n");
          if ( sudoku[i][j].val > L || sudoku[i][j].val < 0 ) printf("Reinserire il numero\n");
        // } while ( sudoku[i][j] > L || sudoku[i][j] < 0 );
        } while ( sudoku[i][j].val > L || sudoku[i][j].val < 0 );
      }
    }

}

int checksize(int L){
  int i, answer ;
  double p;
  if ( L < 0 ) return 0 ;
  p = sqrt(L);

  for( i = 0 ; i < p + 1 ; i++){
    answer = L % i ;
    if ( answer == 0 ) return 0 ;
  }
  return 1 ;
}

void init_sudoku(cell ***sudoku, int L) {
  int i, j;
  // sudoku = (int**) malloc( sizeof(int*) * L);
  *sudoku = (cell **) malloc(sizeof(cell *) * L);

  for ( i = 0 ; i < L ; i++){
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