//AGGIUNGIAMO UN COMMENTO AD OGNI FUNZIONE
#ifndef FUNCTION_H
#define FUNCTION_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct info_cell {
  int val;
  int *poss;
  int nposs;
} cell;

typedef struct Recursive {
  cell **sudoku;
  cell ***rows;
  cell ***cols;
  cell ***sqrs;
  int there_is_next;
} rec;

void scansudoku(cell **sudoku, int L, char *name);

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

// Alloca lo spazio per la tabella
void init_sudoku(cell ***sudoku, int L);

// Alloca lo spazio per righe, colonne e quadrati e gli
// assegna i relativi valori della tabella
void init_ro_co_sq(cell **sudoku, cell ****rows, cell ****cols, cell ****sqrs, int L);

// Dice se nella tabella ci sono doppioni
int check_doubles(cell ***rows, cell ***cols, cell ***sqrs, int L);

// Dice se nella lista ci sono doppioni
int doubles_in(cell **list, int L);

// Libera lo spazio occupato
void free_everything(cell **sudoku, cell ***rows, cell ***cols, cell ***sqrs, int L);

void init_rec(rec *instance, int L);

void next_rec(rec **instances, int depth, int L);

void copy_cell(cell *a1, cell *a2);

void copy_rec(rec *inst1, rec *inst2, int L);

void free_rec(rec *insts, int L);

void get_solution(rec *instances, int L);

/************************************************************
 * Funzioni che modificano i valori possibili delle caselle *
 ************************************************************/

// Data una cella a, toglie dai possibili valori il valore n.
// Se lo ha effettivamente tolto (e quindi non c'era un
// valore definitivo già assegnato e n era effetivamente tra
// i valori possibili di quella cella) restituisce 1, se no 0.
int toggle_poss_cell(cell *a, int n);

// Data una lista list, toglie dai possibili valori di tutte
// le celle il valore n. Se ha effettivamente tolto n da
// qualche cella restituisce 1, se no 0;
int toggle_poss_list(cell **list, int n, int L);

// Data una lista list, ripulisce tutti i possibili valori
// di tutte le celle basandosi sui valori inseriti sulla
// lista stessa. Restituisce 1 o 0.
int clean_list_poss(cell **list, int L);

// Ripulisce i valori possibili di tutta la tabella. Se ha
// aggiornato qualcosa restituisce 1, se no 0;
int clean_all_poss(cell ***rows, cell ***cols, cell ***sqrs, int L);

/***********************************************************
 * Funzioni che assegnano i valori definitivi alle caselle *
 ***********************************************************/

// Dice se un determinato valore n è tra quelli possibili di
// una determinata cella a
int is_possible(cell *a, int n);

// Dice se il valore n è o no comparso una sola volta nella lista
// list. In caso affermativo assegna il valore n all'unica
// casella in cui quel valore è possibile.
int is_n_once_in_list(cell **list, int n, int L);

// Dice se in una lista sono stati assegnati dei numeri
// perché unici
int any_once_in_list(cell **list, int L);

// Dice se in tutta la tabella sono stati assegnati dei numeri
// perché unici
int any_once(cell ***rows, cell ***cols, cell ***sqrs, int L);

// Dice se nella cella a c'è un solo valore possibile; se
// sì assegna alla cella quel valore
int alone_poss(cell *a);

// Dice se in una lista ci sono celle con una sola possibilità.
// Se sì le cambia
int any_alone_in_list(cell **list, int L);

// Dice se in tutta la tabella sono stati assegnati dei numeri
// perchè erano l'unica possibilità
int any_alone(cell ***rows, int L);

// Assegna i valori alle caselle (o se sono possibilità che
// compaiono una sola volta in una lista o se ci sono caselle
// con una sola possibilità). Se assegna qualcosa restituisce 1,
// altrimenti 0.
int assign_values(cell ***rows, cell ***cols, cell ***sqrs, int L);

/****************
 * Check finali *
 ****************/

// Dice se sono stati inseriti tutti i numeri all'interno
// di una lista
int is_list_complete(cell **list, int L);

// Dice se sono stati inseriti tutti i numeri all'interno
// dell'intera tabella
int is_sudoku_complete(cell ***lists, int L);

// Ti dice quante caselle bianche sono rimaste in
// totale.
int blank_cells_left(cell ***lists, int L);

/***************
 * Risoluzione *
 ***************/

// Dice se lo ha risolto
int solve_it(rec **instances, int depth, int L);

int trial_moves(rec **instances, int depth, int L);

/***********************************************
 * Check per vedere se il puzzle è risolvibile *
 ***********************************************/

int empty_cell(cell *a);

int empty_list(cell **list, int L);

int something_empty(cell ***lists, int L);

int is_n_in_list(cell **list, int n, int L);

int completable_list(cell **list, int L);

int completable_sudoku(cell ***rows, cell ***cols, cell ***sqrs, int L);

int is_solvable(cell ***rows, cell ***cols, cell ***sqrs, int L);

/********************
 * Per il debugging *
 ********************/

void debug_print_poss_list(cell **list, int L);

void debug_print_tab_rows(cell ***rows, int L);

#endif  // FUNCTION_H
