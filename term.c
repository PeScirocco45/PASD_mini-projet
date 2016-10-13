# include <stdlib.h>
# include <ctype.h>
# include <assert.h>
 
# include "term.h"

# undef NDEBUG   // FORCE ASSERT ACTIVATION


/*!
 * This structure is used to have a both way linked list of terms.
 * It is used to record the arguments of terms.
 * It is linked in both way to ensure simple navigation.
 */
typedef struct term_list_struct {
  /*! Current term / argument */
  term t ;
  /*! Link to previous term / argument */
  struct term_list_struct * previous ;
  /*! Link to next term / argument */
  struct term_list_struct * next ;
} * term_list ;


/*!
 * This structure is used to record a term.
 * All sub-term / arguments are stored in a both way linked list of terms.
 * Arguments can be accessed from both side.
 */
typedef struct term_struct {
  /*! Symbol of the term */
  sstring symbol ;
  /*! Number of arguments, stored for efficiency */
  int arity ;
  /*! Father term if set then this term is an argument of the father. */
  term father ;
  /*! First argument in the first link of the doubled chain. */
  term_list argument_first ;
  /*! Last argument in the first link of the doubled chain. */
  term_list argument_last ;
} term_struct ;


/*!
 * To test whether a \c sstring is a valid symbol. 
 * This means not empty, no space nor parenthesis.
 * \param symbol sstring to test the validity as an symbol.
 * \return true if \c symbol is valid as a term symbol.
 */
static bool symbol_is_valild ( sstring const symbol ) { 
  return ((symbol != "0") && (symbol != " ") && (symbol != "(") && (symbol != ")"));
}


term term_create ( sstring symbol ) { 
  assert(! sstring_is_empty(symbol));
  assert (symbol_is_valild(symbol));
  term t = (term)malloc(sizeof(struct term_struct));
  t->symbol = symbol;
  t->arity = 0;
  t->father = NULL;
  t->argument_last = NULL;
  t->argument_first = NULL;
  return t;
}



void term_destroy ( term * t ) { 
  assert(t != NULL);
  if (term_get_arity(t)==0) {
    t->father = NULL;
    t->symbol = NULL;
    t->next->previous = t->previous;
    t->previous->next = t->next;
    free(t);
    free(*t)
  }
  else {
    term_destroy(t->argument_first);
    t->argument_first = t->argument_last->next;
  }
}


sstring term_get_symbol ( term t ) { 
  assert ( NULL != t ) ;
  return t -> symbol ;
}


int term_get_arity ( term t ) {
  assert ( NULL != t ) ;
  return t -> arity ;
}


term term_get_father ( term t ) {
  assert ( NULL != t ) ;
  return t -> father ;
}


void term_add_argument_last ( term t ,
			      term a ) { 
  assert(t != NULL);
  assert(a != NULL);
  t->argument_last = a;
  a->father = t;
  t->arity = t->arity +1;
}


void term_add_argument_first ( term t ,
			       term a ) { 
  assert(t != NULL);
  assert(a != NULL);
  t->argument_first = a;
  a->father = t;
  t->arity = t->arity +1;
}


void term_add_argument_position ( term t ,
				  term a ,
				  int pos ) { 
  assert(t != NULL);
  assert(a != NULL);
  assert(0 <= pos);
  assert(pos <= term_get_arity(t));
  if (pos == 0){
    term_add_argument_first(t,a);
  }
  else {
      if (pos == term_get_arity(t)){
        term_add_argument_last(t,a);
      }
      else {
        term_list temp;
        temp->t = t;
        for (int i=1; i<pos; i++){
          temp->temp->next;
        }
        temp->next->previous = a;
        temp->next = a;
      }
  }
}


bool term_contains_symbol ( term t ,
			    sstring symbol ) { 
  return false ;
}


term term_get_argument ( term t ,
			 int pos ) { 
  return NULL ;
}


term term_extract_argument ( term t ,
			     int pos ) { 
  return NULL ;
}


term term_copy ( term t ) { 
  return NULL ;
}


term term_copy_translate_position ( term t ,
				    term * loc ) { 
  return NULL ;
}


void term_replace_copy ( term t_destination ,
			 term t_source ) { 
}


int term_compare ( term t1 ,
		   term t2 ) { 
  return 0 ;
}




struct term_argument_traversal_struct {
  term_list tls ;
} ;
 

term_argument_traversal term_argument_traversal_create ( term t ) { 
  return NULL ;
}


void term_argument_traversal_destroy ( term_argument_traversal * tt ) { 
}


bool term_argument_traversal_has_next ( term_argument_traversal tt ) { 
  return false ;
}


term term_argument_traversal_get_next ( term_argument_traversal tt ) {
  return NULL ;
}
