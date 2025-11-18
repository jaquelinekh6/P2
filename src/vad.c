#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "pav_analysis.h"

#include "vad.h"

const float FRAME_TIME = 10.0F; /* in ms. */

/* * As the output state is only ST_VOICE, ST_SILENCE, or ST_UNDEF,
 * only this labels are needed. You need to add all labels, in case
 * you want to print the internal state in string format
 */

const char *state_str[] = {
  "UNDEF", "S", "V", "INIT", "POSIBLE_V", "POSIBLE_S"
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

/* * TODO: Delete and use your own features!
 */

Features compute_features(const float *x, int N) {
  /*
   * Input: x[i] : i=0 .... N-1 
   * Ouput: computed features
   */
  Features feat;
  feat.p = compute_power(x,N);
  return feat;
}

/* * TODO: Init the values of vad_data
 */

// El argumento alpha_unused (anteriormente alpha_offset) ya no se usa, ya que los óptimos están fijos.
VAD_DATA * vad_open(float rate, float alpha_unused) { 

  VAD_DATA *vad_data = malloc(sizeof(VAD_DATA));
  vad_data->state = ST_INIT;
  vad_data->sampling_rate = rate;
  vad_data->frame_length = rate * FRAME_TIME * 1e-3;
  
  vad_data->init_count = 0; //Inicializar a 0
  vad_data->sum_potencia_inicial = 0.0f; //inicializamos la acumulacion a 0
  
  // VALORES ÓPTIMOS ENCONTRADOS
  vad_data->alpha1 = 10.25f; // Margen V óptimo
  vad_data->alpha2 = 0.75f; // Margen S óptimo (10.25 - 9.5 = 0.75)

  vad_data->contador_posibles = 0; //Inicializar a 0
  vad_data->contador_segmentos = 0;

  return vad_data;
}

// CORRECCIÓN: Resolver estados transitorios al cierre
VAD_STATE vad_close(VAD_DATA *vad_data) {
  
  VAD_STATE state = vad_data->state;

  if (state == ST_POSIBLE_V) {
      state = ST_VOICE; 
  } else if (state == ST_POSIBLE_S || state == ST_INIT || state == ST_UNDEF) {
      state = ST_SILENCE; 
  }

  free(vad_data);
  return state; 
}

unsigned int vad_frame_size(VAD_DATA *vad_data) {
  return vad_data->frame_length;
}

/* * TODO: Implement the Voice Activity Detection 
 * using a Finite State Automata
 */

VAD_STATE vad(VAD_DATA *vad_data, float *x) {

  Features f = compute_features(x, vad_data->frame_length);
  vad_data->last_feature = f.p; /* Potencia acutal: save feature, in case you want to show */

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
   * CORRECCIÓN DE LÓGICA: Si la potencia BAJA -> vuelve al estado de S
   * Si la potencia se MANTIENE ALTA -> confirma V
   */
    if (f.p > vad_data ->p1){ // La potencia se mantiene alta (confirmación)
      vad_data->contador_posibles++;
      if(vad_data->contador_posibles >=N_POSIBLES)
      vad_data->state = ST_VOICE;
      vad_data->contador_segmentos = 1; // Reinicia el contador de segmentos
      
    } else { // Cae la potencia antes de la confirmación (falla la confirmación)
      vad_data->state = ST_SILENCE;
      vad_data->contador_segmentos = 1;
    }
    break;

  case ST_VOICE: 
    vad_data->contador_segmentos++;
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
   * CORRECCIÓN DE LÓGICA: Si la potencia SUBE -> vuelve al estado de V
   * Si la potencia se MANTIENE BAJA -> confirma S
   */
    if (f.p < vad_data ->p0){ // La potencia se mantiene baja (confirmación)
      vad_data->contador_posibles++;
      if(vad_data->contador_posibles >=N_POSIBLES)
      vad_data->state = ST_SILENCE;
      vad_data->contador_segmentos = 1;

    } else { // Sube la potencia antes de la confirmación (falla la confirmación)
      vad_data->state = ST_VOICE;
      vad_data->contador_segmentos = 1;
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