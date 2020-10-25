#ifndef STRUCTURE_H
#define structure_H

//constructeur poidds biais +delta et activer pour la backprop les activation
typedef struct neuron_t
{
    float factiver;
    float *fout_weights;
    float fbiais;

    float *fdelta_weights;
    float fdelta_biais;


} neuron;

neuron Init_neuron(int inbPoid);





#endif //UNTITLED_LIBRARY_H
