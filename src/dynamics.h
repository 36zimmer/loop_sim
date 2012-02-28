/*
 * dynamics.h
 *
 *  Created on: Jul 25, 2011
 *      Author: mab
 *
 *      dynamics functions to choose from.
 *      if possible they should work for all numbers of states
 */

#ifndef DYNAMICS_H_
#define DYNAMICS_H_

//dynamics take parameters p and time, p can be used to adapt the dynamics between repetitions
//time  to adapt between timesteps
//todo change p into r for repetition and take the repetition number as int directly from the main
//if the parameters are not used in the dynamics there is no need to worry about them
int fixed_dynamics(int last_state, int last_input,int number_of_states,  double p,unsigned int time) {
	return last_state;
//	return ((last_a_state + 1)/** (1+(int)sin (1/20*time))*/) % number_of_agent_states;
}


int random_dynamics(int last_a_state, int last_e_output,int number_of_states,  double p,unsigned int time) {
	return rand() % number_of_states;
//	return ((last_a_state + 1)/** (1+(int)sin (1/20*time))*/) % number_of_agent_states;
}

int parametric_random_walk(int last_state, int last_input,int number_of_states,  double p,unsigned int time) {
	assert((number_of_states % 2)==1);
	//passes a random number between 0 and 1 to 'distribute_p'
	//returns either deterministically the last state (for p=0) or
	//for increasing p up to 1 a more and more equally distributet state
	//start at the random value between 0 and 1, ist is scaled to between 0 and 2 then shifted to between -1 and 1
	//then it is redistributed acoording to distribute_p which returns a value again between -1 and 1, this intervall
	//is scaled and a step length (psitive or negative) is extracted which is added to the last state to  get a new one
	// its complicated but at least for uneven numbers of states it generates random steps with increasing variance.
	double x = ((double)((number_of_states-1)/2)+0.5) * distribute_p((double)rand()/(double)RAND_MAX*2.0-1.0,p);
	//because c++ % operation is not a true modulo, we have cannot return y directly
	double y = (last_state + (int)copysign(fabs(x)+0.5,x)) % number_of_states;
	return (y<0) ? y + number_of_states : y;




	//return rand() % number_of_states;
//	return ((last_a_state + 1)/** (1+(int)sin (1/20*time))*/) % number_of_agent_states;
}

int copy_dynamics(int last_state, int last_input,int number_of_states,  double p,unsigned int time) {
	return last_input % number_of_states;
//	return ((last_a_state + 1)/** (1+(int)sin (1/20*time))*/) % number_of_agent_states;
}

int e_dynamics(int last_e_state, int last_a_output,int number_of_environment_states, double p, unsigned int time) {
	return ( last_e_state + ((((double)rand()/(double)RAND_MAX) > p) ? 1 : 0)) % number_of_environment_states;
//	return ((last_e_state+last_a_output)/** (1+(int)cos (1/20*time))*/) % number_of_environment_states;
}

int switch_or_stay(int last_state, int input,int number_of_states,  double p,unsigned int time) {
	return (input == 1) ? last_state : ((last_state + 1) % number_of_states);
//	return ((last_a_state + 1)/** (1+(int)sin (1/20*time))*/) % number_of_agent_states;
}

int nb94a(int last_state, int input,int number_of_states, double p, unsigned int time) {
	assert((input == 0) || (input == 1));
	assert((last_state == 0) || (last_state == 1));
	//state 1 is state B in the thesis
	if(last_state == 1)
		return (input == 1) ? 1 : 0;
	//if last state is A and
	else if(input == 1)
		return ((((double)rand()/(double)RAND_MAX) > p) ? 1 : 0);
	else
		return ((((double)rand()/(double)RAND_MAX) > p) ? 0 : 1);
}
int nb94b(int last_state, int input,int number_of_states, double p, unsigned int time) {
	assert((input == 0) || (input == 1));
	assert((last_state == 0) || (last_state == 1));
	//state 1 is state B in the thesis
	if(last_state == 1)
		return (input == 1) ? 1 : 0;
	//if last state is A and
	else if(input == 1)
		return  1;
	else
		return ((((double)rand()/(double)RAND_MAX) > p) ? 0 : 1);
}


int agent16(int last_state, int last_input,int number_of_states,  double p,unsigned int time) {
	if (((last_input == 3) && (last_state == 0))
			|| ((last_input == 0) && (last_state == 1))
			|| ((last_input == 0) && (last_state == 3))
			|| ((last_input == 3) && (last_state == 1)))
		return 0;
	else if (((last_input == 2) && (last_state == 0))
			|| ((last_input == 0) && (last_state == 2))
			|| ((last_input == 2) && (last_state == 2))
			|| ((last_input == 3) && (last_state == 2)))
		return 1;
	else if (((last_input == 1) && (last_state == 0))
			|| ((last_input == 1) && (last_state == 1))
			|| ((last_input == 2) && (last_state == 3))
			|| ((last_input == 3) && (last_state == 3)))
		return 2;
	else if (((last_input == 0) && (last_state == 0))
			|| ((last_input == 2) && (last_state == 1))
			|| ((last_input == 1) && (last_state == 2))
			|| ((last_input == 1) && (last_state == 3)))
		return 3;
	else
		assert(0==1);
}
int environment16(int last_state, int last_input,int number_of_states,  double p,unsigned int time) {
	//!!!!note that state and input changed position compared to the agent16!!!!
	if (/*((last_state == 3) && */(last_input == 0))
/*			|| ((last_state == 2) && (last_input == 0))
			|| ((last_state == 1) && (last_input == 0))
			|| ((last_state == 0) && (last_input == 0)))*/
		return 0;
	else if (((last_state == 0) && (last_input == 1))
			|| ((last_state == 0) && (last_input == 2))
			|| ((last_state == 1) && (last_input == 1))
			|| ((last_state == 2) && (last_input == 1)))
		return 1;
	else if (((last_state == 0) && (last_input == 3))
			|| ((last_state == 2) && (last_input == 2))
			|| ((last_state == 2) && (last_input == 3))
			|| ((last_state == 1) && (last_input == 2)))
		return 2;
	else if (((last_state == 3) && (last_input == 1))
			|| ((last_state == 3) && (last_input == 2))
			|| ((last_state == 3) && (last_input == 3))
			|| ((last_state == 1) && (last_input == 3)))
		return 3;
	else
		assert(0==1);
}

