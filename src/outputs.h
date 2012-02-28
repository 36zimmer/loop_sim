/*
 * outputs.h
 *
 *  Created on: Jul 25, 2011
 *      Author: mab
 *
 *      output functions to choose from
 */

#ifndef OUTPUTS_H_
#define OUTPUTS_H_

#include <math.h>

int paste_output(int state, int number_of_outputs, unsigned int time) {
	return state % number_of_outputs;
}

int add_one_output(int state, int number_of_outputs, unsigned int time) {
	return (state + 1) % number_of_outputs;
}
int zero_output(int state, int number_of_outputs, unsigned int time) {
	return 0;
}
int e_output(int state,int number_of_environment_outputs, unsigned int time) {
	double q = .2;
	//outputs the state or state "minus" one
	return (state + ((((double)rand()/(double)RAND_MAX) > q) ? (number_of_environment_outputs-1) : 0)) % number_of_environment_outputs;
//	return state % number_of_environment_outputs;
}
int nbcircle_output(int state,int number_of_environment_outputs, unsigned int time) {
	double q = .8;
	//outputs the state or state "minus" one
	return (state + ((((double)rand()/(double)RAND_MAX) > q) ? (number_of_environment_outputs-1) : 0)) % number_of_environment_outputs;
//	return state % number_of_environment_outputs;
}
int nbfourstateactive_output(int state,int number_of_environment_outputs, unsigned int time) {

	return (state == 2 ? 1 : 0);

}


#endif /* OUTPUTS_H_ */
