/**
 * \file test_audio.c
 * \brief Fonctions concernant les tests unitaires du fichier audio.c.
 * \author Clément Caumes - Yassin Doudouh
 * \date 26 Avril 2018
 *
 * Programme de test de primalité de Solovay-Strassen.
 *
 */

#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

#include "../app/inc/audio.h"
#include "../app/inc/errors.h"

/**
 * \fn void test_pgcd_1()
 * \brief Teste si pgcd(1128,2986)=1.
 */
void test_format_wav(){
	AUDIO* wav=malloc(sizeof(AUDIO));
	wav->file=fopen("env/a.wav","rb");
	CU_ASSERT_NOT_EQUAL(wav->file,NULL);
	
	CU_ASSERT_EQUAL(test_format(wav),1);
	CU_ASSERT_EQUAL(wav->header_length,44);
	CU_ASSERT_EQUAL(wav->data_length,266240);
	
	if(wav->file!=NULL) fclose(wav->file);
	free_list(wav->words_list);
	free(wav->data);
	free(wav->silent);
	free(wav);
	
}

int setup(void)  { return 0; }
int teardown(void) { return 0; }

int main(){
	
	CU_initialize_registry();
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();	
	
	CU_pSuite suite = CU_add_suite("Tests Audio", setup,teardown);

	CU_add_test(suite, "test_format_wav", test_format_wav);

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();

    return 0;
}	

