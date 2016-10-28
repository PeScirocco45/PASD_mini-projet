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
  sstring ss1 = sstring_create_empty();
  char const * c1 = "(";
  sstring ss2 = sstring_create_string(c1);
  char const * c2 = ")";
  sstring ss3 = sstring_create_string(c2);
  bool res = true;
  for (int i=0; i<sstring_get_length(symbol); i++){
      res = ((sstring_compare(symbol, ss1) != 0) && (!isspace(sstring_get_char(symbol, 0))) && (sstring_compare(ss2, symbol) != 0) && (sstring_compare(ss3, symbol)));
  }
  sstring_destroy(&ss1);
  sstring_destroy(&ss2);
  sstring_destroy(&ss3);
  return res;


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
  term_list temp = NULL;
  temp->t = (*t);
  if (term_get_arity(*t)==0) {
    (*t)->father = NULL;
    (*t)->symbol = NULL;
    temp->next->previous = temp->previous;
    temp->previous->next = temp->next;
    (*t)->father->arity = (*t)->father->arity -1;
    free(temp);
    free(t);
    free(*t);
  }
  else {
    term_destroy(&(*t)->argument_first->t);
    (*t)->argument_first = temp->next;
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
  t->argument_last->t = a;
  a->father = t;
  t->arity = t->arity +1;
}


void term_add_argument_first ( term t ,
             term a ) {
  assert(t != NULL);
  assert(a != NULL);
  t->argument_first = (term_list)a;
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
        term_list temp = NULL;
        term_list temp2 = NULL;
        temp->t = t;
        temp2->t = a;
        for (int i=1; i<pos; i++){
          temp = temp->next;
        }
        temp->next->previous = temp;
        temp->next = temp;
      }
  }
}


bool term_contains_symbol ( term t ,
          sstring symbol ) {
  assert(t != NULL);
  assert(symbol != NULL);
  term_list temp = NULL;
  temp->t = t->argument_last->t;
  if (sstring_compare(t->symbol, symbol) == 0) {
    return true;
  }
  else {
    term_contains_symbol(temp->next->t, symbol);
    temp->previous = temp->previous->next;
  }
  return false;
}


term term_get_argument ( term t ,
       int pos ) {
  assert(t != NULL);
  assert(0<= pos);
  assert(pos<=t->arity);
  term_list l =  t->argument_first;
  for(int i=0; i<pos; i++){
    l = l->next;
  }
  return l->t;
}


term term_extract_argument ( term t ,
           int pos ) {
  assert(t != NULL);
  assert(0 <= pos);
  assert(pos<=t->arity);
  term temp = term_get_argument(t,pos);
  term res = temp;
  term_destroy(&temp);
  return res;
}


term term_copy ( term t ) {
    assert(t != NULL);
    term nouv = term_create(t->symbol);
    nouv->father = t->father;
	term_argument_traversal courant = term_argument_traversal_create(t);
	while(term_argument_traversal_has_next(courant)){
        term_add_argument_last(nouv,term_copy(term_argument_traversal_get_next(courant)));
	}
	term_argument_traversal_destroy(&courant);
	return nouv;
}


term term_copy_translate_position ( term t ,
            term * loc ) {
  return NULL ;
}


void term_replace_copy ( term t_destination ,
       term t_source ) {
    assert(t_destination != NULL);
    assert(t_source != NULL);
    term * add = &t_destination;
    t_source->father = t_destination->father;
    term_destroy(&t_destination);
    *add = t_source;
}


int term_compare ( term t1 ,
       term t2 ) {
    assert(t1 != NULL);
    assert(t2 != NULL);
  if (sstring_compare(t1->symbol, t2->symbol) == -1){
        return -1;
  }
  else {
    if (sstring_compare(t1->symbol, t2->symbol) == 1){
        return 1;
    }
    else {
        if (term_get_arity(t1)<term_get_arity(t2)){
            return -1;
        }
        else {
            if (term_get_arity(t1)>term_get_arity(t2)){
                return 1;
            }
            else {
                term_compare(t1->argument_first->t, t2->argument_first->t);
            }
        }
    }
   }
   return 0;
}





struct term_argument_traversal_struct {
  term_list tls ;
} ;


term_argument_traversal term_argument_traversal_create ( term t ) {
  assert ( NULL != t ) ;
  // On creer un term_argument_traversal nouv
  term_argument_traversal nouv = ( term_argument_traversal ) malloc ( sizeof ( struct term_argument_traversal_struct ) ) ;
  assert ( NULL != nouv ) ;
  // On donne pour valeur à nouv -> tls la valeur de argument_first de t
  nouv -> tls = t -> argument_first ;
  return nouv ;
}


void term_argument_traversal_destroy ( term_argument_traversal * tt ) {
  assert( NULL != tt ) ;
  assert ( NULL != * tt ) ;
  ( * tt ) -> tls = NULL ;
  free( * tt ) ;
  *tt = NULL ;
}


bool term_argument_traversal_has_next ( term_argument_traversal tt ) {
  assert ( NULL != tt ) ;
  if ( tt -> tls -> next != NULL ) return true ;
  return false ;
}


term term_argument_traversal_get_next ( term_argument_traversal tt ) {
  assert ( NULL != tt ) ;
  // On t -> tls prend pour valeur la term_list next de tt -> tls 
  tt -> tls = tt -> tls -> next;
  return tt -> tls -> t ;
}
