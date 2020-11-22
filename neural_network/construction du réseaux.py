# -*- coding: utf-8 -*-
"""
Created on Thu Oct 22 11:04:46 2020

@author: luluf
"""

poids_pour_xor = ([1,-1],[-1,1])                    #Marche pour la fonction XOR, pas pour la fonction finale du réseau de neurones
valeurs_entrées_xor  = [0,1]                        #Entrée obligatoire pour la fonction XOR
biais_pour_xor = 0                          
Lout = [0,0]
L = [0,0]                                           #Marche dans le cas de la fonction Xor. Dans le réseau final,
                                                    #il faudra faire une liste de biais pour que chaque neurone possède un biais. 


def Couche_de_neurones():
    out = 0
    i = 0
    while i < 2:                                            
        out = 0
        j = 0
        while j < 2:
            out += poids_pour_xor[i][j] * L[j]
            j+=1
        out += biais_pour_xor
        if out > 0:
            Lout[i] = 1
        else:
            Lout[i] = 0
        i+=1
   
                                                   #(resultat = 1/(1+math.exp(-out))) cette équation n'est pas nécessaire dans notre réseau de neurone pour XOR
                                                   #En effet, en utilisant des perceptrons nos valeurs de sortie sont soit égale à 0 soit égale à 1. 
                                                   #C'est pour cela que l'on a le if else ci-contre.
 
def fonction_xor(x,y):
    L[0] = x
    L[1] = y
    Couche_de_neurones()
    return Lout[x] + Lout[y]


    
    
    
    
        
        
            