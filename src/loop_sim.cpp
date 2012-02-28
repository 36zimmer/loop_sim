//============================================================================
// Name        : loop_sim.cpp
// Author      : Martin Biehl
// Version     :
// Copyright   : Use as you like, but cite me please.
// Description : Simulates a simplistic agent environment system.
//============================================================================

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <cassert>
#include <algorithm>
#include "boost/multi_array.hpp"
//#include "libs/multi_array/example/for_each.hpp"
//#include "for_each_sum.hpp"
//#include "for_each.hpp"

#include "repetition.h"
#include "used_functions.h"
//#include "initializations.h"
//#include "dynamics.h"
//#include "outputs.h"
#include "configurations.h"

using namespace std;


//todo: write all functions as templates that can deal with multiarrays ;)
typedef boost::multi_array<unsigned int, 3> f_triple_type; //the frequency array type
typedef boost::multi_array<unsigned int, 2> f_matrix_type; //the frequency array type
typedef f_matrix_type::index a_index;
typedef boost::multi_array<unsigned int, 1> f_vector_type; //the frequency array type
typedef f_matrix_type::index v_index;
typedef boost::multi_array<double, 1> vector_type; //the frequency array type

// array_type A(boost::extents[3][4][2]);

//typedef int a_S_type;  //define agent state type here.
//typedef int a_O_type;



//maybe use a template typedef construction for this...
template < class a_state_type, class a_output_type , class e_state_type, class e_output_type>
class Supervisor {
public:
	Supervisor(Agent<a_state_type,e_output_type,a_output_type> *a, Agent<e_state_type,a_output_type,e_output_type> *e,int T) {
		agent =a;
		env = e;
	}

//	void frequencyupdate(state_type *s, output_type *o) {
//		state_frequencies.at(*s) = state_frequencies.at(*s)+1;
//		output_frequencies.at(*o) = output_frequencies.at(*o)+1;
//	}

	Agent<a_state_type,e_output_type,a_output_type>* agent;
	Agent<e_state_type,a_output_type,e_output_type>* env;
};




