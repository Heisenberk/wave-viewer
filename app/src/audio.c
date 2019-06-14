/**
 * \file audio.c
 * \brief Fichier contenant les fonctions nécessaires à la lecture du
 * fichier audio.
 * \author Clément Caumes
 *
 */

#include <string.h>

#include "../inc/audio.h"
#include "../inc/errors.h"

/** Signature du subchunk data. */
#define WAV_DATA_SIGN 0x61746164
/** Signature du subchunk fmt. */
#define WAV_FMT_SIGN 0x20746d66
/** Offset entre le champ situé après la taille du subchunk fmt et le nombre de bit
 * par sample. */
#define WAV_FMT_BPS_OFF 14
/** Valeur représentant PCM dans le header WAV. */
#define WAV_FORMAT_PCM 1
/** Valeur représentant le mono dans le header WAV. */
#define WAV_MONO 1
/** Valeur représentant la fréquence dans le header WAV. */
#define WAV_HERTZ 44100
/** Adresse du premier subchunk (octet). */
#define WAV_SUBCHK1_ADDR 12
/** Longueur des espaces entre les mots. */
#define LENGTH_BYTE_BTW_WORDS 1000

/** 
 * @brief Crée une liste de mots vide. 
 * @return Liste vide créée. 
 * @author Clément Caumes
 */
WORD_LIST create_list(){return NULL;}

/** 
 * @brief Teste si une liste est vide. 
 * @param l liste à tester. 
 * @return 1 si la liste est vide et 0 sinon. 
 * @author Clément Caumes
 */
int test_list_empty(WORD_LIST l)
{
	if(l == NULL)
		return 1;
	return 0;
}

/** 
 * @brief Libère en mémoire la liste en paramètre. 
 * @param l liste à libérer. 
 * @author Clément Caumes
 */
void free_list(WORD_LIST l)
{
	WORD_LIST tmp;
	while(l)
	{
		tmp = l->next;
		free(l);
		l = tmp;
	}
}

/** 
 * @brief Ajoute un mot au début de la liste. 
 * @param l liste où insérer.
 * @param word mot à insérer.  
 * @return WORD_LIST modifiée. 
 * @author Clément Caumes
 */
WORD_LIST add_elem_begin(WORD_LIST l,WORD word)
{
	WORD_LIST new = malloc(sizeof(struct word));
	new->word = word;
	new->next = l;
	return new;
}

/** 
 * @brief Ajoute un mot à la fin de la liste. 
 * @param l liste où insérer.
 * @param word mot à insérer.  
 * @return WORD_LIST modifiée. 
 * @author Clément Caumes
 */
WORD_LIST add_elem_end(WORD_LIST l, WORD word)
{
	WORD_LIST new = malloc(sizeof(struct word));
	new->word = word;
	new->next = NULL;
	if(test_list_empty(l))
	{
		return new;
	}
	WORD_LIST debut = l;
	//on place le pointeur sur le dernier element de la liste
	while(l->next)
		l = l->next;
	l->next = new;
	return debut;
}

/** 
 * @brief Initialise une structure AUDIO grâce aux paramètres choisis par 
 * l'utilisateur. 
 * @param argc nombre d'arguments choisis par l'utilisateur.
 * @param **argv chaines de caractères choisis par l'utilisateur.  
 * @return AUDIO initialisé. 
 * @author Clément Caumes
 */
AUDIO* init_audio(int argc,char** argv){
	if(argc==1){
		return arnold_errno=ERR_FILE, NULL;
	}
	AUDIO* audio=malloc(sizeof(AUDIO));
	audio->file=fopen(argv[1],"rb");
	if(audio->file==NULL){
		arnold_errno=ERR_FILE;
		free(audio);
		return NULL;
	}
	else return audio;
}

/** 
 * @brief Libère en mémoire la structure AUDIO. 
 * @param audio structure à libérer. 
 * @author Clément Caumes
 */
void free_audio(AUDIO* audio){
	if(audio->file!=NULL) fclose(audio->file);
	free_list(audio->words_list);
	free(audio->data);
	free(audio->silent);
	free(audio);
}


