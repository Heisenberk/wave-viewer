/**
 * \file audio.h
 * \brief Fichier contenant les signature des fonctions nécessaires à la 
 * lecture du fichier audio.
 * \author Clément Caumes
 *
 */

#ifndef AUDIO_H
#define AUDIO_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct word{
	uint32_t deb;
	uint32_t length;
};
typedef struct word WORD;

struct elem{
	WORD word;
	struct elem* next;
};
typedef struct elem* WORD_LIST;

/** 
 * @brief Libère en mémoire la liste en paramètre. 
 * @param l liste à libérer. 
 * @author Clément Caumes
 */
void free_list(WORD_LIST l);

struct audio{
	uint32_t header_length;
	uint32_t data_length;
	FILE* file;
	int16_t* data;
	uint8_t* silent; //0 si silence et 1 si sound
	WORD_LIST words_list;
	uint32_t nb_words;
	
};
typedef struct audio AUDIO;

/** 
 * @brief Initialise la structure WAV correspondant au fichier à analyser 
 * mis en paramètre. 
 * @param argc nombre d'arguments que l'utilisateur a écrit. 
 * @param argv chaines de caractères que l'utilisateur a écrit. 
 * @return WAV* structure à créer.
 * @author Clément Caumes
 */
AUDIO* init_audio(int argc,char** argv);

/** 
 * @brief Libère en mémoire la structure WAV correspondant au fichier à 
 * analyser. 
 * @param *wav structure à libérer en mémoire.
 * @author Clément Caumes
 */
void free_audio(AUDIO* audio);

/** 
 * @brief Teste si le fichier en paramètre correspond bien aux attentes 
 * de l'application. 
 * @param audio structure à tester contenant les informations pour le 
 * fichier.
 * @return 1 si le format est compatible et 0 sinon. 
 * @author Clément Caumes
 */
int test_format(AUDIO* audio);

/** 
 * @brief Fait la lecture complète de l'extrait de son.
 * @param audio représente le son à analyser. 
 * @return 1 en cas d'erreur et 0 si tout se passe bien.
 * @author Clément Caumes
 */
int read_data(AUDIO* audio);

/** 
 * @brief Découpe en mots l'extrait de son en paramètre.
 * @param audio représentant le son à analyser. 
 * @return 0 dans tous les cas.
 * @author Clément Caumes
 */
int fill_list_words(AUDIO* audio);

/** 
 * @brief Analyse des différents mots de l'extrait de son.
 * @param audio représentant le son à analyser. 
 * @return 0 dans tous les cas.
 * @author Clément Caumes
 */
int create_word(AUDIO* audio);

#endif
