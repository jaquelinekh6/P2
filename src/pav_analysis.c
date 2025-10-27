#include <math.h>
#include "pav_analysis.h"
// Calcula la potencia media en decibelios de la señal
float compute_power(const float *x, unsigned int N)
{
    float acc = 0.0f;
    // Suma el cuadrado de cada muestra
    for (unsigned int idx = 0; idx < N; ++idx)
    {
        acc += x[idx] * x[idx];
    }
    float mean = acc / (float)N; // Media de los cuadrados
    return 10.0f * log10f(mean); // Conversión a dB
}
// Calcula la amplitud media de la señal
float compute_am(const float *x, unsigned int N)
{
    float total = 0.0f;
    // Suma el valor absoluto de cada muestra
    for (unsigned int j = 0; j < N; j++)
    {
        total += fabsf(x[j]);
    }
    return total / (float)N; // Media de los valores absolutos
}
// Calcula la tasa de cruces por cero (ZCR)
float compute_zcr(const float *x, unsigned int N, float fm)
{
    unsigned int crossings = 0;
    // Cuenta cuantas veces la señal cruza el eje cero
    for (unsigned int k = 1; k < N; ++k)
    {
        if ((x[k - 1] < 0 && x[k] >= 0) || (x[k - 1] >= 0 && x[k] < 0))
        {
            crossings++;
        }
    }
    // Formula para la tasa de cruces por cero
    float rate = ((float)crossings * fm) / (2.0f * (N - 1));
    return rate;
}