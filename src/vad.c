#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "pav_analysis.h"

#include "vad.h"

const float FRAME_TIME = 10.0F; /* in ms. */

/* 
 * As the output state is only ST_VOICE, ST_SILENCE, or ST_UNDEF,
 * only this labels are needed. You need to add all labels, in case
 * you want to print the internal state in string format
 */

const char *state_str[] = {
  "UNDEF", "S", "V", "INIT", "POSIBLE_V", "POSIVBLE_S"
};

const char *state2str(VAD_STATE st) {
  return state_str[st];
}

/* Define a datatype with interesting features */
typedef struct {
  float zcr;
  float p;
  float am;
} Features;

/* 
 * TODO: Delete and use your own features!
 */

Features compute_features(const float *x, int N) {
  /*
   * Input: x[i] : i=0 .... N-1 
   * Ouput: computed features
   */
  /* 
   * DELETE and include a call to your own functions
   *
   * For the moment, compute random value between 0 and 1 
   */
  Features feat;
  feat.p = compute_power(x,N);
  return feat;
}

/* 
 * TODO: Init the values of vad_data
 */

VAD_DATA * vad_open(float rate, float alpha1) { 

  VAD_DATA *vad_data = malloc(sizeof(VAD_DATA));
  vad_data->state = ST_INIT;
  vad_data->sampling_rate = rate;
  vad_data->frame_length = rate * FRAME_TIME * 1e-3;
  
  vad_data->init_count = 0; //Inicializar a 0
  vad_data->sum_potencia_inicial = 0.0f; //inicializamos la acumulacion a 0
  vad_data->alpha1 = alpha1; //Margen inferior que se pasa por parametro
  vad_data->alpha2 = alpha1 - 10.0f; //Margen superior que se pasa por parametro

  vad_data->contador_posibles = 0; //Inicializar a 0
  vad_data->contador_segmentos = 0;

  return vad_data;
}

VAD_STATE vad_close(VAD_DATA *vad_data) {
  /* 
   * TODO: decide what to do with the last undecided frames
   */
  VAD_STATE state = vad_data->state;

  free(vad_data);
  return state;
}

unsigned int vad_frame_size(VAD_DATA *vad_data) {
  return vad_data->frame_length;
}

/* 
 * TODO: Implement the Voice Activity Detection 
 * using a Finite State Automata
 */

VAD_STATE vad(VAD_DATA *vad_data, float *x) {

  /* 
   * TODO: You can change this, using your own features,
   * program finite state automaton, define conditions, etc.
   */

  Features f = compute_features(x, vad_data->frame_length);
  vad_data->last_feature = f.p; /* Potencia acutal: save feature, in case you want to show */
   float time_passed = FRAME_TIME * 1e-3 * vad_data->contador_posibles; //Tiempo en estado transitorio de "posible"

  switch (vad_data->state) {
  case ST_INIT: 
  /* Acumula la potencia de los primeros frames para calcular el ruido de fondo
   * y asi definir los umbrales
   */
    vad_data->sum_potencia_inicial += compute_power(x, vad_data->frame_length);//Acumulaciön de potencias
    vad_data->init_count++; 
    
    if (vad_data->init_count >= N_INIT_FRAMES) { 
      float media_inicial = vad_data->sum_potencia_inicial / vad_data->init_count;
      vad_data->p0 = media_inicial + vad_data->alpha2; //Umbral inferior (S)
      vad_data->p1 = media_inicial + vad_data->alpha1 + vad_data->alpha2; //Umbral superior (V)
    
      vad_data->state = ST_SILENCE; //Estado por default inicialmente
      vad_data->contador_segmentos = 0;
    }
    break;

  case ST_SILENCE:
    vad_data->contador_segmentos++;
  /* Si la potencia supera p1, podria ser voz 
   * (cambiamos a estado ST_POSIBLE_V) y se reinicia el contador
   */
    if (f.p > vad_data ->p1){
      vad_data->contador_posibles = 1; 
      vad_data->state = ST_POSIBLE_V;
    }
    break;
  
  case ST_POSIBLE_V: 
  /* Si hay un numero de frames consecutivos que son sup a p1, se confirma V 
   * Si la potencia baja -> vuelve al estado de S
   */
    if (f.p < vad_data ->p1){
      vad_data->contador_posibles++;
      if(vad_data->contador_posibles >=N_POSIBLES)
      vad_data->state = ST_VOICE;
      vad_data->contador_segmentos = 1;
      
    } else {
      vad_data->state = ST_SILENCE;
    }
    break;

  case ST_VOICE: 
    vad_data->contador_posibles++;
  /* Si la potencia baja de p0, podria ser una pequeña pausa de voz 
   * (cambiamos a estado ST_POSIBLE_S) y se reinicia el contador
   */
    if (f.p < vad_data ->p0){
      vad_data->contador_posibles = 1;
      vad_data->state = ST_POSIBLE_S;
    }
    break;

  case ST_POSIBLE_S: 
  /* Si hay un numero de frames consecutivos que son inf a p0, se confirma S 
   * Si la potencia sube -> vuelve al estado de V
   */
    if (f.p < vad_data ->p0){
      vad_data->contador_posibles++;
      if(vad_data->contador_posibles >=N_POSIBLES)
      vad_data->state = ST_SILENCE;
      vad_data->contador_segmentos = 1;

    } else {
      vad_data->state = ST_VOICE;
    }
    break;

  

  case ST_UNDEF:
    break;
  }

  if (vad_data->state == ST_SILENCE ||
      vad_data->state == ST_VOICE)
    return vad_data->state;
  else
    return ST_UNDEF;
}

void vad_show_state(const VAD_DATA *vad_data, FILE *out) {
  fprintf(out, "%d\t%f\n", vad_data->state, vad_data->last_feature);
}
