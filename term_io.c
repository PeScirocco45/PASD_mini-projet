# include <stdlib.h>
# include <ctype.h>
# include <assert.h>

# include "term_io.h"


/*!
 * To move pass spaces in a stream.
 * \param in input stream.
 */
static inline void skip_space ( FILE * in ) {
  char c ;
  while ( ( EOF != ( c = getc ( in ) ) )
	  &&  ( isspace ( c ) ) ) ;
  ungetc ( c , in ) ;
}


/*!
 * Initial size for scaning a symbol.
 */
# define SYMBOL_STRING_LENGHT_BASE 10

/*
 * Scan a sequence of non space and non parenthesis (symbol) of unbounded length.
 * if next non space char is '(' then it scans arguments.
 */
term term_scan ( FILE * in ) {
  term nouv = NULL ;
  sstring ss ;
  term * a = & nouv ;
  char c ;
  int cpt ;
  while ( EOF != ( c = getc ( in ) ) ) {
    //if ( c == ')' ) break ; //a voir quoi faire quand ')'
    cpt = 0 ;
    if ( ispunct ( c ) || isalnum ( c ) ) {
      if ( c == '(' || c == ')' ){  //ici on lance la creation des fils
        printf( "()" ) ;
        if ( c == '(' ){
            printf ( "aaaaaaa" ) ;
          term_add_argument_last ( * a , term_scan( in ) ) ;
        } else {
          printf ( "bbbbbbb" ) ;
          //a voir quoi faire quand ')'
        }
      } else {
        char * d = (char *)malloc(100 * sizeof(char)) ;
        char * pt = d ;
        do{
          printf( "%c" , c ) ;
          cpt++ ;
          * pt = c ;
          pt++ ;
          c = getc ( in ) ;
        }while ( c != ' ' && EOF != c ) ;
        ungetc ( c , in ) ;
        printf ( "%d" , cpt ) ;
        ss = sstring_create_string ( d ) ;
        nouv = term_create ( ss ) ;
        //* a = nouv ;
        //free( * d ) ;
        free ( d ) ;
      }
    } else {
      printf( "  " ) ;
    }
	}
	return nouv ;
}


/*!
 * To add spaces to a stream.
 * \param n half the number of spaces to add.
 * \param out output stream to print to.
 */
static inline void add_space_prefix ( int n ,
				      FILE * out ) {
  while ( 0 < n -- ) {
    fputs ( "  " , out ) ;
  }
}



/*!
 * Recursive function called by \c  term_print_expanded .
 * \param t (sub-)term to print
 * \param out output stream to print to.
 * \param depth nesting inside the main term. It is used to handle indentation.
 */
static void term_print_expanded_rec(term const t, FILE* const out, int const depth ) {
	// On affiche le term dans out
	sstring symbol = (term_get_symbol(t));
	int taille = sstring_get_length(symbol);
	for (int i = 0; i < taille; i++) {
		fputc(sstring_get_char(symbol, i), out);
	}
	/*
	 * Si le term a des arguments, on affiche " (\n" dans out
	 * On affiche (2 espaces) * (depth + 1) dans out avec la fonction add_space_prefix
	 * On creer un term_argument_traversal afin de parcourir la liste de ses arguments
	 * On rappel la fonction récursivement avec chacun de ses arguements en sautant des lignes
	 		et en indentant bien à chaque fois avec la fonction add_space_prefix
	*/
	if (term_get_arity(t) != 0) {
		fputs(" (\n", out);
		add_space_prefix(depth + 1, out);
		term_print_expanded_rec(term_get_argument(t, 0), out, depth + 1);
		term_argument_traversal arguments = term_argument_traversal_create(t);
		while (term_argument_traversal_has_next(arguments)) {
			fputs("\n", out);
			add_space_prefix(depth + 1, out);
			term_print_expanded_rec(term_argument_traversal_get_next(arguments), out, depth + 1);
		}
		// On saute une ligne, on indente, et on affiche ")" dans out
		fputs("\n", out);
		add_space_prefix(depth, out);
		fputs(")", out);
	}
}


void term_print_expanded(term t, FILE* out) {
  assert(NULL != t);
  assert(NULL != out);
  term_print_expanded_rec(t, out, 0);
}


/*!
 * Recursive function called by \c term_print_compact .
 * \param t (sub-)term to print
 * \param out output stream to print to.
 */
static void term_print_compact_rec(term const t, FILE* const out) {
	// On affiche le term dans out
	sstring symbol = (term_get_symbol(t));
	int taille = sstring_get_length(symbol);
	for (int i = 0; i < taille; i++) {
		fputc(sstring_get_char(symbol, i), out);
	}
	/*
	 * Si le term a des arguments, on affiche " ( " dans out 
	 * Puis on creer un term_argument_traversal afin de parcourir la liste de ses arguments
	 * On rappel la fonction récursivement avec chacun de ses arguements
	 */
	if (term_get_arity(t) != 0) {
		fputs(" ( ", out);
		term_print_compact_rec(term_get_argument(t, 0), out);
		term_argument_traversal arguments = term_argument_traversal_create(t);
		while (term_argument_traversal_has_next(arguments)) {
			fputs(" ", out);
			term_print_compact_rec(term_argument_traversal_get_next(arguments), out);
		}
		// On affiche " ) " dans out
		fputs(" )", out);
	}
}


void term_print_compact(term t, FILE* out) {
  assert(NULL != t);
  assert(NULL != out);
  term_print_compact_rec(t, out);
}