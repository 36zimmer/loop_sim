/*
 * measure.h
 *
 *  Created on: Aug 5, 2011
 *      Author: martin biehl
 */

#ifndef MEASURE_H_
#define MEASURE_H_

template <class state_type, class output_type, unsigned int NumDims>
class Measure<state_type, NumDims> {
public:
	Measure(std::vector<state_type> &a_states, std::vector<output_type> &a_outputs, std::vector<state_type> &e_states, std::vector<output_type> &e_outputs) {

	}
	Measure(measure_conf<state_type,output_type,NumDims> m_conf){
		//todo use a conf setup just like for the agent maybe.
	}
	//todo either pass this function to the constructor or make all measures children of this class and overwrite this function
	virtual void calculate() = 0;
	//todo either pass this function to the constructor or make all measures children of this class and overwrite this function
	virtual void frequencyupdate() = 0;

	double value;
	boost::multi_array<state_type,NumDims> frequencies;
	string name;
};

#endif /* MEASURE_H_ */
