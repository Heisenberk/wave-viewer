/**
 * \file graphic.c
 * \brief Fichier contenant les fonctions nécessaires au dessin SDL.
 * \author Clément Caumes
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>

#include "../inc/graphic.h"

 /** 
 * @brief Dessine les ondes produits par l'extrait de son en paramètre.
 * @param 0 si tout se passe bien et 1 sinon.
 * @author Clément Caumes
 */
int draw_audio(AUDIO* wav){
	SDL_Surface *ecran = NULL;
    
    SDL_Surface **pixel= NULL;
    uint32_t length=(wav->data_length/2);
    pixel=malloc(length*sizeof(SDL_Surface));
    
    SDL_Rect position;

    SDL_Init(SDL_INIT_VIDEO);
    
    int continu=1;
    SDL_Event event;
    SDL_EnableKeyRepeat(10, 10);

	// Initialisation de la fenetre
    ecran = SDL_SetVideoMode(650, 650, 32, SDL_HWSURFACE);
    if (ecran == NULL) // si l'ouverture a échoué
    {
		return arnold_errno=ERR_VIDEO_MODE, 1;
    }    
    
    SDL_WM_SetCaption("Audio WAV", NULL);
    
    uint32_t i;
    uint32_t deb=0;
    int xx,yy;
    xx=yy=0;
    for(i=0;i<length;i++){
		pixel[i] = SDL_CreateRGBSurface(SDL_HWSURFACE, 1, 1, 32, 0, 0, 0, 0);
	}
    

    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
    
    while(continu){
		SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continu = 0;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_UP: // Flèche haut
                        yy-=100;
                        break;
                    case SDLK_DOWN: // Flèche bas
                        yy+=100;
                        break;
                    case SDLK_RIGHT: // Flèche droite
						if(deb+100<length-1000) deb+=100;
                        break;
                    case SDLK_LEFT: // Flèche gauche
                        xx-=100;
                        if(deb>0) deb-=100;
                        break;
                    default:
						break;
                }
                break;
            default:
				break;
        }
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
            
        int j=0;
		for(i=deb;i<deb+650;i++){		
				
			position.x=j;
			position.y=((wav->data[i])*650)/32768 +300;
			
			if(wav->silent[i]==1) SDL_FillRect(pixel[i], NULL, SDL_MapRGB(ecran->format, 0, 255, 0)); //en vert ->1 -->BRUIT
			else SDL_FillRect(pixel[i], NULL, SDL_MapRGB(ecran->format, 255, 255, 255)); //en blanc ->0 -->SILENCE
			
			SDL_BlitSurface(pixel[i], NULL, ecran, &position); // Collage de la surface sur l'écran
			j++;
		}

    SDL_Flip(ecran); // Mise à jour de l'écran
	}

	for(i=0;i<length;i++){
		SDL_FreeSurface(pixel[i]); // Libération de la surface
	}
	
	free(pixel);
	
	SDL_FreeSurface(ecran);
    
    SDL_Quit();
    return 0;
}
