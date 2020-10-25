#include "structure.h"

#include <stdio.h>
#include <math.h>


//les *marque un pointeur

neuron Init_neuron(int iNbPoid)
{
    neuron neuron;

    neuron.factiver = 0.0;
    neuron.fout_weights = (float*) (iNbPoid);
    neuron.fbiais=0.0;

    neuron.fdelta_weights = (float*) (iNbPoid);
    neuron.fdelta_biais = 0.0;


    return neuron;
}








