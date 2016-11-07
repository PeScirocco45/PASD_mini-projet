# include <assert.h>

# undef NDEBUG   // FORCE ASSERT ACTIVATION

# include "term_variable.h"
# include "unify.h"


/*! Symbol key-word for a rewriting term. */
static char const * const symbol_rewrite = "rewrite" ;
/*! Symbol key-word for a single rewriting rule. */
static char const * const symbol_rule = "->" ;
/*! Symbol key-word for the result term. */
static char const * const symbol_results = "results" ;

/*! Symbol key-word for the affectation of variables. */
static char const * const symbol_affectation = "affectation" ;
/*! Symbol key-word for the valuation of one variable. */
static char const * const symbol_valuation = "valuation" ;

/*!
 * To check that a term correspond to a term pattern.
 * Terms \c t and \c pattern are not modified.
 * \param t term to check
 * \param pattern pattern to find
 * \param affectation term representing the variables set so far.
 * \pre t, pattern and affectation are non NULL.
 * \return true if \c pattern is matched. In such a case affectation is filled accordingly.
 */
static bool term_is_pattern ( term t ,
			      term pattern ,
			      term affectation ) { 
	assert(t != NULL);
	assert(pattern != NULL);
	assert(affectation != NULL);

	bool ispattern;
	term unifie;

	sstring unif = sstring_create_string("unify");
	sstring egal = sstring_create_string("=");
	sstring inc = sstring_create_string("incompatible");
	sstring v = sstring_create_string("val");
	term uni = term_create(unif);
	term equal = term_create(egal);
	term incomp = term_create(inc);
	term val = term_create(v);
	sstring_destroy(&unif);
	sstring_destroy(&egal);
	sstring_destroy(&inc);
	sstring_destroy(&v);
	term_add_argument_last(equal, t);
	term_add_argument_last(equal, pattern);
	term_add_argument_first(uni, equal);

	if (! term_compare(t,pattern) == 0) {
		ispattern = true;
		unifie = term_unify(uni);
		if (term_compare(unifie, incomp) == 0){
			ispattern = false;
		}
	}
	return ispattern;
}


/*!
 * Add an argument to a term in sorted position.
 * Sort is done according to term_compare.
 * If the added argument is already present, then it is not added.
 * \param t term to check.
 * \param arg term to add as argument.
 * \pre t and arg are non NULL.
 */
static void term_add_arg_sort_unique ( term t ,
				       term arg )  { 
	assert(t != NULL);
	assert(arg != NULL);
	term_argument_traversal courant = term_argument_traversal_create(t);
	int cpt = 0;
	term comp;
	while (term_argument_traversal_has_next(courant) && (term_compare((comp = term_argument_traversal_get_next(courant)), arg)) == -1){
		cpt ++;
	}
	if (! (term_compare(comp, arg)))
	term_add_argument_position(t,arg,cpt);
	
}

/*!
 * To make operate a single rewriting rule on a term.
 * The products of rewriting are added to the results term as argument.
 * They are added sorted without duplicate.
 * Rewriting process is local, but the whole structure has to output.
 * \param t_whole whole term 
 * \param t_current current sub-term being looked for a match
 * \param pattern pattern to match
 * \param replace term to replace matches
 * \param results terms already generated by previous rules and the current rule.
 * \pre none of the term is NULL.
 */
static void term_rewrite_rule ( term t_whole ,
				term t_current ,
				term pattern ,
				term replace ,
				term results ) {  
	assert(t_whole != NULL);
	assert(t_current != NULL);
	assert(pattern != NULL);
	assert(replace != NULL);
	assert(results != NULL);

	if (term_compare(t_current, pattern) == 0) {
		term_replace_copy(t_current, replace);
		term_add_arg_sort_unique(results, t_whole);
	}
}


/*!
 * Check that rules are well formed.
 * It is supposed that:
 * \li any number first argument has been removed
 * \li last argument is the term to rewrite
 *
 * Should be used for assert only
 */
static bool rules_are_well_formed ( term t) { 
	if (term_get_arity(t) < 2){
		return false;
	}
	else {
		term courant;
		sstring fleche = sstring_create_string(symbol_rule);
		for (int i=0; i<term_get_arity(t)-1; i++){
			courant = term_get_argument(t, i);
			if(sstring_compare(term_get_symbol(courant), fleche) != 0){
				return false;
			}
			else {
				if (term_get_arity(t) != 2){
					return false;
				}
			}
		}
	}
	return true;
}


term term_rewrite ( term t ) {  
	assert(rules_are_well_formed(t));

	/*int n = 1;
	term courant = t;
	term pattern;
	term solution = term_create(sstring_create_string(symbol_results));
	if (sstring_get_char(term_get_symbol(t), 3) != "-"){
		n = sstring_get_char(term_get_symbol(t), 3);
	}
	for (int i=0; i<n; i++){
		int cpt = 0;
		while (cpt != term_get_arity(t)){
			if (term_is_pattern(t, pattern, solution)){
				term_add_arg_sort_unique(solution,courant);
			}
			cpt ++;
		}
		term_rewrite(solution);
	}*/

  return NULL ;
}
