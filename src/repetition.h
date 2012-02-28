/*
 * repetition.h
 *
 *  Created on: Jul 24, 2011
 *      Author: Martin Biehl
 */

#ifndef REPETITION_H_

#include "agent.h"
#include "used_functions.h"
#include "boost/multi_array.hpp"

typedef boost::multi_array<unsigned int, 2> f_matrix_type; //the frequency matrix type
typedef boost::multi_array<unsigned int, 3> f_array_type; //the frequency matrix type
typedef boost::multi_array<unsigned int, 4> f_big_array_type; //the big frequency array

template < class a_state_type, class a_output_type , class e_state_type, class e_output_type>
class Repetition {
public:
	Repetition(double prob,unsigned int timesteps, agent_conf<a_state_type,e_output_type,a_output_type> a_conf, agent_conf<e_state_type,a_output_type,e_output_type> e_conf) :
		agent(timesteps,a_conf),
		environment(timesteps,e_conf),
		auto_freq(boost::extents[e_conf.number_of_outputs][a_conf.number_of_outputs]),
		auto_star_freq(boost::extents[a_conf.number_of_states][a_conf.number_of_states]),
		auto_freq2(boost::extents[e_conf.number_of_outputs][a_conf.number_of_outputs][e_conf.number_of_outputs]),
		auto_0_freq(boost::extents[a_conf.number_of_states][a_conf.number_of_states]
		                            [e_conf.number_of_states]),
		auto_2_freq(boost::extents[a_conf.number_of_states][a_conf.number_of_states]
											[e_conf.number_of_states][e_conf.number_of_states][e_conf.number_of_states]),
		auto_5_freq(boost::extents[a_conf.number_of_states][a_conf.number_of_states]
													[e_conf.number_of_states][e_conf.number_of_states]
													 [e_conf.number_of_states][e_conf.number_of_states]
													  [e_conf.number_of_states][e_conf.number_of_states]),
		past_3_freq(boost::extents[e_conf.number_of_outputs][a_conf.number_of_outputs]
												[e_conf.number_of_outputs][e_conf.number_of_outputs][e_conf.number_of_outputs]),
		past_6_freq(boost::extents[e_conf.number_of_outputs][a_conf.number_of_outputs]
														[e_conf.number_of_outputs][e_conf.number_of_outputs]
														 [e_conf.number_of_outputs][e_conf.number_of_outputs]
														  [e_conf.number_of_outputs][e_conf.number_of_outputs])
		{
		//todo, from a repetition conf, or directly fill up here a vector (initialized above like the agent etc.) of (pointers to?) structures that contain
		//names and procedures of the different statistical measures that are to be collected
		T=timesteps;
		p=prob;
	};
	Repetition(unsigned int timesteps, int a_statenumber, int a_outputnumber, int e_statenumber, int e_outputnumber) :
		agent(timesteps,a_statenumber,a_outputnumber),
		environment(timesteps,e_statenumber,e_outputnumber),
		auto_freq(boost::extents[e_outputnumber][a_outputnumber]){
		T=timesteps;
	};

	void initsys(){
		agent.states[0] = agent.init(agent.number_of_states);
		environment.states[0] = (environment.init)(environment.number_of_states);
		agent.outputs[0] = (agent.output)(agent.states[0],agent.number_of_outputs,0);
		environment.outputs[0] = (environment.output)(environment.states[0],environment.number_of_outputs,0);
		agent.frequencyupdate(&agent.states[0],&agent.outputs[0]);
		environment.frequencyupdate(&environment.states[0],&environment.outputs[0]);
		//todo: for each measure call measure.frequencyupdate(pass the address of states and outputs and
		//time or only time,what is better? maybe only time as the measures then onlyuse the random variable
		//they need and do not get passed all four at each timestep)
	}

