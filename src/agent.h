/*
 * agent.h
 *
 *  Created on: Jul 24, 2011
 *      Author: Martin Biehl
 */

#ifndef AGENT_H_

#include "boost/multi_array.hpp"
#include <vector>
#include <string>


typedef boost::multi_array<unsigned int, 1> f_vector_type; //the frequency array type

template < class state_type, class input_type, class output_type >
struct agent_conf {
	//todo let the name be generated from the functions pointed to below
	std::string name;
	int number_of_states;
	int number_of_outputs;
	state_type (*dynamics) (state_type, input_type, int,double, unsigned int);
	output_type (*output) (state_type,int,unsigned int);
	state_type (*init) (int);
};

template < class state_type, class input_type, class output_type >
class Agent {
public:
	//default constructor
//	Agent(){
//		number_of_states = 1;
//		number_of_outputs = 1;
//		states.resize(1);
//		outputs.resize(1);
//		state_frequencies.resize(boost::extents[1]);
//		output_frequencies.resize(boost::extents[1]);
//	}
	//todo: the statenumber and outputnumber break the template!!
	Agent(/*vector<input_type>* input,*/ unsigned int timesteps, agent_conf<state_type,input_type,output_type> conf) {
		number_of_states = conf.number_of_states;
		number_of_outputs = conf.number_of_outputs;
		dynamics = conf.dynamics;
		output = conf.output;
		init = conf.init;
		states.resize(timesteps);
		outputs.resize(timesteps);
		state_frequencies.resize(boost::extents[number_of_states]);
		output_frequencies.resize(boost::extents[number_of_outputs]);


	};
	Agent(/*vector<input_type>* input,*/ unsigned int timesteps, int statenumber, int outputnumber) {
		number_of_states = statenumber;
		number_of_outputs = outputnumber;
		states.resize(timesteps);
		outputs.resize(timesteps);
		state_frequencies.resize(boost::extents[number_of_states]);
		output_frequencies.resize(boost::extents[number_of_outputs]);


	};
	virtual ~Agent(){
	    	states.clear();
	    	outputs.clear();

	    };

	state_type (*dynamics) (state_type, input_type, int,double, unsigned int);

	output_type (*output) (state_type, int, unsigned int);

	state_type (*init) (int);

	void frequencyupdate(state_type *s, output_type *o) {
		state_frequencies[*s] = state_frequencies[*s]+1;
		output_frequencies[*o] = output_frequencies[*o]+1;
	}

public:
	int number_of_states;
	int number_of_outputs;
	//todo: separate out of the agent, the list of past states, and put them into the supervisor
	//the agent should only contain its present state, and output. and publish those.
	std::vector<state_type> states;
	std::vector<output_type> outputs;
	//todo these frequencies are only used by measures, so they should be recorded by the measure
	//that needs them
	f_vector_type state_frequencies;
	f_vector_type output_frequencies;

};

#define AGENT_H_


#endif /* AGENT_H_ */
