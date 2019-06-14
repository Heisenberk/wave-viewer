/**
 * \file errors.h
 * \brief Fichier contenant les signatures des fonctions nécessaires à 
 * la gestion d'erreurs.
 * \author Clément Caumes
 *
 */

#ifndef ERRORS_H
#define ERRORS_H

 /**
 * \enum enum err_code
 * \brief Constantes d'erreurs.
 *
 * err_code est une série de constantes prédéfinie pour diverses futures 
 * fonctions.
 * @author Clément Caumes
 */
enum err_code {
    ERR_NONE = 0,               /*!< Aucune erreur. */
    ERR_FILE,                   /*!< Erreur pendant l'ouverture du fichier. */
    ERR_BITS_PER_SAMPLE,		/*!< Erreur le nombre de bits par sample doit être égal à 16. */
    ERR_PCM,					/*!< Erreur le fichier wav n'est pas PCM. */
    ERR_MONO,					/*!< Erreur le fichier wav n'est pas MONO. */
    ERR_HERTZ,					/*!< Erreur le fichier wav n'est pas à 44100Hz. */
    ERR_FSEEK,					/*!< Erreur utilisation fseek. */
    ERR_FREAD,					/*!< Erreur utilisation fread. */
    ERR_FWRITE, 				/*!< Erreur utilisation fwrite. */
    ERR_VIDEO_MODE, 			/*!< Erreur lors de l'ouverture du mode video SDL. */
    ERR_OTHER                   /*!< Erreur quelconque. */
};
extern enum err_code arnold_errno;

 /** 
 * @brief Affiche le message d'erreur sur la sortie d'erreur en fonction 
 * du code d'erreur spécifié.
 * @param err Code d'erreur.
 * @author Clément Caumes
 */
void err_print(const enum err_code err);

#endif    
