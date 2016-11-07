# include <stdlib.h>
# include <string.h>
# include <ctype.h>
# include <assert.h>

# include "sstring.h"


# undef NDEBUG    // FORCE ASSERT ACTIVATION


/*! Used to test the validity in asserts */
# define ASSERT_SSTRING_OK( ss )					\
  assert ( NULL != ss ) ;						\
  assert ( ( 0 == ss -> length ) == ( NULL == ss -> chars ) ) 

/*! 
 * Structure to store a sstring.
 * There is no room for the final \c '\0' of C-string.
 * \param length  length of the string, the number of char exactly
 * \param chars pointer to the the sequence of char
 */
typedef struct sstring_struct {
  unsigned int length ;
  char * chars ;
} sstring_struct ;


bool sstring_is_empty ( sstring ss ) {
  ASSERT_SSTRING_OK ( ss ) ;
  return 0 == ss -> length ;
}
 

sstring sstring_create_empty ( void ) {
  sstring res = (sstring) malloc ( sizeof ( sstring_struct ) ) ;
  assert ( NULL != res ) ;
  res -> length = 0 ;
  res -> chars = NULL ;
  ASSERT_SSTRING_OK ( res ) ;
  return res ;
}


sstring sstring_create_string(char const* st) { 
	assert(NULL != st);
	sstring res = sstring_create_empty();
	//Si *st n'est pas le caractère vide 
	if (*st != 0) {
		res->length = strlen(st);
		res->chars = (char*) malloc(sizeof(char) * (res->length));
		assert(NULL != res->chars);
		memcpy(res->chars, st, res->length);
	}
	ASSERT_SSTRING_OK(res);
  return res;
}


void sstring_destroy ( sstring * ss ) {
  assert ( NULL != ss ) ;
  ASSERT_SSTRING_OK ( ( * ss ) ) ;
  // NB free ( NULL ) is legal
  free ( ( * ss ) -> chars ) ;
  free ( * ss ) ;
  * ss = NULL ;
}


void sstring_print ( sstring ss ,
		     FILE * f ) {
  ASSERT_SSTRING_OK ( ss ) ;
  assert ( NULL != f ) ;
  if ( ! sstring_is_empty ( ss ) ) {
    for ( unsigned int i = 0 ; i < ss -> length ; i ++ ) {
      fputc ( ss -> chars [i] , f ) ;
    }
  }
}


void sstring_concatenate(sstring ss1, sstring ss2) { 
 	ASSERT_SSTRING_OK(ss1);
 	ASSERT_SSTRING_OK(ss2);
 	// Si ss2 est vide rien ne se passe
 	// Sinon on ajoute ss2->chars à la fin de ss1->chars
 	if (!(sstring_is_empty(ss2))) {
 		unsigned int new_length = ss1->length + ss2->length;
 		ss1->chars = (char*) realloc(ss1->chars, sizeof(char) * (new_length));
	 	unsigned int j = 0;
	 	for (unsigned int i = ss1->length; i < (new_length); i++)
	 	{
	 		ss1->chars[i] = ss2->chars[j];
	 		j++;
	 	}
	 	ss1->length = new_length;
	 	ASSERT_SSTRING_OK(ss1);
	}
}


sstring sstring_copy ( sstring ss ) 
{
  ASSERT_SSTRING_OK ( ss ) ;
  if ( sstring_is_empty ( ss ) ) {
    return sstring_create_empty () ;
  } else {
    sstring res = (sstring) malloc ( sizeof ( sstring_struct ) ) ;
    assert ( NULL != res ) ;
    res -> length = ss -> length ;
    res -> chars = (char*) malloc ( res -> length * sizeof ( char ) ) ;
    assert ( NULL != res -> chars ) ;
    for ( unsigned int i = 0 ; i < res -> length ; i ++ ) {
      res -> chars [ i ] = ss -> chars [ i ] ;
    }
    ASSERT_SSTRING_OK ( res ) ;
    return res ;
  }
}


int sstring_compare (sstring ss1, sstring ss2) {  
  ASSERT_SSTRING_OK(ss1);
  ASSERT_SSTRING_OK(ss2);
  // Pour chaque itération on compare ss1->chars et ss2->chars
	for (unsigned int i = 0; (i < ss1->length) && (i < ss2->length); i++) {
  	if (ss1->chars[i] < ss2->chars[i]) {
	  	return -1;
	 	} else if (ss1->chars[i] > ss2->chars[i]) {
	 		return 1;
  	}
  }
  // Sinon on compare les tailles de ss1 et ss2
  if (ss1->length < ss2->length) {
  	return -1;
  } else if (ss1->length > ss2->length) {
  	return 1;
  } 
  return 0;
}


int sstring_get_length ( sstring ss ) {
  ASSERT_SSTRING_OK ( ss ) ;
  return ss -> length ;
}


char sstring_get_char (sstring ss, int i) { 
   ASSERT_SSTRING_OK(ss);
   assert(NULL != ss->chars);
   assert((i >= 0) && ((unsigned int) i < ss->length));
   return ss->chars[i];
}


bool sstring_is_integer(sstring ss, int* n_pt) { 
  int res = 0;
  // Si ss est vide alors on retourne false
  if (sstring_is_empty(ss)) {
  	return false;
  }
  // Sinon on parcourt ss-chars 
  // On vérifie qu'il contient des chiffres avec isdigit
  for(unsigned int i = 0; i < ss->length; i++) {
    if (isdigit(ss->chars[i])) {
      res = res * 10 + ss->chars[i] - '0';
    } else {
      return false;
    }
  }
  *n_pt = res;
  return true ;
}
