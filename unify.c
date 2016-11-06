# include <assert.h>
# include <stdbool.h>
# include <ctype.h>

# undef NDEBUG   // FORCE ASSERT ACTIVATION

# include "term_variable.h"
# include "unify.h"


/*! Symbol key-word for a unification term. */
static char const * const symbol_unify = "unify" ;

/*! Symbol key-word for an equality term. */
static char const * const symbol_equal = "=" ;

/*! Symbol key-word for a solution term. */
static char const * const symbol_solution = "solution" ;

/*! Symbol key-word for a value of a variable in a solution. */
static char const * const symbol_val = "val" ;

/*! Symbol key-word for sting an incompatible term (no solution). */
static char const * const symbol_incompatible = "incompatible" ;

term term_unify ( term t ) { 
	assert(t!=NULL);
	assert(term_contains_symbol(t,symbol_unify));
	term_argument_traversal traversal_t = term_argument_traversal_create(t);
	term_argument_traversal traversal_equal = NULL;
	term actual_t = NULL;
	term actual_equal = NULL;
	term solution = term_create(symbol_solution);
	term c1 = NULL;
	term c2 = NULL;
	term val = NULL;
	while(term_argument_traversal_has_next(traversal_t)){
		actual_t=term_argument_traversal_get_next(taversal_t);
		traversal_equal = term_argument_traversal_create(actual_t);
		while(term_argument_traversal_has_next(traversal_equal)){
			actual_equal=term_argument_traversam_get_next(traversal_equal);
			if(term_compare(term_get_argument(actual_equal,0),term_get_argument(actual_equal,1)==0){
				if(!term_is_variable(term_get_argument(actual_equal,0))){
					c1=term_get_argument(actual_equal,0);
					c2=term_get_argument(actual_equal,1);
					while(term_compare(c1,c2)==0 && !term_is_variable(c1)){
						c1 = term_get_argument(c1,0);
						c2 = term_get_argument(c2,0);
					}
					if(term_is_variable(c1) && term_is_variable(c2)){
						val = create_term(symbol_val);
						c1=term_copy(c1);
						c2=term_copy(c2);
						term_add_argument_last(val,c1);
						term_add_argument_last(val,c2);
					}else if(term_is_variable(c1)){
						if(!term_contains_symbol(c2,term_get_symbol(c1)){
							val = create_term(symbol_val);
							c1=term_copy(c1);
							c2=term_copy(c2);
							term_add_argument_last(val,c1);
							term_add_argument_last(val,c2);
						}else{
							val = create_term(symbol_incompatible);
							c1=term_copy(c1);
							c2=term_copy(c2);
							term_add_argument_last(val,c1);
							term_add_argument_last(val,c2);
						}
					}else if(term_is_variable(c2)){
						if(!term_contains_symbol(c1,term_get_symbol(c2)){
							val = create_term(symbol_val);
							c1=term_copy(c1);
							c2=term_copy(c2);
							term_add_argument_last(val,c1);
							term_add_argument_last(val,c2);
						}else{
							val = create_term(symbol_incompatible);
							c1=term_copy(c1);
							c2=term_copy(c2);
							term_add_argument_last(val,c1);
							term_add_argument_last(val,c2);
						}
					}else{
						val = create_term(symbol_incompatible);
						c1=term_copy(c1);
						c2=term_copy(c2);
						term_add_argument_last(val,c1);
						term_add_argument_last(val,c2);
					}
				}else{
					val = term_create(symbol_val);
					term_add_argument_last(solution,val);
					c1 = term_copy(term_is_variable(term_get_argument(actual_equal,0));
					c2 = term_copy(term_is_variable(term_get_argument(actual_equal,1));
					term_add_argument_last(val,c1);
					term_add_argument_last(val,c2);
				}
			}else if(term_is_variable(term_get_argument(actual_equal,0) && term_is_variable(term_get_argument(actual_equal,1)){
				val = term_create(symbol_val);
				term_add_argument_last(solution,val);
				c1 = term_copy(term_is_variable(term_get_argument(actual_equal,0));
				c2 = term_copy(term_is_variable(term_get_argument(actual_equal,1));
				term_add_argument_last(val,c1);
				term_add_argument_last(val,c2);
			}else if(term_is_variable(term_get_argument(actual_equal,0)){
				if(term_contains_symbol(term_get_argument(actual_equal,1),term_get_symbol(actual_equal,0)){
					val = term_create(symbol_incompatible);
					term_add_argument_last(solution,val);
					c1 = term_copy(term_is_variable(term_get_argument(actual_equal,0));
					c2 = term_copy(term_is_variable(term_get_argument(actual_equal,1));
					term_add_argument_last(val,c1);
					term_add_argument_last(val,c2);
				}else{
					val = term_create(symbol_val);
					term_add_argument_last(solution,val);
					c1 = term_copy(term_is_variable(term_get_argument(actual_equal,0));
					c2 = term_copy(term_is_variable(term_get_argument(actual_equal,1));
					term_add_argument_last(val,c1);
					term_add_argument_last(val,c2);
				}
			}else if(term_is_variable(term_get_argument(actual_equal,1)){
				if(term_contains_symbol(term_get_argument(actual_equal,0),term_get_symbol(actual_equal,1)){
					val = term_create(symbol_incompatible);
					term_add_argument_last(solution,val);
					c1 = term_copy(term_is_variable(term_get_argument(actual_equal,0));
					c2 = term_copy(term_is_variable(term_get_argument(actual_equal,1));
					term_add_argument_last(val,c1);
					term_add_argument_last(val,c2);
				}else{
					val = term_create(symbol_val);
					term_add_argument_last(solution,val);
					c1 = term_copy(term_is_variable(term_get_argument(actual_equal,0));
					c2 = term_copy(term_is_variable(term_get_argument(actual_equal,1));
					term_add_argument_last(val,c1);
					term_add_argument_last(val,c2);
				}
			}else{
				val = term_create(symbol_incompatible);
				term_add_argument_last(solution,val);
				c1 = term_copy(term_is_variable(term_get_argument(actual_equal,0));
				c2 = term_copy(term_is_variable(term_get_argument(actual_equal,1));
				term_add_argument_last(val,c1);
				term_add_argument_last(val,c2);
			}
		}
	}
  return solution ;
}
