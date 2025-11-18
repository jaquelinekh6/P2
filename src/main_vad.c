#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sndfile.h>

#include "vad.h"
#include "vad_docopt.h"

#define DEBUG_VAD 0x1

int main(int argc, char *argv[]) {
  int verbose = 0; /* To show internal state of vad: verbose = DEBUG_VAD; */

  SNDFILE *sndfile_in, *sndfile_out = 0;
  SF_INFO sf_info;
  FILE *vadfile;
  int n_read = 0, i;

  VAD_DATA *vad_data;
  VAD_STATE state, last_state;

  float *buffer, *buffer_zeros;
  int frame_size;         /* in samples */
  float frame_duration;   /* in seconds */
  unsigned int t, last_t; /* in frames */
  float alpha1; //MARGEN PARA PASAR DE SILENCIO A VOZ 
  //float alpha2; //AMRGEN PARA PASAR DE VOZ A SILENCIO 

  char  *input_wav, *output_vad, *output_wav;

  DocoptArgs args = docopt(argc, argv, /* help */ 1, /* version */ "2.0");

  verbose    = args.verbose ? DEBUG_VAD : 0;
  input_wav  = args.input_wav;
  output_vad = args.output_vad;
  output_wav = args.output_wav;
  alpha1     = atof(args.alpha1); //Para que el usuario pueda configurar
 

  if (input_wav == 0 || output_vad == 0) {
    fprintf(stderr, "%s\n", args.usage_pattern);
    return -1;
  }

  /* Open input sound file */
  if ((sndfile_in = sf_open(input_wav, SFM_READ, &sf_info)) == 0) {
    fprintf(stderr, "Error opening input file %s (%s)\n", input_wav, strerror(errno));
    return -1;
  }

  if (sf_info.channels != 1) {
    fprintf(stderr, "Error: the input file has to be mono: %s\n", input_wav);
    return -2;
  }

  /* Open vad file */
  if ((vadfile = fopen(output_vad, "wt")) == 0) {
    fprintf(stderr, "Error opening output vad file %s (%s)\n", output_vad, strerror(errno));
    return -1;
  }

  /* Open output sound file, with same format, channels, etc. than input */
  if (output_wav) {
    if ((sndfile_out = sf_open(output_wav, SFM_WRITE, &sf_info)) == 0) {
      fprintf(stderr, "Error opening output wav file %s (%s)\n", output_wav, strerror(errno));
      return -1;
    }
  }

  vad_data = vad_open(sf_info.samplerate, alpha1);
  /* Allocate memory for buffers */
  frame_size   = vad_frame_size(vad_data);
  buffer       = (float *) malloc(frame_size * sizeof(float));
  buffer_zeros = (float *) malloc(frame_size * sizeof(float));
  for (i=0; i< frame_size; ++i) buffer_zeros[i] = 0.0F;

  frame_duration = (float) frame_size/ (float) sf_info.samplerate;
  // INICIALIZACIÓN CLAVE: last_state debe ser INIT para el primer segmento.
  last_state = ST_INIT; 

  for (t = last_t = 0; ; t++) { /* For each frame ... */
    /* End loop when file has finished (or there is an error) */
    if  ((n_read = sf_read_float(sndfile_in, buffer, frame_size)) != frame_size) break;

    if (sndfile_out != 0) {
      /* TODO: copy all the samples into sndfile_out */
    }

    state = vad(vad_data, buffer);
    if (verbose & DEBUG_VAD) vad_show_state(vad_data, stdout);
    
    // Si el estado actual es un estado DEFINITIVO (S o V) y ha habido un cambio, 
    // imprimimos el segmento anterior.
    if ((state == ST_SILENCE || state == ST_VOICE) && (state != last_state)) { 
        
        // El segmento anterior (last_state) ha finalizado en el tiempo 't'.
        
        // 1. Manejo del segmento anterior (ya sea S o V)
        if (last_state == ST_SILENCE || last_state == ST_VOICE) {
            fprintf(vadfile, "%.5f\t%.5f\t%s\n", 
                    last_t * frame_duration, 
                    t * frame_duration, 
                    state2str(last_state));
            
        } 
        // 2. Manejo de la Inicialización (De ST_INIT a ST_SILENCE)
        else if (last_state == ST_INIT) {
            // Este es el primer segmento (de 0.0 hasta el final de la inicialización).
            // Lo etiquetamos como SILENCIO.
            fprintf(vadfile, "%.5f\t%.5f\t%s\n", 
                    0.0F, // Comienza en 0.0
                    t * frame_duration, 
                    state2str(ST_SILENCE));
        }

        // 3. Actualizamos las variables para el nuevo segmento
        // last_state es ahora el estado definitivo 'state' (S o V)
        last_state = state; 
        last_t = t;
    }
    
    // NOTA: Si 'state' es un estado transitorio (POSIBLE_V/S), no se imprime nada,
    // y 'last_state' se mantiene, esperando a que el estado se confirme o se revierta.

    if (sndfile_out != 0) {
      /* TODO: go back and write zeros in silence segments */
    }
  }

  // Cierre: vad_close se encarga de resolver el estado final (que es ahora S o V)
  state = vad_close(vad_data); 
  /* TODO: what do you want to print, for last frames? */
  // Se imprime el último segmento si hay frames pendientes.
  if (t != last_t)
    fprintf(vadfile, "%.5f\t%.5f\t%s\n", 
            last_t * frame_duration, 
            t * frame_duration + n_read / (float) sf_info.samplerate, 
            state2str(state)); // 'state' ya está resuelto a S o V por vad_close

  /* clean up: free memory, close open files */
  free(buffer);
  free(buffer_zeros);
  sf_close(sndfile_in);
  fclose(vadfile);
  if (sndfile_out) sf_close(sndfile_out);
  return 0;
}