/** 
 * @brief Teste la validité du format wav.
 * @param audio structure à valider. 
 * @return 0 si le format du fichier est valide et 1 sinon. 
 * @author Clément Caumes
 */
int test_format(AUDIO* audio){
	/* Lecture de tout les subchunk depuis le premier subchunk du header
     * jusqu'au subchunk "data". */
    uint16_t bits_per_sample;
    uint32_t chunk_id = 0, chunk_size = WAV_SUBCHK1_ADDR;   /* ID et taille du chunk lu. */
    while (chunk_id != WAV_DATA_SIGN) {
		/* On saute le chunk venant d'être lu (pour le premier : on va à
         * l'adresse du premier subchunk). */
        if (fseek(audio->file, chunk_size, SEEK_CUR))
			return arnold_errno=ERR_FSEEK, 0;
        /* Lecture de l'ID du subchunk. */
        if (fread(&chunk_id, sizeof(chunk_id), 1, audio->file) != 1)
			return arnold_errno=ERR_FREAD, 0;
        /* Lecture de la taille du subchunk. */
		if (fread(&chunk_size, sizeof(chunk_size), 1, audio->file) != 1)
			return arnold_errno=ERR_FREAD, 0;
			
		/* Cas spécial : quand on lit le subchunk fmt, on en profite pour
		 * lire le nombre de bits par sample. */
		 uint16_t double_byte;
		 uint32_t four_byte;
        if (chunk_id == WAV_FMT_SIGN) {
			//test si c'est PCM
			if (fread(&double_byte, sizeof(uint16_t), 1, audio->file) != 1)
				return arnold_errno=ERR_FREAD, 0;
			if(double_byte!=WAV_FORMAT_PCM) 
				return arnold_errno = ERR_PCM, 0;
			//test si c'est mono
			if (fread(&double_byte, sizeof(uint16_t), 1, audio->file) != 1)
				return arnold_errno=ERR_FREAD, 0;
			if(double_byte!=WAV_MONO) 
				return arnold_errno = ERR_MONO, 0;
			//test si c'est 44100Hz
			if (fread(&four_byte, sizeof(uint32_t), 1, audio->file) != 1)
				return arnold_errno=ERR_FREAD, 0;
			if(four_byte!=WAV_HERTZ) 
				return arnold_errno = ERR_HERTZ, 0;
			if (fseek(audio->file, -8, SEEK_CUR))
				return arnold_errno=ERR_FSEEK, 0;
			
			chunk_size -= WAV_FMT_BPS_OFF + sizeof(uint16_t);
			if (fseek(audio->file, WAV_FMT_BPS_OFF, SEEK_CUR))
				return arnold_errno=ERR_FSEEK, 0;
			if (fread(&bits_per_sample, sizeof(uint16_t), 1, audio->file) != 1)
				return arnold_errno=ERR_FREAD, 0;
			if(bits_per_sample!=16)
				return arnold_errno = ERR_BITS_PER_SAMPLE, 0;
        }
	}
    /* Récupération de la taille totale du header et de la taille de data. */
    audio->header_length=ftell(audio->file);
    audio->data_length=chunk_size;
    audio->data=malloc((audio->data_length/2)*sizeof(uint16_t));
    audio->silent=calloc((audio->data_length/2),sizeof(uint8_t));

    return 1;	
}

/** 
 * @brief Fait la lecture complète de l'extrait de son.
 * @param audio représente le son à analyser. 
 * @return 1 en cas d'erreur et 0 si tout se passe bien.
 * @author Clément Caumes
 */
int read_data(AUDIO* audio){
	if (fseek(audio->file, audio->header_length, SEEK_SET))
		return arnold_errno=ERR_FSEEK, 1;
	uint32_t i; 

	// lecture des données
	for(i=0;i<(audio->data_length/2);i++){
		if (fread(&(audio->data[i]), sizeof(int16_t), 1, audio->file) != 1)
			return arnold_errno=ERR_FREAD, 1;
	}
	
	int tmp;
	i=0; 
	uint32_t deb_silent=0;
	uint32_t j;
	while(i<(audio->data_length/2)){		
		tmp=((audio->data[i])*650)/32768; //calcul inventé qui peut etre proche de la realité
		if((tmp<=-5)||(tmp>=5)) { //si on tombe sur du son on incremente deb_sound pour savoir combien de temps il y a du son
			audio->silent[i]=1; //vert
			if(deb_silent>LENGTH_BYTE_BTW_WORDS){
				for(j=i-deb_silent;j<i;j++){
					audio->silent[j]=0; //blanc
				}
			}
			else{
				for(j=i-deb_silent;j<i;j++){
					audio->silent[j]=1; //vert
				}
			}
			deb_silent=0;
		}
		else{ //sinon cest du silence
			deb_silent++;
		}
		i++;

	}
	return 0;
}

