# include <stdlib.h>
# include <ctype.h>
# include <assert.h>

# include "term_variable.h"


bool term_is_variable(term t) {
	assert(t != NULL);
	// Si t a une arité de 0 
	// Et que son symbol est d'au moins de taille 2 avec pour premier caractère '\''
  if ((term_get_arity(t) == 0) && (sstring_get_char(term_get_symbol(t), 0) == '\'') 
  	&& (sstring_get_length(term_get_symbol(t)) >= 2)) {
  	char pos1 = sstring_get_char(term_get_symbol(t), 1);
  // Si le second caractère de son symbol est compris entre 'a' et 'z' ou 'A' et 'Z'
  // ou est le caractère '_' 
  	if (((pos1 >= 'a') && (pos1 <= 'z')) || ((pos1 >= 'A') && (pos1 <= 'Z')) 
  		                                                   || (pos1 == '_')) {
  		/* 
  		 * Alors on regarde si les autres caractères de son symbol sont:
  		 * compris entre 'a' et 'z' ou entre 'A' et 'Z' ou entre '0' et '9'
  		 * ou est le caractère '_'
  		 */
  		int term_symbol_length = sstring_get_length(term_get_symbol(t));
  		for (int i = 2; i < term_symbol_length - 1; i++) {
  			char posi = sstring_get_char(term_get_symbol(t), i);
  			if (!((posi >= 'a') && (posi <= 'z')) && !((posi >= 'A') && (posi <= 'Z')) 
  				              	&& !((posi >= '0') && (posi <= '9')) && !(posi == '_')) {
  				return false;
  			}	
  		}
  		return true;
  	}
	}
	return false;
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
		term arg = term_get_argument(t,0);
		term_replace_variable(arg,variable,value);
		while(term_argument_traversal_has_next(traversal)){
			arg = term_argument_traversal_get_next(traversal);
			term_replace_variable(arg,variable,value);
		}
		term_replace_variable(arg,variable,value);
	}
}
