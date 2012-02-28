/*
 * initializations.h
 *
 *  Created on: Jul 25, 2011
 *      Author: mab
 *
 * lists initialization functions to choose from
 */

#ifndef INITIALIZATIONS_H_
#define INITIALIZATIONS_H_
#include <math.h>
#include <time.h>



int random_state(int number_of_states) {
//	cout << rand() << endl;
	return rand() % number_of_states;
}

int state_zero(int number_of_states) {
	return 0;
}

int state_two(int number_of_states) {
	return 2;
}
//
//template <class state_type>
//struct init_agent<state_type> {
//	state_type operator() (int number_of_states) {
//		return 10 % number_of_states;
//	}
//	std::string name;
//
//};


//
int init_environment(int number_of_states) {
//	cout << rand() << endl;
//	return rand() % number_of_states;
	return 10 % number_of_states;
}


#endif /* INITIALIZATIONS_H_ */
