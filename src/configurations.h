/*
 * configurations.h
 *
 *  Created on: Jul 25, 2011
 *      Author: mab
 *
 *      lists configurations for agents and environments (there really is no difference)
 *      the agent_conf structure is defined in agent.h
 */

#ifndef CONFIGURATIONS_H_
#define CONFIGURATIONS_H_

#include "initializations.h"
#include "dynamics.h"
#include "outputs.h"

//template < class state_type, class input_type, class output_type >
//agent_conf<state_type,input_type,output_type> getAgentConf(){
//	agent_conf<state_type,input_type,output_type> c;
//	c.name = "standard_agent";
//	c.number_of_states = 2;
//	c.number_of_outputs = 2;
//	c.init = &state_zero;
//	c.dynamics = &fixed_dynamics;
//	c.output = &paste_output;
//	return c;
//}


template < class state_type, class input_type, class output_type >
agent_conf<state_type,input_type,output_type> getFixedAgentConf(int n_of_states, int n_of_outputs){
	agent_conf<state_type,input_type,output_type> c;
	c.name = "fixed";
	c.number_of_states = n_of_states;
	c.number_of_outputs = n_of_outputs;
	c.init = &state_zero;
	c.dynamics = &fixed_dynamics;
	c.output = &paste_output;
	return c;
}

template < class state_type, class input_type, class output_type >
agent_conf<state_type,input_type,output_type> getRandomAgentConf(int n_of_states, int n_of_outputs){
	agent_conf<state_type,input_type,output_type> c;
	c.name = "random";
	c.number_of_states = n_of_states;
	c.number_of_outputs = n_of_outputs;
	c.init = &random_state;
	c.dynamics = &random_dynamics;
	c.output = &paste_output;
	return c;
}

template < class state_type, class input_type, class output_type >
agent_conf<state_type,input_type,output_type> getParamRandomAgentConf(int n_of_states, int n_of_outputs){
	agent_conf<state_type,input_type,output_type> c;
	c.name = "paramrandom";
	c.number_of_states = n_of_states;
	c.number_of_outputs = n_of_outputs;
	c.init = &random_state;
	c.dynamics = &parametric_random_walk;
	c.output = &paste_output;
	return c;
}
template < class state_type, class input_type, class output_type >
agent_conf<state_type,input_type,output_type> getCopyAgentConf(int n_of_states, int n_of_outputs){
	agent_conf<state_type,input_type,output_type> c;
	c.name = "copy";
	c.number_of_states = n_of_states;
	c.number_of_outputs = n_of_outputs;
	c.init = &state_zero;
	c.dynamics = &copy_dynamics;
	c.output = &paste_output;
	return c;
}
template < class state_type, class input_type, class output_type >
agent_conf<state_type,input_type,output_type> getEnvironmentConf(int n_of_states, int n_of_outputs){
	agent_conf<state_type,input_type,output_type> c;
	c.name = "standard";
	c.number_of_states = n_of_states;
	c.number_of_outputs = n_of_outputs;
	c.init = &init_environment;
	c.dynamics = &e_dynamics;
	c.output = &e_output;
	return c;
}

