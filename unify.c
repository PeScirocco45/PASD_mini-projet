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

	sstring sstring_symbol_unify = sstring_create_string(symbol_unify);

	assert(term_contains_symbol(t,sstring_symbol_unify));

	//On cr�e des sstring des variables static pour faciliter la cr�ation de terme futur
	sstring sstring_symbol_equal = sstring_create_string(symbol_equal);
	sstring sstring_symbol_solution = sstring_create_string(symbol_solution);
	sstring sstring_symbol_val = sstring_create_string(symbol_val);
	sstring sstring_symbol_incompatible = sstring_create_string(symbol_incompatible);

	//On cr�e deux variables pour traverser d'une part toutes les �galit�s d'autre part pour parcourir les arguments des �galit�s � v�rifier
	term_argument_traversal traversal_t = term_argument_traversal_create(t);
	term_argument_traversal traversal_equal = NULL;

	//On cr�e les termes qui serviront � r�cup�rer ceux renvoy�s par le parcours
	term actual_t = NULL;
	term actual_equal = NULL;

	//On cr�e les termes qui serviront � faire des copies d'autre termes pour tester les �galit�s renvoy�s les valeurs etc...
	term solution = term_create(sstring_symbol_solution);
	term c1 = NULL;
	term c2 = NULL;
	term val = NULL;

	while(term_argument_traversal_has_next(traversal_t)){

		actual_t=term_argument_traversal_get_next(traversal_t);
		traversal_equal = term_argument_traversal_create(actual_t);

		while(term_argument_traversal_has_next(traversal_equal)){

			actual_equal=term_argument_traversal_get_next(traversal_equal);

			//On compare si les deux arguments sont �gaux, si oui :
			if((term_compare(term_get_argument(actual_equal,0),term_get_argument(actual_equal,1))==0)){

				//On regarde si ce ne sont pas des variables, si oui : on regarde les arguments des deux symbol qui ont �t� d�tect�s �gaux
				if(!term_is_variable(term_get_argument(actual_equal,0))){
					c1=term_get_argument(actual_equal,0);
					c2=term_get_argument(actual_equal,1);
					while(term_compare(c1,c2)==0 && !term_is_variable(c1)){
						c1 = term_get_argument(c1,0);
						c2 = term_get_argument(c2,0);
					}

					//Si les deux arguments sont des variables alors il y a �galit�
					if(term_is_variable(c1) && term_is_variable(c2)){
						val = term_create(sstring_symbol_val);
						c1=term_copy(c1);
						c2=term_copy(c2);
						term_add_argument_last(val,c1);
						term_add_argument_last(val,c2);

					//Sinon si le premier argument est une variable on teste l'�galit�
					}else if(term_is_variable(c1)){
						if(!term_contains_symbol(c2,term_get_symbol(c1))){
							val = term_create(sstring_symbol_val);
							c1=term_copy(c1);
							c2=term_copy(c2);
							term_add_argument_last(val,c1);
							term_add_argument_last(val,c2);
						}else{
							val = term_create(sstring_symbol_incompatible);
							c1=term_copy(c1);
							c2=term_copy(c2);
							term_add_argument_last(val,c1);
							term_add_argument_last(val,c2);
						}

					//De m�me que plus haut pour le second argument
					}else if(term_is_variable(c2)){
						if(!term_contains_symbol(c1,term_get_symbol(c2))){
							val = term_create(sstring_symbol_val);
							c1=term_copy(c1);
							c2=term_copy(c2);
							term_add_argument_last(val,c1);
							term_add_argument_last(val,c2);
						}else{
							val = term_create(sstring_symbol_incompatible);
							c1=term_copy(c1);
							c2=term_copy(c2);
							term_add_argument_last(val,c1);
							term_add_argument_last(val,c2);
						}

					//Sinon on renvoie l'in�galit�
					}else{
						val = term_create(sstring_symbol_incompatible);
						c1=term_copy(c1);
						c2=term_copy(c2);
						term_add_argument_last(val,c1);
						term_add_argument_last(val,c2);
					}
				//Sinon on renvoie l'�galit� : les deux sont bien compatibles
				}else{
					val = term_create(sstring_symbol_val);
					term_add_argument_last(solution,val);
					c1 = term_copy(term_get_argument(actual_equal,0));
					c2 = term_copy(term_get_argument(actual_equal,1));
					term_add_argument_last(val,c1);
					term_add_argument_last(val,c2);
				}

			//Cas o� les 2 arguments sont des variables
			}else if(term_is_variable(term_get_argument(actual_equal,0)) && term_is_variable(term_get_argument(actual_equal,1))){
				val = term_create(sstring_symbol_val);
				term_add_argument_last(solution,val);
				c1 = term_copy(term_get_argument(actual_equal,0));
				c2 = term_copy(term_get_argument(actual_equal,1));
				term_add_argument_last(val,c1);
				term_add_argument_last(val,c2);

			//Cas o� le premier argument est une variable
			}else if(term_is_variable(term_get_argument(actual_equal,0))){
				if(term_contains_symbol(term_get_argument(actual_equal,1),term_get_symbol(actual_equal))){
					val = term_create(sstring_symbol_incompatible);
					term_add_argument_last(solution,val);
					c1 = term_copy(term_get_argument(actual_equal,0));
					c2 = term_copy(term_get_argument(actual_equal,1));
					term_add_argument_last(val,c1);
					term_add_argument_last(val,c2);
				}else{
					val = term_create(sstring_symbol_val);
					term_add_argument_last(solution,val);
					c1 = term_copy(term_get_argument(actual_equal,0));
					c2 = term_copy(term_get_argument(actual_equal,1));
					term_add_argument_last(val,c1);
					term_add_argument_last(val,c2);
				}

			//Cas o� le deuxi�me argument est une variable
			}else if(term_is_variable(term_get_argument(actual_equal,1))){
				if((term_get_argument(actual_equal,0),term_get_symbol(term_get_argument(actual_equal,1)))){
					val = term_create(sstring_symbol_incompatible);
					term_add_argument_last(solution,val);
					c1 = term_copy(term_get_argument(actual_equal,0));
					c2 = term_copy(term_get_argument(actual_equal,1));
					term_add_argument_last(val,c1);
					term_add_argument_last(val,c2);
				}else{
					val = term_create(sstring_symbol_val);
					term_add_argument_last(solution,val);
					c1 = term_copy(term_get_argument(actual_equal,0));
					c2 = term_copy(term_get_argument(actual_equal,1));
					term_add_argument_last(val,c1);
					term_add_argument_last(val,c2);
				}

			//Dernier cas : in�galit�
			}else{
				val = term_create(sstring_symbol_incompatible);
				term_add_argument_last(solution,val);
				c1 = term_copy(term_get_argument(actual_equal,0));
				c2 = term_copy(term_get_argument(actual_equal,1));
				term_add_argument_last(val,c1);
				term_add_argument_last(val,c2);
			}
		}
	}

	//On d�truit les �l�ments restants inutiles en m�moire
	sstring * sstring_to_destroy = NULL;
	*sstring_to_destroy = sstring_symbol_unify;
	sstring_destroy(sstring_to_destroy);
	*sstring_to_destroy = sstring_symbol_equal;
	sstring_destroy(sstring_to_destroy);
	*sstring_to_destroy = sstring_symbol_solution;
	sstring_destroy(sstring_to_destroy);
	*sstring_to_destroy = sstring_symbol_val;
	sstring_destroy(sstring_to_destroy);
	*sstring_to_destroy = sstring_symbol_incompatible;
	sstring_destroy(sstring_to_destroy);
	sstring_to_destroy = NULL;
	term * term_to_destroy = NULL;
    c1 = NULL;
    c2 = NULL;
	val = NULL;
	*term_to_destroy = c1;
	term_destroy(term_to_destroy);
	*term_to_destroy = c2;
	term_destroy(term_to_destroy);
	*term_to_destroy = val;
	term_destroy(term_to_destroy);
	*term_to_destroy = actual_t;
	term_destroy(term_to_destroy);
	*term_to_destroy = actual_equal;
	term_destroy(term_to_destroy);
	term_to_destroy = NULL;
    term_argument_traversal * to_destroy = NULL;
    *to_destroy = traversal_t;
	term_argument_traversal_destroy(to_destroy);
	*to_destroy = traversal_equal;
	term_argument_traversal_destroy(to_destroy);
	to_destroy = NULL;
	return solution ;
}

