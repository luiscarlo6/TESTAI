/*
 *  Copyright (C) 2012 Universidad Simon Bolivar
 * 
 *  Permission is hereby granted to distribute this software for
 *  non-commercial research purposes, provided that this copyright
 *  notice is included with any such distribution.
 *  
 *  THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
 *  EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE.  THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE
 *  SOFTWARE IS WITH YOU.  SHOULD THE PROGRAM PROVE DEFECTIVE, YOU
 *  ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR CORRECTION.
 *  
 *  Blai Bonet, bonet@ldc.usb.ve
 *
 *  Last revision: 11/08/2012
 *
 */

#include "othello_cut.h" // won't work correctly until .h is fixed!
#include <iostream>
#include <climits>

using namespace std;

int evaluados = 0;
int expandidos = 0;

int negamax(state_t s, int depth, bool color){
    int player = color ? 1 : -1;
    //s.terminal() ? cout << "si es terminal" : cout << "nope";
    if(s.terminal())
        return player * s.value();

    int bestValue = INT_MIN;
    int value;
    std::vector<int> childNodes = s.get_children(color);
    //cout << "Size c: " << childNodes.size();

    if(childNodes.size() <= 0)
        return -negamax(s, depth - 1, !color);
    else
    {
        expandidos++;
        for(int i = 0; i < childNodes.size(); i++)
        {
            state_t new_s = s.move(color, childNodes[i]);

            //cout << "depth: " << depth;
            //s.terminal() ? cout << "si es terminal" : cout << "nope";
            //s.print(cout, depth);
            value = -negamax(new_s, depth - 1, !color);
            evaluados++;
            bestValue = MAX(bestValue, value);
        }
    }

    return bestValue;
}

/* max = color, if !max then its min */
int test(state_t s, int depth, int value, bool color){
    if(depth == 0 || s.terminal()) return s.value() > value;

    std::vector<int> childNodes = s.get_children(color);

    if(childNodes.size() <= 0)
        return color;

    else{
        bool t;
        for(int i = 0; i < childNodes.size(); i++){
            state_t new_s = s.move(color, childNodes[i]);
            t = test(new_s, depth - 1, value, !color);
            if(color && t) return true;
            
            if(!color && !t) return false;

            return color ? true : false;
        }
    }

}

int scout(state_t s, int depth, bool color){
    if(depth == 0 || s.terminal()) return s.value();

    int score = 0;
    std::vector<int> childNodes = s.get_children(color);

    for(int i = 0; i < childNodes.size(); i++){
        state_t new_s = s.move(color, childNodes[i]);
        if(childNodes[i] == childNodes[0]) 
            score = scout(new_s, depth -1, !color);
        else{
            if(color && test(new_s, depth - 1, score, true)) 
                score = scout(new_s, depth - 1, !color);
            if(!color && test(new_s, depth - 1, score, false))
                score = scout(new_s, depth - 1, !color);
        }
    }
    return score;
}

int help(){
    cout << "Número invalido de argumentos o argumentos incorrectos, por favor ejecute de la forma: ";
    cout << "./main <Algoritmo> <Profundidad>" << endl;
    cout << "Algoritmo:\n1. Negamax\n2. Negamax con poda alpha-beta\n3. Scout\n4. Negascout"<<endl;
    cout << "Profundidad: Número entre 1-33"<<endl;
    return 0;
}

int main(int argc, const char **argv) {
    state_t state;
    int color = -2;
    int alg;
    int depth;
    int result = 0;
    int seed = 0;
    bool player;


    if (argc == 3){
        alg = atoi(argv[1]);
        depth = atoi(argv[2]);
        if (!(alg>0 && alg<5) || !(depth>0 && depth<34)){
            help();
            return -1;
        }


        for (int i = 0; i < depth; ++i)
        {
            player = i % 2 == 0; // black moves first!
            int pos = PV[i];
            state = state.move(player, pos);
        }


        player = !player;
        seed = player ? 1 : -1;

        switch(alg){
            case(1)://Negamax
                //result = seed * negamax(state, depth, player);
                result = seed * negamax(state, 33 - depth, player);
                cout << "Resultado de Negamax: " << result << "\nEvaluados: " << evaluados<< "\nExpandidos: " << expandidos<< endl;
                //cout << "Resultado de Negamax: " << result << endl;
                break;
            case(2):
                break;
            case(3):
                result = scout(state, 33 - depth, player);
                cout << "Resultado de scout: " << result << endl;
                break;
            case(4):
                break;
        }


        
    }else
        help();
    return 0;
}