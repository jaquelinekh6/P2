#ifndef _VAD_H
#define _VAD_H

#define N_INIT_FRAMES 10        //Para calcular el ruido de fondo
#define N_POSIBLES 2            //Frames para confirmar cambio de estado de transición (Aumentado de 3 a 4)
#define MIN_SEGMENT_FRAMES 10   //Duración mínima de segmento en frames (0.15s para frames de 10ms)

#include <stdio.h>

/* TODO: add the needed states */
typedef enum {ST_UNDxEF=0, ST_SILENCE, ST_VOICE, ST_INIT, ST_POSIBLE_V, ST_POSIBLE_S} VAD_STATE;

/* Return a string label associated to each state */
const char *state2str(VAD_STATE st);

/* TODO: add the variables needed to control the VAD 
   (counts, thresholds, etc.) */

typedef struct {
  VAD_STATE state;
  float sampling_rate;
  unsigned int frame_length;
  float last_feature; /* for debuggin purposes */

  /* El proceso consiste en medir el nivel base de potencia ruido de fondo
   * usando los primeros N frames.(se hace calcula en vad_open)
   * p0 = promedio + alpha2 → umbral inferior para pasar a S
   * p1 = promedio + alpha1 + alpha2 → umbral superior para pasar a v
   */
  float p0, p1; //umbral abs. inferior (S) y superior de potencia(V)
  int init_count; //N frames iniciales para calcular el ruido de fondo
  float sum_potencia_inicial; //suma acumulada de potencia inicial
  float alpha1, alpha2; //Margenes para construir los umbrales
 
   /* Cuenta cuántos frames seguidos cumplen la condición para 
    * confirmar el cambio entre V y S
    */
   int contador_posibles; //Para contar el tiempo minimo de la transición
   int contador_segmentos; //Para asegurar que un segmento de V/S dure al menos un tiempo minimo
   


} VAD_DATA;

/* Call this function before using VAD: 
   It should return allocated and initialized values of vad_data

   sampling_rate: ... the sampling rate */
VAD_DATA *vad_open(float sampling_rate, float alpha1);

/* vad works frame by frame.
   This function returns the frame size so que the program knows how
   many samples have to be provided */
unsigned int vad_frame_size(VAD_DATA *);

/* Main function. For each 'time', compute the new state 
   It returns:
    ST_UNDEF   (0) : undefined; it needs more frames to take decission
    ST_SILENCE (1) : silence
    ST_VOICE   (2) : voice

    x: input frame
       It is assumed the length is frame_length */
VAD_STATE vad(VAD_DATA *vad_data, float *x);

/* Free memory
   Returns the state of the last (undecided) states. */
VAD_STATE vad_close(VAD_DATA *vad_data);

/* Print actual state of vad, for debug purposes */
void vad_show_state(const VAD_DATA *, FILE *);

#endif