int main() {
	 //set and get parameters.
	bool file_output = 0;
	bool latex_out = 1;
	bool per_t_step = 0;
	unsigned int timesteps = 100000;
	unsigned int number_of_repetitions = 1;

	//remove the following line
//	std::ofstream datafile("test.txt");



//	string input = "";
//	while (true) {
//		cout << "Enter number of timesteps: ";
//		getline(cin, input);
//		// This code converts from string to number safely.
//		stringstream myStream(input);
//		if (myStream >> timesteps)
//		 break;
//		cout << "Invalid number, please try again" << endl;
//	}
//	while (true) {
//		cout << "Enter number of repetitions: ";
//		getline(cin, input);
//		// This code converts from string to number safely.
//		stringstream myStream(input);
//		if (myStream >> number_of_repetitions)
//		 break;
//		cout << "Invalid number, please try again" << endl;
//	}

	//initialize the random number generator
	srand ( time(NULL) );
	//create pointers to each repetition
	Repetition<int,int,int,int> *R[number_of_repetitions];
	//get the configurations including all dynamics for agent and environment

	//agent
//	agent_conf<int,int,int> aconf = getFixedAgentConf<int,int,int>(2,2);
//	agent_conf<int,int,int> aconf = getRandomAgentConf<int,int,int>(5,5);
//	agent_conf<int,int,int> aconf = getCopyAgentConf<int,int,int>(5,5);
//	agent_conf<int,int,int> aconf = getAgent16Conf<int,int,int>();
//	agent_conf<int,int,int> aconf = getNBAgent94bConf<int,int,int>();
//	agent_conf<int,int,int> aconf = getParamRandomAgentConf<int,int,int>(5,5);
//	agent_conf<int,int,int> aconf = getNBtwostateactive<int,int,int>();
//	agent_conf<int,int,int> aconf = getNBtwostatepassive<int,int,int>();
//	agent_conf<int,int,int> aconf = getNBfourstateactive<int,int,int>();
//	agent_conf<int,int,int> aconf = getNBfourstatepassive<int,int,int>();
	agent_conf<int,int,int> aconf = getMemTest1AgentConf<int,int,int>();

	//environment
//	agent_conf<int,int,int> econf = getFixedAgentConf<int,int,int>(10,3);
//	agent_conf<int,int,int> econf = getRandomAgentConf<int,int,int>(5,5);
//	agent_conf<int,int,int> econf = getParamRandomAgentConf<int,int,int>(5,5);
//	agent_conf<int,int,int> econf = getEnvironment16Conf<int,int,int>();
//	agent_conf<int,int,int> econf = getNB2StateEnvironmentConf<int,int,int>();
	agent_conf<int,int,int> econf = getCopyAgentConf<int,int,int>(5,5);
//	agent_conf<int,int,int> econf = getParamInfluenceConf<int,int,int>(5,5);
//	agent_conf<int,int,int> econf = getNBcircleEnvironment<int,int,int>();
//	agent_conf<int,int,int> econf = getSineAgentConf<int,int,int>(5,5);
//	agent_conf<int,int,int> econf = getMemTest1AgentConf<int,int,int>();

	//super loop (over experiments)
	for (unsigned int r = 0; r < number_of_repetitions; r++ ) {
		//create repetition, including agent and environment
		R[r] = new Repetition<int,int,int,int>(0.9,timesteps,aconf,econf);
		//initialize agents and their first outputs
		R[r]->initsys();

		//start main loop
		for (unsigned int t=1; t<timesteps; t++) {
			//calculate and save new states so that the end of the vector is at t
			R[r]->step(t);
			//update the state_frequencies
			R[r]->frequencyupdate(t);

			//remove from here
//			if (datafile.is_open()) {
//				datafile << R[r]->environment.outputs.at(t) << " " << R[r]->agent.outputs.at(t) << "\n";
//			}
//			else cout << "Unable to open file";
			//to here
		//end of main loop
		}
		//remove from here
//		datafile.close();
		//to here

		//get the entropies over all timesteps of the repetition:
		R[r]->getentropies();
//		R[r]->H_Eo_Ao = entropy(&R[r]->auto_freq,timesteps-1);
//		R[r]->mutual_inf = mi(R[r]->auto_freq,timesteps-1);
//		R[r]->H_A = entropy(&R[r]->agent.state_frequencies, timesteps);
//		R[r]->H_Ao = entropy(&R[r]->agent.output_frequencies, timesteps);
//		R[r]->H_E = entropy(&R[r]->environment.state_frequencies, timesteps);
//		R[r]->H_Eo = entropy(&R[r]->environment.output_frequencies, timesteps);

	//end of super loop
	}

	vector_type H_A_r(boost::extents[timesteps]);
	vector_type H_Ao_r(boost::extents[timesteps]);
	vector_type H_E_r(boost::extents[timesteps]);
	vector_type H_Eo_r(boost::extents[timesteps]);
	vector_type H_Eo_Ao(boost::extents[timesteps]);
	vector_type auto_r(boost::extents[timesteps]);
	if(per_t_step) {
		for (unsigned int t=0; t<timesteps; t++){
			//get the timestep specific frequencies
			//todo this is not prepared for variation of agents among repetitions!
			f_vector_type a_state_f_accross_r(boost::extents[aconf.number_of_states]);
			f_vector_type a_output_f_accross_r(boost::extents[aconf.number_of_outputs]);
			f_vector_type e_state_f_accross_r(boost::extents[econf.number_of_states]);
			f_vector_type e_output_f_accross_r(boost::extents[econf.number_of_outputs]);
			f_matrix_type auto_f_accross_r(boost::extents[econf.number_of_outputs][aconf.number_of_outputs]);
			for (unsigned int r=0; r<number_of_repetitions;r++){
				a_state_f_accross_r[R[r]->agent.states.at(t)]+=1;
				a_output_f_accross_r[R[r]->agent.outputs.at(t)]+=1;
				e_state_f_accross_r[R[r]->environment.states.at(t)]+=1;
				e_output_f_accross_r[R[r]->environment.outputs.at(t)]+=1;
				if(t>0)
					auto_f_accross_r[R[r]->environment.outputs.at(t)][R[r]->agent.outputs.at(t-1)]+=1;

			}
			//and the timestep specific entropies
			H_A_r[t] = entropy(&a_state_f_accross_r,number_of_repetitions);
			H_Ao_r[t] = entropy(&a_output_f_accross_r,number_of_repetitions);
			H_E_r[t] = entropy(&e_state_f_accross_r,number_of_repetitions);
			H_Eo_r[t] = entropy(&e_output_f_accross_r,number_of_repetitions);
			H_Eo_Ao[t] = (t>0) ? entropy(&auto_f_accross_r,number_of_repetitions) : 0;
			auto_r[t] = (t>0) ? mi(auto_f_accross_r,number_of_repetitions) : 0;

		}
		//print the entropies per timestep

		out(H_A_r);

		out(H_Ao_r);

		out(H_E_r);

		out(H_Eo_r);

		out(H_Eo_Ao);

		out(auto_r);

	}

	//print the entropies per repetition (over all timesteps)
	std::cout << "#r" << " " << "H_A" << " " << "H_Ao" << " " << "H_E" << " "
			<< "H_Eo" << " " << "H_Eo_Ao" << " " << "mutual_inf" << " " << "mutual_inf2" << " "
			<< "mutual_inf2_3" << " "<< "mutual_inf2_6" << " "
			<< "aut_star" << " " << "aut_0" << " " << "aut_2" << " " << "aut_5" << " "
			<< "hat_aut_0" << " " << "hat_aut_star" <<"\n";
	for (unsigned int r=0; r<number_of_repetitions; r++) {
		std::cout << r << " " << R[r]->H_A << " " << R[r]->H_Ao << " " << R[r]->H_E << " "
				<< R[r]->H_Eo << " " << R[r]->H_Eo_Ao << " " << R[r]->mutual_inf << " "
				<< R[r]->mutual_inf2 << " "<< R[r]->mutual_inf2_3 << " "<< R[r]->mutual_inf2_6 << " "
				<< R[r]->mutual_inf2 << " " << R[r]->aut_star << " " << R[r]->aut_0 << " "
				<< R[r]->aut_2 <<  " " << R[r]->aut_5 <<  " " << R[r]->hat_aut_0 <<  " "
				<< R[r]->hat_aut_star << "\\\\" <<"\n";
	}

	//file output

	if(file_output)  {

		time_t rawtime;
		struct tm * timeinfo;
		char buffer [80];

		time ( &rawtime );
		timeinfo = localtime ( &rawtime );

		strftime (buffer,80,"_%m-%d-%y_%H-%M-%S.log",timeinfo);
		string buffer1 =aconf.name+"_on_"+econf.name+buffer;
		cout << buffer1;
		std::ofstream datafile(buffer1.c_str());
		if (datafile.is_open()) {
			datafile << "# Total timesteps: " << timesteps << "\n";
			datafile << "# Total repetitions: " << number_of_repetitions << "\n";
			datafile << "###\n";
			datafile << "# Agent configuration:\n";
			datafile << "# Name: " << aconf.name << "\n";
			datafile << "# number_of_states: " << aconf.number_of_states << "\n";
			datafile << "# number_of_outputs: " << aconf.number_of_outputs << "\n";
	//		datafile << "# init: " << aconf.number_of_states << "\n";
			datafile << "###\n";
			datafile << "# Environment configuration:\n";
			datafile << "# Name: " << econf.name << "\n";
			datafile << "# number_of_states: " << econf.number_of_states << "\n";
			datafile << "# number_of_states: " << econf.number_of_outputs << "\n";
			datafile << "###\n";
			if(per_t_step) {
				datafile << "# Averages over repetitions, per timestep:\n";
				datafile << "# H(A)at timestep t=0,1,2,3,... \n";
				datafile << "# H(Ao)at timestep t=0,1,2,3,... \n";
				datafile << "# H(E)at timestep t=0,1,2,3,... \n";
				datafile << "# H(Eo)at timestep t=0,1,2,3,... \n";
				datafile << "# H(Eo,Ao) at timestep t= 1,2,3,... at t=0 a zero is manually inserted\n";
				datafile << "# MI(Eo,Ao) at timestep t= 1,2,3,... at t=0 a zero is manually inserted\n";
				datafile << "###\n";
				//print the entropies per timestep
				datafile << "$H(A)$ & ";
				fout(H_A_r,&datafile);
				datafile << "$H(A^{\\circ})$ & ";
				fout(H_Ao_r,&datafile);
				datafile << "$H(E)$ & ";
				fout(H_E_r,&datafile);
				datafile << "$H(E^{\\circ})$ & ";
				fout(H_Eo_r,&datafile);
				datafile << "$H(E^{\\circ}_n,A^{\\circ}_{n-1})$ & ";
				fout(H_Eo_Ao,&datafile);
				datafile << "$MI(E^{\\circ}_n,A^{\\circ}_{n-1})$ & ";
				fout(auto_r,&datafile);
				datafile << "###\n";
			}
			//print the entropies per repetition (over all timesteps)
			datafile << "#Averages over all timesteps, per repetition:\n";
			if(latex_out) {
				datafile << "#r" << " & " << "$H(A)$" << " & " << "$H(A^{\\circ})$" << " & "
						<< "$H(E)$" << " & " << "$H(E^{\\circ})$" << " & "
						<< "$H(E^{\\circ}_n,A^{\\circ}_{n-1})$" << " & "
						<< "$MI(E^{\\circ}_n,A^{\\circ}_{n-1})$" << " & "
						<< "$MI(E^{\\circ}_n,A^{\\circ}_{n-1}|E^{\\circ}_{n-1})$" << " & "
						<< "$Aut^*$" << " & " << "$Aut_0$" << " & " << "$Aut_2$" << " & "
						<< "$Aut_5$" << " & " << "$\\hat{A}ut_0$" << " & "
						<< "$\\hat{A}ut^*$" << "\\\\" << "\n";
				for (unsigned int r=0; r<number_of_repetitions; r++) {
					datafile << r << " & " << R[r]->H_A << " & " << R[r]->H_Ao << " & "
							<< R[r]->H_E << " & " << R[r]->H_Eo << " & " << R[r]->H_Eo_Ao << " & "
							<< R[r]->mutual_inf << " & " << R[r]->mutual_inf2 << " & "
							<< R[r]->aut_star << " & " << R[r]->aut_0 << " & "
							<< R[r]->aut_2 <<  " & " << R[r]->aut_5 <<  " & "
							<< R[r]->hat_aut_0 <<  " & "
							<< R[r]->hat_aut_star << "\\\\" << "\n";
				}
			}else {
				datafile << "#r" << " " << "H_A" << " " << "H_Ao" << " " << "H_E" << " "
						<< "H_Eo" << " " << "H_Eo_Ao" << " " << "mutual_inf" << " "
						<< "mutual_inf2" << " " << "aut_star" << " " << "aut_0" << " "
						<< "aut_2" << " " << "aut_5" << " " << "hat_aut_0"  << " "
						<< "hat_aut_star"<<"\n";
				for (unsigned int r=0; r<number_of_repetitions; r++) {
					datafile << r << " " << R[r]->H_A << " " << R[r]->H_Ao << " "
							<< R[r]->H_E << " "	<< R[r]->H_Eo << " " << R[r]->H_Eo_Ao << " "
							<< R[r]->mutual_inf << " " << R[r]->mutual_inf2 << " "
							<< R[r]->aut_star << " " << R[r]->aut_0 << " " << R[r]->aut_2 <<  " "
							<< R[r]->aut_5 <<  " " << R[r]->hat_aut_0 <<  " "
							<< R[r]->hat_aut_star <<"\n";
				}

			}
			datafile.close();
		}
		else cout << "Unable to open file";
		datafile.close();
	}

//	std::copy(H_A_r.begin(),H_A_r.end(),
//		            std::ostream_iterator<double>(std::cout," "));
//	std::copy(H_A_r.begin(),H_A_r.end(),
//		            std::ostream_iterator<double>(std::cout," "));


	//todo delete the pointer array
	return 0;
}
