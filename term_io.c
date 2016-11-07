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
  sstring ss = sstring_create_empty() ;
  sstring temp = sstring_create_empty() ;
  term * a = & nouv ;
  char c ;
  // Le in fait office de while grace au getc
  // Tant qu'on est pas à la fin du fichier
  if ( EOF != ( c = getc ( in ) ) ) {
    // Si ce n'est pas '(' ou ')' on a un term, sinon des arguments
    if ( !( c == '(' ) && !( c == ')') ) {
      do{
        // On vérifie si le caractere n'est pas un retour chariot (utile dans term_variable)
        if ( ! ( c == '\n' ) ) {
          temp = sstring_create_string ( &c ) ;
          sstring_concatenate(ss,temp);
        }
      }while ( ' ' != ( c = getc ( in ) ) && EOF != c ) ;
      ungetc ( c , in ) ;
      nouv = term_create ( ss ) ;
      skip_space ( in ) ;
      sstring_destroy ( & ss ) ;
      sstring_destroy ( & temp ) ;
    }
    // Si '(' les arguments commencent ici
    if ( ( c = getc ( in ) ) == '(' ) {
      // On parcours in tant que ')', sinon on crée un ou des argument(s)
      while ( EOF != ( c = getc ( in ) ) ) {
        if (c == ')' ) {
          skip_space ( in ) ;
          break ;
        }
        ungetc ( c , in ) ;
        skip_space ( in ) ;
        term_add_argument_last ( * a , term_scan( in ) ) ;
      }
    } else {
      ungetc ( c , in ) ;
      skip_space ( in ) ;
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
	 * On crée un term_argument_traversal afin de parcourir la liste de ses arguments
	 * On rappelle la fonction récursivement avec chacun de ses arguments en sautant des lignes
	 		et en indentant bien à chaque fois avec la fonction add_space_prefix
	*/
	if (term_get_arity(t) != 0) {
		fputs(" (\n", out);
		term_argument_traversal arguments = term_argument_traversal_create(t);
		while (term_argument_traversal_has_next(arguments)) {
			add_space_prefix(depth + 1, out);
			term_print_expanded_rec(term_argument_traversal_get_next(arguments), out, depth + 1);
		}
		// On saute une ligne, on indente, et on affiche ")" dans out
		add_space_prefix(depth, out);
		fputs(")", out);
	}
	printf("\n");
}


void term_print_expanded(term t, FILE* out) {
  assert(NULL != t);
  assert(NULL != out);
  term_print_expanded_rec(t, out, 0);
}


/*
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
	 * Si le term a des arguments, on affiche " (" dans out
	 * Puis on crée un term_argument_traversal afin de parcourir la liste de ses arguments
	 * On rappelle la fonction récursivement avec chacun de ses arguements
	 */
	if (term_get_arity(t) != 0) {
		fputs(" (", out);
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