	void step(unsigned int t) {
		agent.states[t] = (agent.dynamics)(agent.states[t-1],environment.outputs[t-1],agent.number_of_states,p,t);
		environment.states[t] = (environment.dynamics)(environment.states[t-1],agent.outputs[t-1],environment.number_of_states,p,t);
		agent.outputs[t] = (agent.output)(agent.states[t],agent.number_of_outputs,t);
		environment.outputs[t] = (environment.output)(environment.states[t],environment.number_of_outputs,t);
		//todo: for each measure call measure.frequencyupdate(pass the address of states and outputs and
		//time or only time,what is better? maybe only time as the measures then onlyuse the random variable
		//they need and do not get passed all four at each timestep)
	}
	//todo remove the following function, each measure should take care of its own data collection
	//better yet: a data keeper creates a set of frequency matrices to collect in response to
	//requests from the measures (this way the same matrix is not collected multiple times
	void frequencyupdate(unsigned int t) {
		agent.frequencyupdate(&agent.states[t],&agent.outputs[t]);
		environment.frequencyupdate(&environment.states[t],&environment.outputs[t]);
		auto_freq[environment.outputs[t]][agent.outputs[t-1]]+=1;
		auto_freq2[environment.outputs[t]][agent.outputs[t-1]][environment.outputs[t-1]]+=1;
		auto_star_freq[agent.states[t]][agent.states[t-1]]+=1;
		auto_0_freq[agent.states[t]][agent.states[t-1]][environment.states[t-1]]+=1;
		if(t>2){
			auto_2_freq[agent.states[t]][agent.states[t-1]][environment.states[t-1]]
			                                                [environment.states[t-2]]
			                                                 [environment.states[t-3]]+=1;
			past_3_freq[environment.outputs[t]][agent.outputs[t-1]][environment.outputs[t-1]]
			                                                        [environment.outputs[t-2]]
			                                                         [environment.outputs[t-3]]+=1;
		}
		if(t>5){
			auto_5_freq[agent.states[t]][agent.states[t-1]][environment.states[t-1]]
			                             [environment.states[t-2]][environment.states[t-3]]
			                              [environment.states[t-4]][environment.states[t-5]]
			                                                        [environment.states[t-6]]+=1;
			past_6_freq[environment.outputs[t]][agent.outputs[t-1]][environment.outputs[t-1]]
			                                                        [environment.outputs[t-2]]
			                                                         [environment.outputs[t-3]]
			                                                          [environment.outputs[t-4]]
			                                                           [environment.outputs[t-5]]
			                                                            [environment.outputs[t-6]]+=1;
		}
	}

	void getentropies() {
		//todo for each measure call measure.calculate(passing T)
		H_Eo_Ao = entropy(&auto_freq,T-1);
		mutual_inf = mi(auto_freq,T-1);
		H_A = entropy(&agent.state_frequencies, T);
		H_Ao = entropy(&agent.output_frequencies, T);
		H_E = entropy(&environment.state_frequencies, T);
		H_Eo = entropy(&environment.output_frequencies, T);
		aut_star = mi(auto_star_freq,T-1);
		mutual_inf2 = cmi0(&auto_freq2,T-1);
		mutual_inf2_3 = cmi2(&past_3_freq,T-3);
		mutual_inf2_6 = cmi5(&past_6_freq,T-6);
		aut_0 = cmi0(&auto_0_freq,T-1);
		aut_2 = cmi2(&auto_2_freq,T-3);
		aut_5 = cmi5(&auto_5_freq,T-6);
		hat_aut_0 = hat_cmi0(&auto_0_freq,T-1);
		hat_aut_star = hat_star(&auto_0_freq,T-1);
	}

	//todo write a print function. should be just calling all measures print functions
	//DATA


	//timesteps, todo: better handling of this parameter, it is now basically global to
	//the main() and local to a repetition. this sucks. make it one or or the other
	//note that for a local version the postprocessing of data becomes more challanging
	//basically timesteps has to be obtained again from each repetition.
	unsigned int T;

	Agent<a_state_type,e_output_type,a_output_type> agent;
	Agent<e_state_type,a_output_type,e_output_type> environment;

	//these arrays will be kept by the measures themselves
	f_matrix_type auto_freq;
	f_matrix_type auto_star_freq;
	f_array_type auto_freq2;
//	f_big_array_type auto_1_freq;
	boost::multi_array<unsigned int, 3> auto_0_freq;
	boost::multi_array<unsigned int, 5> auto_2_freq;
	boost::multi_array<unsigned int, 8> auto_5_freq;

	//for our measure  with multiple past states:
	boost::multi_array<unsigned int, 5> past_3_freq;
	boost::multi_array<unsigned int, 8> past_6_freq;

	//todo this should be a vector of pointers to measures
	double H_Eo_Ao;
	double mutual_inf;
	double H_E;
	double H_Eo;
	double H_A;
	double H_Ao;
	double aut_star;
	double mutual_inf2;
	double mutual_inf2_3;
	double mutual_inf2_6;
	double aut_0;
	double aut_2;
	double aut_5;
	double hat_aut_0;
	double hat_aut_2;
	double hat_aut_5;
	double hat_aut_star;

	double p;
};


#define REPETITION_H_


#endif /* REPETITION_H_ */
