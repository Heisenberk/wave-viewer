/**
 * \file errors.c
 * \brief Fichier contenant les fonctions nécessaires à la gestion d'erreurs.
 * \author Clément Caumes
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "../inc/errors.h"

 /**
 * \enum enum err_code
 * \brief Constantes d'erreurs.
 *
 * err_code est une série de constantes prédéfinie pour diverses futures 
 * fonctions.
 * @author Clément Caumes
 */
enum err_code arnold_errno = ERR_NONE;

 /** 
 * @brief Affiche le message d'erreur sur la sortie d'erreur en fonction 
 * du code d'erreur spécifié.
 * @param err Code d'erreur.
 * @author Clément Caumes
 */
void err_print(enum err_code err)
{
    static const char *err_desc[] = {
        /* ERR_NONE */ 					"aucune erreur",
        /* ERR_FILE */ 					"ouverture en lecture du fichier impossible",
        /* ERR_BITS_PER_SAMPLE */ 		"le nombre de bits par sample doit être égal à 16",
		/* ERR_PCM */			 		"le fichier wav n'est pas PCM",
		/* ERR_MONO */			 		"le fichier wav n'est pas MONO",
		/* ERR_HERTZ */			 		"le fichier wav n'est pas à 44100Hz",
        /* ERR_FSEEK */					"impossible de faire des sauts dans le fichier",
		/* ERR_FREAD */					"impossible de lire le fichier",
		/* ERR_FWRITE */				"impossible d'écrire dans le fichier",
		/* ERR_VIDEO_MODE */ 			"impossible d'ouvrir le mode video SDL",
        /* ERR_OTHER */ 				"erreur inconnu"
    };

    /* Vérification de la valeur de "err". */
    err = (unsigned int)err <= ERR_OTHER ? err : ERR_OTHER;
    fprintf(stderr, "Erreur %d : %s.\n", err, err_desc[err]);
}