/** 
 * @brief Découpe en mots l'extrait de son en paramètre.
 * @param audio représentant le son à analyser. 
 * @return 0 dans tous les cas.
 * @author Clément Caumes
 */
int fill_list_words(AUDIO* audio){
	
	audio->words_list=create_list();
	
	
	if (fseek(audio->file, audio->header_length, SEEK_SET))
		return arnold_errno=ERR_FSEEK, 1;
	uint32_t i;
	uint32_t compteur_word=0; 
	uint32_t compteur_bits_per_word=0;
	uint8_t bool=1;
	WORD w;

	// lecture des données
	for(i=0;i<(audio->data_length/2);i++){
		if(audio->silent[i]==1){ //si BRUIT
			if(bool==1){ //premier passage du premier bit
				compteur_word++;
				bool=0;
			}
			compteur_bits_per_word++;
		}
		if(audio->silent[i]==0){ //si blanc
			if(compteur_bits_per_word!=0){
				
				w.deb=i-compteur_bits_per_word;
				w.length=compteur_bits_per_word;
				if(w.length>LENGTH_BYTE_BTW_WORDS){
					audio->words_list=add_elem_end(audio->words_list,w);
				}
				else compteur_word--;
			}
			compteur_bits_per_word=0;
			bool=1;
		}
	}
	if(bool==0) { //cas particulier : si il y a un bruit qui se coupe a la fin
		w.deb=i-compteur_bits_per_word;
		w.length=compteur_bits_per_word;
		audio->words_list=add_elem_end(audio->words_list,w);
	}
	audio->nb_words=compteur_word;
	return 0;
}

/** 
 * @brief Analyse des différents mots de l'extrait de son.
 * @param audio représentant le son à analyser. 
 * @return 0 dans tous les cas.
 * @author Clément Caumes
 */
int create_word(AUDIO* audio){
	uint32_t i,k;
	uint8_t zero=0;
	int j;
	
	FILE* f=fopen("build/result.wav","wb");
	
	int8_t hexData[0x28] = {
		0x52, 0x49, 0x46, 0x46, 0x24, 0x50, 0x05, 0x00, 0x57, 0x41, 0x56, 0x45, 0x66, 0x6D, 0x74, 0x20, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x44, 0xAC, 0x00, 0x00, 0x88, 0x58, 0x01, 0x00, 
		0x02, 0x00, 0x10, 0x00, 0x64, 0x61, 0x74, 0x61 };
	for(j=0;j<0x28;j++){
			fwrite(&hexData[j], sizeof(int8_t), 1, f);
	}
	uint32_t length_new=0;
	WORD_LIST w=audio->words_list;
	for(i=0;i<audio->nb_words;i++){
		length_new+=w->word.length;
		w=w->next;
	}
	length_new*=2;
	fwrite(&length_new,sizeof(uint32_t),1,f);
	printf("\n");
	printf("Nombre de mots dans l'extrait de son : %d\n",audio->nb_words);
	for(i=0;i<audio->nb_words;i++){
		for(k=audio->words_list->word.deb;k<(audio->words_list->word.deb+audio->words_list->word.length);k++){
			fwrite(&audio->data[k],sizeof(int16_t),1,f);
		}
		
		printf("(%d;%d) ",audio->words_list->word.deb,audio->words_list->word.length);
		audio->words_list=audio->words_list->next;
	}
	printf("\n");
	
	if((length_new & 1) == 1) fwrite(&zero,sizeof(uint8_t),1,f);
	fclose(f);
	
	return 0;
}
