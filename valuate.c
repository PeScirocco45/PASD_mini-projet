# include <assert.h>
# include <stdbool.h>
#include <ctype.h>

# undef NDEBUG   // FORCE ASSERT ACTIVATION

# include "term_variable.h"
# include "valuate.h"


/*! Symbol key-word for setting a variable term. */
static char const * const symbol_set = "set" ;


/*!
 * Used as a stack to record the values of the variable being replaced.
 */
typedef struct variable_definition_list_struct {
  /*! variable symbol */
  sstring variable ;
  /*! variable value */
  term value ;
  /*! link to next valuation */
  struct variable_definition_list_struct * next ;
}  * variable_definition_list ;


/*!
 * Used to store the symbol for setting a variable.
 * Global variable to avoid to have to redefined it at every call to \c term_valuate_inner
 */
static sstring ss_set = NULL ;



/*!
 * Recursive valuating function.
 * \param t term being valuated.
 * \param t_res valuated variable stack (list of active variables).
 */
static term term_valuate_inner ( term t ,
				 variable_definition_list var_list ) {
		term_replace_variable(t,ss_set,var_list->value);
		term_argument_traversal traversal = term_argument_traversal_create(t);
		term actual = NULL;
		while(term_argument_traversal_has_next(traversal)){
			actual = term_argument_traversal_get_next(traversal);
			if (term_get_arity(actual>2) && term_contains_symbol(actual,symbol_set)){
				variable_definition_list var_list2 = (variable_definition_list)malloc(sizeof(struct variable_definition_list_struct));
				var_list2 -> variable = sstring_copy(term_get_symbol(term_get_argument(t,0)));
				var_list2 -> value = term_create(ss_set);
				var_list2 -> next = NULL;
				var_list -> next = var_list2;
				var_list = var_list -> next;
				term_valuate_inner(actual, var_list);
			}
		}
		return t;
}



term term_valuate ( term t ) { 
	if(term_get_arity(t)>2 && term_contains_symbol(t,symbol_set)){
		term v = term_copy(term_get_argument(t,2));
		ss_set = sstring_copy(term_get_symbol(term_get_argument(t,1)));
		variable_definition_list var_list = (variable_definition_list)malloc(sizeof(struct variable_definition_list_struct));
		var_list -> variable = sstring_copy(term_get_symbol(term_get_argument(t,0)));
		var_list -> value = term_create(ss_set);
		var_list -> next = NULL;
		v = term_valuate_inner(v,var_list);
		return v;
	}else{
		term cp = term_copy(term_get_argument(t,2));
		term_argument_traversal traversal = term_argument_traversal_create(t);
		term actual = NULL;
		while(term_argument_traversal_has_next(traversal)){
			actual = term_argument_traversal_get_next(traversal);
			return term_valuate(actual);
		}
	}
}

