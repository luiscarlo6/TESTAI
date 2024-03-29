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
#include <stdint.h>

using namespace std;

uint64_t evaluados = 0;
uint64_t expandidos = 0;

int negamax(state_t s, int depth, bool color){
    int player = color ? 1 : -1;
    //s.terminal() ? cout << "si es terminal" : cout << "nope";
    if(depth == 0 || s.terminal())
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
        for(int i = 0; i < childNodes.size(); ++i)
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

int negamaxAB(state_t s, int depth, int alpha, int beta, bool color){
    int player = color ? 1 : -1;
    //s.terminal() ? cout << "si es terminal" : cout << "nope";
    if(depth == 0 || s.terminal())
        return player * s.value();

    int bestValue = INT_MIN;
    int value;
    std::vector<int> childNodes = s.get_children(color);
    //cout << "Size c: " << childNodes.size();

    if(childNodes.size() <= 0)
        return -negamaxAB(s, depth - 1, -beta, -alpha, !color);
    else
    {
        expandidos++;
        for(int i = 0; i < childNodes.size(); ++i)
        {
            state_t new_s = s.move(color, childNodes[i]);

            //cout << "depth: " << depth;
            //s.terminal() ? cout << "si es terminal" : cout << "nope";
            //s.print(cout, depth);
            value = -negamaxAB(new_s, depth - 1, -beta, -alpha, !color);
            evaluados++;
            bestValue = MAX(bestValue, value);
            alpha = MAX(alpha,value);
            if (alpha >= beta)
                break;
        }
    }

    return bestValue;
}


int negaScout(state_t s, int depth, int alpha, int beta, bool color){
    int player = color ? 1 : -1;
    //s.terminal() ? cout << "si es terminal" : cout << "nope";
    if(depth == 0 || s.terminal())
        return player * s.value();

    int bestValue = beta;
    int value;
    bool first_child = true;
    std::vector<int> childNodes = s.get_children(color);
    //cout << "Size c: " << childNodes.size();

    if(childNodes.size() <= 0)
        return -negaScout(s, depth - 1, -beta, -alpha, !color);
    else
    {
        expandidos++;
        for(int i = 0; i < childNodes.size(); ++i)
        {
            state_t new_s = s.move(color, childNodes[i]);
            if (!first_child){
                first_child = false;
                value = -negaScout(new_s, depth - 1, -alpha-1, -alpha, !color);
                evaluados++;
                if (value > alpha && value < beta)
                {
                    value = -negaScout(new_s, depth - 1, -beta, -value, !color);
                    evaluados++;
                }
            }
            else{
                value = -negaScout(new_s, depth - 1, -beta, -alpha, !color);
                evaluados++;
            }
            alpha = MAX(alpha,value);

            if (alpha >= beta)
                break;            
        }
    }
    return alpha;
}

/* max = color, if !max then its min */
int test(state_t s, int depth, int value, bool max, bool player, bool operand){
    if(depth == 0 || s.terminal()) {
        if(operand)
            return s.value() > value;
        else
            return s.value() >= value;
    }

    std::vector<int> childNodes = s.get_children(player);

    bool t;
    expandidos++;
    for(int i = 0; i < childNodes.size(); ++i){
        state_t new_s = s.move(player, childNodes[i]);
        t = test(new_s, depth - 1, value, !max, !player, true);
        evaluados++;

        if(max && t) return true;

        if(!max && !t) return false;
    }
    return max ? false : true;

}

int scout(state_t s, int depth, bool color){
    if(depth == 0 || s.terminal()) return s.value();

    int score = 0;
    std::vector<int> childNodes = s.get_children(color);
    expandidos++;
    if(childNodes.size() <= 0)
        score = scout(s, depth -1, !color);
    else{
        for(int i = 0; i < childNodes.size(); ++i){
            state_t new_s = s.move(color, childNodes[i]);
            if(childNodes[i] == childNodes[0]) {
                evaluados++;
                score = scout(new_s, depth -1, !color);
            }else{
                if(color && test(new_s, depth - 1, score, !color, !color, true)){
                    score = scout(new_s, depth - 1, !color);
                    evaluados++;
                }
                if(!color && !test(new_s, depth - 1, score, !color, !color, false)){
                    score = scout(new_s, depth - 1, !color);
                    evaluados++;
                }
            }
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


        for (int i = 0; i < 33 - depth; ++i)
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
                result = seed * negamax(state, depth, player);
                cout << "Resultado de Negamax: " << result << "\nEvaluados: " << evaluados<< "\nExpandidos: " << expandidos<< endl;
                //cout << "Resultado de Negamax: " << result << endl;
                break;
            case(2):
                result = seed * negamaxAB(state, depth, INT_MIN, INT_MAX, player);
                cout << "Resultado de Negamax alpha-beta: " << result << "\nEvaluados: " << evaluados<< "\nExpandidos: " << expandidos<< endl;
                break;
            case(3):
                result = scout(state, 33 - depth, player);
                cout << "Resultado de scout: " << result << "\nEvaluados: " << evaluados<< "\nExpandidos: " << expandidos<< endl;
                break;
            case(4):
                result = seed * negaScout(state, depth, INT_MIN, INT_MAX, player);
                cout << "Resultado de nega scout: " << result << "\nEvaluados: " << evaluados<< "\nExpandidos: " << expandidos<< endl;
                break;
        }


        
    }else
        help();
    return 0;
}