template < class state_type, class input_type, class output_type >
agent_conf<state_type,input_type,output_type> getNBAgent93aConf(){
	agent_conf<state_type,input_type,output_type> c;
	c.name = "NBAgent93a";
	c.number_of_states = 2;
	c.number_of_outputs = 2;
	c.init = &state_zero;
	//the agent takes the input as state, state A is here 0 and state B is 1
	c.dynamics = &copy_dynamics;
	//use paste output even though there is no influence on the environment
	c.output = &paste_output;
	return c;
}
template < class state_type, class input_type, class output_type >
agent_conf<state_type,input_type,output_type> getNBAgent93bConf(){
	agent_conf<state_type,input_type,output_type> c;
	c.name = "NBAgent93b";
	c.number_of_states = 2;
	c.number_of_outputs = 2;
	c.init = &state_zero;
	//the agent takes the input as state, state A is here 0 and state B is 1
	c.dynamics = &switch_or_stay;
	//use paste output even though there is no influence on the environment
	c.output = &paste_output;
	return c;
}
template < class state_type, class input_type, class output_type >
agent_conf<state_type,input_type,output_type> getNB2StateEnvironmentConf(){
	agent_conf<state_type,input_type,output_type> c;
	//the environment is the same for 93a) and 93b)
	c.name = "NB2StateEnvironment";
	c.number_of_states = 2;
	c.number_of_outputs = 2;
	c.init = &random_state;
	c.dynamics = &random_dynamics;
	c.output = &paste_output;
	return c;
}
template < class state_type, class input_type, class output_type >
agent_conf<state_type,input_type,output_type> getNBAgent94aConf(){
	agent_conf<state_type,input_type,output_type> c;
	c.name = "NBAgent94a";
	c.number_of_states = 2;
	c.number_of_outputs = 2;
	c.init = &state_zero;
	c.dynamics = &nb94a;
	c.output = &paste_output;
	return c;
}
template < class state_type, class input_type, class output_type >
agent_conf<state_type,input_type,output_type> getNBAgent94bConf(){
	agent_conf<state_type,input_type,output_type> c;
	c.name = "NBAgent94b";
	c.number_of_states = 2;
	c.number_of_outputs = 2;
	c.init = &state_zero;
	c.dynamics = &nb94b;
	c.output = &paste_output;
	return c;
}
template < class state_type, class input_type, class output_type >
agent_conf<state_type,input_type,output_type> getAgent16Conf(){
	agent_conf<state_type,input_type,output_type> c;
	c.name = "Agent16";
	c.number_of_states = 4;
	c.number_of_outputs = 4;
	c.init = &state_zero;
	c.dynamics = &agent16;
	c.output = &paste_output;
	return c;
}
template < class state_type, class input_type, class output_type >
agent_conf<state_type,input_type,output_type> getEnvironment16Conf(){
	agent_conf<state_type,input_type,output_type> c;
	c.name = "Environment16";
	c.number_of_states = 4;
	c.number_of_outputs = 4;
	c.init = &state_zero;
	c.dynamics = &environment16;
	c.output = &paste_output;
	return c;
}
template < class state_type, class input_type, class output_type >
agent_conf<state_type,input_type,output_type> getParamInfluenceConf(int n_of_states, int n_of_outputs){
	agent_conf<state_type,input_type,output_type> c;
	c.name = "paraminfluence";
	c.number_of_states = n_of_states;
	c.number_of_outputs = n_of_outputs;
	c.init = &random_state;
	c.dynamics = &paraminfluence;
	c.output = &paste_output;
	return c;
}
template < class state_type, class input_type, class output_type >
agent_conf<state_type,input_type,output_type> getNBcircleEnvironment(){
	agent_conf<state_type,input_type,output_type> c;
	c.name = "NBcircleEnvironment";
	c.number_of_states = 4;
	c.number_of_outputs = 4;
	c.init = &state_zero;
	c.dynamics = &nbcircle_dynamics;
	c.output = &nbcircle_output;
	return c;
}
template < class state_type, class input_type, class output_type >
agent_conf<state_type,input_type,output_type> getNBtwostateactive(){
	agent_conf<state_type,input_type,output_type> c;
	c.name = "NBtwostateactive";
	c.number_of_states = 2;
	c.number_of_outputs = 2;
	c.init = &state_zero;
	c.dynamics = &nbtwostateactive_dynamics;
	c.output = &paste_output;
	return c;
}
template < class state_type, class input_type, class output_type >
agent_conf<state_type,input_type,output_type> getNBfourstateactive(){
	agent_conf<state_type,input_type,output_type> c;
	c.name = "NBfourstateactive";
	c.number_of_states = 4;
	c.number_of_outputs = 2;
	c.init = &state_zero;
	c.dynamics = &nbfourstateactive_dynamics;
	c.output = &nbfourstateactive_output;
	return c;
}
template < class state_type, class input_type, class output_type >
agent_conf<state_type,input_type,output_type> getNBtwostatepassive(){
	agent_conf<state_type,input_type,output_type> c;
	c.name = "NBtwostatepassive";
	c.number_of_states = 2;
	c.number_of_outputs = 1;
	c.init = &state_zero;
	c.dynamics = &nbtwostatepassive_dynamics;
	c.output = &zero_output;
	return c;
}
template < class state_type, class input_type, class output_type >
agent_conf<state_type,input_type,output_type> getNBfourstatepassive(){
	agent_conf<state_type,input_type,output_type> c;
	c.name = "NBfourstatepassive";
	c.number_of_states = 4;
	c.number_of_outputs = 1;
	c.init = &state_zero;
	c.dynamics = &nbfourstatepassive_dynamics;
	c.output = &zero_output;
	return c;
}
template < class state_type, class input_type, class output_type >
agent_conf<state_type,input_type,output_type> getSineAgentConf(int n_of_states, int n_of_outputs){
	agent_conf<state_type,input_type,output_type> c;
	c.name = "sine";
	c.number_of_states = n_of_states;
	c.number_of_outputs = n_of_outputs;
	c.init = &random_state;
	c.dynamics = &sine_dynamics;
	c.output = &paste_output;
	return c;
}
template < class state_type, class input_type, class output_type >
agent_conf<state_type,input_type,output_type> getMemTest1AgentConf(){
	agent_conf<state_type,input_type,output_type> c;
	c.name = "memtest1";
	c.number_of_states = 5;
	c.number_of_outputs = 5;
	c.init = &state_two;
	c.dynamics = &memtest1_dynamics;
	c.output = &paste_output;
	return c;
}
#endif /* CONFIGURATIONS_H_ */
