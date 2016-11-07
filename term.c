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
  t->symbol = sstring_copy(symbol);
  t->arity = 0;
  t->father = NULL;
  t->argument_last = NULL;
  t->argument_first = NULL;
  return t;
}


void term_destroy ( term * t ) {
  assert(t != NULL);
  sstring_destroy(&(*t)->symbol);
  while(term_get_arity(*t) != 0){
    term temp = term_extract_argument((*t),0);
    term_destroy(&temp);
  }
  free(*t);
  *t = NULL;
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
  term_list l = (term_list)malloc(sizeof(struct term_list_struct));
  l -> t = a ;
  l -> previous = NULL ;
  l -> next = NULL ;
  // Si on ajoute a à t qui n'a pas d'argument
  if ( t -> arity == 0 ) {
    t -> argument_first = l ;
    t -> argument_last = l ;
    t -> arity = 1 ;
  } else {
    // Sinon
    t -> argument_last -> next = l ;
    l -> previous = t -> argument_last ;
    t -> argument_last = l ;
    a -> father = t ;
    t -> arity = t -> arity + 1 ;
  }
}


void term_add_argument_first ( term t ,
             term a ) {
  assert(t != NULL);
  assert(a != NULL);
  term_list l = (term_list)malloc(sizeof(struct term_list_struct));
  l -> t = a ;
  l -> previous = NULL ;
  l -> next = NULL ;
  // Si on ajoute a à t qui n'a pas d'argument
  if ( t -> arity == 0 ) {
    t -> argument_first = l ;
    t -> argument_last = l ;
    t -> arity = 1 ;
  } else {
    // Sinon
    t -> argument_first -> previous = l ;
    l -> next = t -> argument_first ;
    t -> argument_first = l ;
    a -> father = t ;
    t -> arity = t -> arity + 1 ;
  }
}


void term_add_argument_position ( term t ,
          term a ,
          int pos ) {
  assert(t != NULL);
  assert(a != NULL);
  assert(0 <= pos);
  assert(pos <= term_get_arity(t));
  // Si la position est égale à 0
  if (pos == 0){
    term_add_argument_first(t,a);
  }
  else {
    // Sinon la position est égale à à l'arity
    if (pos == term_get_arity(t)){
      term_add_argument_last(t,a);
    } else {
      // Sinon la position est comprise entre 1 et arity - 1
      term_list temp = (term_list)malloc(sizeof(struct term_list_struct));
      term_list temp2 = (term_list)malloc(sizeof(struct term_list_struct));
      temp = t-> argument_first;
      temp2->t = a;
      for (int i=1; i<pos; i++){
        temp = temp->next;
      }
      temp -> next -> previous = temp2 ;
      temp2 -> next = temp -> next ;
      temp -> next = temp2 ;
      temp2 -> previous = temp ;
      t->arity = t->arity +1;
    }
  }
}


bool term_contains_symbol ( term t ,
          sstring symbol ) {
  assert(t != NULL);
  assert(symbol != NULL);
  term_list temp = (term_list)malloc(sizeof(struct term_list_struct));
  if ( term_get_arity ( t ) == 0 ) {
    return false;
  }
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
  term_list pt = t -> argument_first ;
  int cpt = 0 ;
  // On déplace pt à l'argument à extraire
  while ( cpt != pos ) {
    pt = pt -> next ;
    cpt++ ;
  }
  // Si l'arity vaut 1 on le "détache"
  if ( term_get_arity ( t ) == 1 ) {
    t -> argument_first = NULL ;
    t -> argument_last = NULL ;
    t -> arity = ( t -> arity ) - 1 ;
  } else {
    // Si le compteur vaut 0 on le "détache" au début
    if ( cpt == 0 ) {
      pt -> next -> previous = NULL ;
      t -> argument_first = pt -> next ;
      t -> arity = ( t -> arity ) - 1 ;
      // Si le compteur vaut 1 on le "détache" à la fin
    } else if ( cpt == ( term_get_arity ( t ) - 1 ) ) {
      pt -> previous -> next = NULL ;
      t -> argument_last = pt -> previous ;
      t -> arity = ( t -> arity ) - 1 ;
    } else {
      // Sinon le "détache" entre les autres
      pt -> next -> previous = pt -> previous ;
      pt -> previous -> next = pt -> next ;
      pt -> next = NULL ;
      pt -> previous = NULL ;
      t -> arity = ( t -> arity ) - 1 ;
    }
  }
  //On fait une copie de l'extract qu'on retourne après avoir free
  term res = term_copy ( pt -> t ) ;
  free ( pt ) ;
  pt = NULL;
  return res ;
}


term term_copy ( term t ) {
    assert(t != NULL);
  term nouv = term_create(t->symbol);
  nouv->father = t->father;
  if ( term_get_arity ( t ) == 0 ) {
    return nouv;
  }
  term_argument_traversal courant = term_argument_traversal_create(t);
  while(term_argument_traversal_has_next(courant)){
        term_add_argument_last(nouv,term_copy(term_argument_traversal_get_next(courant)));
  }
  term_argument_traversal_destroy(&courant);
  return nouv;
}


term term_copy_translate_position ( term t ,
            term * loc ) {
  assert(t != NULL);
  assert(*loc != NULL);
  term c = term_copy(t);
  if(term_contains_symbol(c,term_get_symbol(*loc))){
    term_argument_traversal courant = term_argument_traversal_create(c);
    while(term_argument_traversal_has_next(courant)){
      term actual = term_argument_traversal_get_next(courant);
      if(sstring_compare(actual->symbol,term_get_symbol(*loc))){
        *loc = actual;
        return c;
      }
    }
  term_argument_traversal_destroy(&courant);
  }
  return c ;
}


void term_replace_copy ( term t_destination ,
       term t_source ) {
  assert(t_destination != NULL);
  assert(t_source != NULL);
  term_destroy(&t_destination);
  t_destination = term_copy(t_source);
}


int term_compare ( term t1 ,
       term t2 ) {
  assert(t1 != NULL);
  assert(t2 != NULL);
  // Dans tous les cas on retourne 0 si on ne rentre dans aucun if
  // On compare les sstrings de t1 et t2
  if (sstring_compare(t1->symbol, t2->symbol) == -1){
    return -1;
  } else {
    if (sstring_compare(t1->symbol, t2->symbol) == 1){
      return 1;
    } else {
      // On compare les arity de t1 et t2
      if (term_get_arity(t1)<term_get_arity(t2)){
        return -1;
      } else {
        //
        if (term_get_arity(t1)>term_get_arity(t2)){
          return 1;
        } else {
          // Si l'arity est égale à 1
          if ( term_get_arity ( t1 ) == 0 ) {
            return 0;
          }
          if ( term_compare(t1->argument_first->t, t2->argument_first->t) == 0 ) {
            term_compare(t1->argument_first->next->t, t2->argument_first->next->t);
          }
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
  // On crée un term_argument_traversal nouv
  term_argument_traversal nouv = ( term_argument_traversal ) malloc ( sizeof ( struct term_argument_traversal_struct ) ) ;
  assert ( NULL != nouv ) ;
  // On donne à nouv -> tls -> next la valeur de argument_first de t
  nouv -> tls = ( term_list ) malloc ( sizeof ( struct term_list_struct ) ) ;
  nouv -> tls -> next = t -> argument_first ;
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
  // t -> tls prend pour valeur next de tt -> tls
  tt -> tls = tt -> tls -> next;
  return tt -> tls -> t ;
}