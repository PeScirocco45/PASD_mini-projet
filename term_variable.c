# include <stdlib.h>
# include <ctype.h>
# include <assert.h>

# include "term_variable.h"


bool term_is_variable ( term t ) {
	assert(t != NULL);
  return ((term_get_arity(t) == 0) && (sstring_get_char(term_get_symbol(t),0) == '\''));
}



void term_replace_variable ( term t ,
			     sstring variable ,
			     term value ) { 
	assert(t != NULL);
	assert(variable != NULL);
	assert(!(sstring_is_empty(variable)));
	assert(value != NULL);
	//Cas où le terme est une variable (Cas d'arrêt)
	if (term_is_variable(t)) {
		if (term_contains_symbol(t,variable)){
			term_replace_copy(t,value);
		}
	//Cas général où l'on parcours la liste d'arguments d'un terme et on applique la foncton récursivement
	}else{
		term_argument_traversal traversal = term_argument_traversal_create(t);
		term arg = term_get_argumment(t,0);
		term_replace_variable(arg,variable,value);
		while(term_argument_traversal_has_next(traversal)){
			arg = term_argument_traversal_get_next(traversal);
			term_replace_variable(arg,variable,value);
		}
		term_replace_variable(arg,variable,value);
	}
}
