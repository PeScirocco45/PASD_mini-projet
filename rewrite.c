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
	/*assert(t != NULL);
	assert(pattern != NULL);
	assert(affectation != NULL);
	if (term_contains_symbol (t, pattern->symbol)){
		term var = term_unify (affectation);
		term_replace_variable(t, var, pattern);
		return true;
	}*/
 	return false ;
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
	/*assert(t != NULL);
	assert(arg != NULL);
	if (! term_contains_symbol(t, arg->symbol)){
		term_argument_traversal courant = term_argument_traversal_create(t);
		int cpt = 0;
		while (term_argument_traversal_has_next(courant) && (term_compare(courant->tls->t, arg)) == -1){
			cpt ++;
		}
		term_add_argument_position(t,arg,cpt);
	}*/
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
	/*assert(t_whole != NULL);
	assert(t_current != NULL);
	assert(pattern != NULL);
	assert(replace != NULL);
	assert(results != NULL);

	if(term_is_pattern(t_current, pattern, replace)){
		term_add_arg_sort_unique(t_current,replace);
		results = t_whole;
	}*/
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

  return false ;
}


term term_rewrite ( term t ) {  
  return NULL ;
}