int paraminfluence(int last_state, int last_input,int number_of_states,  double p,unsigned int time) {
	//the state here follows the input, how fast is dependent on p. for p=number_of_states this is just like copy,
	//for p = 0 it is like fixed.
	//p controls how many steps into the direction of last_input starting at last_state, the next state can maximally take

	return last_state + ((last_input - last_state <= (int)p) ? (last_input - last_state) : copysign((int)p,last_input - last_state));
}

int nbcircle_dynamics(int last_e_state, int last_a_output,int number_of_environment_states, double p, unsigned int time) {
	return last_a_output == 0 ? (( last_e_state + ((((double)rand()/(double)RAND_MAX) <= p) ? 1 : 0)) % number_of_environment_states) : 0;
//	return ((last_e_state+last_a_output)/** (1+(int)cos (1/20*time))*/) % number_of_environment_states;
}
int nbtwostateactive_dynamics(int last_state, int last_input,int number_of_states,  double p,unsigned int time) {
	if (((last_state == 1) && (last_input != 3))
				|| ((last_state == 0) && (last_input == 2))
/*				|| ((last_state == 1) && (last_input == 0))
				|| ((last_state == 0) && (last_input == 0))*/)
			return 0;
		else if (((last_state == 0) && (last_input != 2))
				|| ((last_state == 1) && (last_input == 3))
/*				|| ((last_state == 1) && (last_input == 1))
				|| ((last_state == 2) && (last_input == 1))*/)
			return 1;
		else
			assert(0==1);
}
int nbfourstateactive_dynamics(int last_state, int last_input,int number_of_states,  double p,unsigned int time) {
	if ((last_state == 2)
			|| ((last_state == 0) && (last_input == 2)))
		return 0;
//	else if (((last_state == 0) && (last_input == 1))
//			|| ((last_state == 0) && (last_input == 2))
//			|| ((last_state == 1) && (last_input == 1))
//			|| ((last_state == 2) && (last_input == 1)))
//		return 1;
	else if (((last_state == 0) && (last_input == 3))
			|| ((last_state == 1) && (last_input == 0))
			|| ((last_state == 1) && (last_input == 2))
			|| ((last_state == 1) && (last_input == 3))
			|| ((last_state == 3) && (last_input != 3)))
		return 2;
	else if (((last_state == 0) && (last_input == 0))
			|| ((last_state == 0) && (last_input == 1))
			|| ((last_state == 1) && (last_input == 1))
			|| ((last_state == 3) && (last_input == 3)))
		return 3;
	else
		assert(0==1);
}
int nbtwostatepassive_dynamics(int last_state, int last_input,int number_of_states,  double p,unsigned int time) {
	if (((last_input == 1))
				|| ((last_input == 3))
/*				|| ((last_state == 1) && (last_input == 0))
				|| ((last_state == 0) && (last_input == 0))*/)
			return 0;
		else if (((last_input == 0))
				|| ((last_input == 2))
/*				|| ((last_state == 1) && (last_input == 1))
				|| ((last_state == 2) && (last_input == 1))*/)
			return 1;
		else
			assert(0==1);
}
int nbfourstatepassive_dynamics(int last_state, int last_input,int number_of_states,  double p,unsigned int time) {
	if ((last_state == 2)
			|| ((last_state != 3) && (last_input == 3))
			|| ((last_state == 1) && (last_input == 0))
			|| ((last_state == 3) && (last_input == 0)))
		return 0;
	else if (((last_state != 2) && (last_input == 2))
			|| ((last_state == 1) && (last_input == 1))
			|| ((last_state == 3) && (last_input == 1))
			|| ((last_state == 3) && (last_input == 3)))
		return 1;
	else if (((last_state == 0) && (last_input == 0))
			|| ((last_state == 0) && (last_input == 1))
			|| ((last_state == 2) && (last_input == 0)))
		return 2;
	else if (((last_state == 2) && (last_input == 1))
			|| ((last_state == 2) && (last_input == 2)))
		return 3;
	else
		assert(0==1);
}
int sine_dynamics(int last_state, int last_input,int number_of_states,  double p,unsigned int time) {
	//this is rather a triangle wave, but whatever.
	return abs(2*((time/2)  % (number_of_states-1))-(number_of_states-1));
}
int memtest1_dynamics(int last_state, int last_input,int number_of_states,  double p,unsigned int time) {
	int trajectory[12] = {2, 2, 3, 2, 2, 1, 2, 2, 3, 2, 2, 4};
	//this is rather a triangle wave, but whatever.
	return trajectory[time % 12];
}


#endif /* DYNAMICS_H_ */
