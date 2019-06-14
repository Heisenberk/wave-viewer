/**
 * \file arnold.c
 * \brief Fichier principal contenant le main.
 * \author Clément Caumes
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "../inc/audio.h"
#include "../inc/graphic.h"
#include "../inc/errors.h"

/** 
 * @brief Correspond à la fonction principale du projet Arnold.
 * @param argc nombre d'arguments que l'utilisateur a écrit. 
 * @param argv chaines de caractères que l'utilisateur a écrit. 
 * @return EXIT_FAILURE si il y a eu un problème et EXIT_SUCCESS si tout 
 * se passe bien.
 * @author Clément Caumes
 */
int main(int argc,char** argv){

	// Initialisation de la structure représentant le fichier audio à analyser
	AUDIO* audio=init_audio(argc,argv);
	if(audio==NULL)
		return err_print(arnold_errno), EXIT_FAILURE;
	
	// Test de validité du format audio à analyser
	if(!test_format(audio))
		return err_print(arnold_errno),EXIT_FAILURE;
	
	// Lecture du fichier audio
	if(read_data(audio))
		return err_print(arnold_errno),EXIT_FAILURE;
	
	// Dessin du fichier audio
	if(draw_audio(audio))
		return err_print(arnold_errno),EXIT_FAILURE;
	
	// Découpage en mots dans le fichier audio
	if(fill_list_words(audio))
		return err_print(arnold_errno),EXIT_FAILURE;
		
	// Création du fichier audio sans silence
	if(create_word(audio))
		return err_print(arnold_errno),EXIT_FAILURE;
		
	// Libération de la mémoire pour la structure du fichier audio analysé
	free_audio(audio);
	
	return EXIT_SUCCESS;
}
