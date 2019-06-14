/**
 * \file graphic.h
 * \brief Fichier contenant les signatures des fonctions nécessaires au 
 * dessin SDL.
 * \author Clément Caumes
 *
 * Projet Arnold.
 */


#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "audio.h"
#include "errors.h"

/** 
 * @brief Dessine le fichier audio wav. 
 * @details Dessine avec la librairie SDL le fichier audio wav qui correspond 
 * au fichier à analyser.
 * @param wav structure représentant le fichier audio à dessiner.
 * @return 0 si le dessin s'est bien déroulé, 1 sinon.
 * @author Clément Caumes
 */
int draw_audio(AUDIO* wav);

#endif
