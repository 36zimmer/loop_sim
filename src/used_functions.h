/*
 * used_functions.h
 *
 *  Created on: Jul 24, 2011
 *      Author: x
 */

#ifndef USED_FUNCTIONS_H_
#include <math.h>
#include <cassert>
#include <algorithm>

typedef boost::multi_array<unsigned int, 3> f_array_type; //the frequency array type
typedef boost::multi_array<unsigned int, 2> f_matrix_type; //the frequency matrix type
typedef boost::multi_array<unsigned int, 1> f_vector_type; //the frequency vector type


double distribute_p(double x, double p) {
	//this function maps x from [-1,1] to [-1,1],
	//for p = 0 it returns 0.0 (the middle) for all values of x
	//for p = 1 it returns x (so it is the identity)
	//in between it concentrates x values around 0.0 more and more the smaller p
//	assert((x <= 1.0) && (0.0 <= x) && (p <= 1.0) && (0.0 <= p));
	assert((x <= 1.0) && (-1.0 <= x) && (p <= 1.0) && (0.0 <= p));
//	return (p==0) ? .5 : (copysign(pow(fabs(2*(x-0.5)),1/p),2*(x-0.5)) + 1)/2;
	return (p==0) ? 0.0 : (copysign(pow(fabs(x),1/p),x));
}
struct Ent_of_Freq {
	Ent_of_Freq(unsigned int T_steps) {
		T=T_steps;
	}
	double operator() (double accu, const unsigned int val) {
	  return accu - ((val>0) ? (double)val/(double)T * log2((double)val/(double)T) : 0);
	}

	unsigned int T;
};

template < class array_type >
void out(array_type A){
	std::copy(A.origin(),A.origin()+A.num_elements(),
		            std::ostream_iterator<double>(std::cout,","));
	std::cout << "\n";
}
template < class array_type >
void out(array_type* A){
	std::copy(A->origin(), A->origin()+A->num_elements(),
		            std::ostream_iterator<double>(std::cout,","));
	std::cout << "\n";
}
template < class array_type >
void fout(array_type A, std::ofstream *file){
	std::copy(A.origin(),A.origin()+A.num_elements(),
		            std::ostream_iterator<double>(*file," & "));
	*file << "\n";
}
template < class array_type >
void fout(array_type* A, std::ofstream *file){
	std::copy(A->origin(), A->origin()+A->num_elements(),
		            std::ostream_iterator<double>(*file,","));
	*file << "\n";
}
template < class array_type >
double entropy(const array_type *A, unsigned int N ) {
	//initialize the entropy from absolute frequencies calculator with the total number of events
	Ent_of_Freq summands(N);
	//get an initial value
	unsigned int counter = 0.0;
//	std::cout << "\nnext is the contents of A \n";
//	std::copy(A->origin(), A->origin()+A->num_elements(),
//		            std::ostream_iterator<double>(std::cout,","));
//	std::cout << "\nbefore was the contents of A \n";
	//check whether there was an event recorded at each timestep (or event)
	assert(accumulate (A->data(), A->data()+A->num_elements(), counter)==N);
	//get an initial value
	double init = 0.0;
	//this awesome stl algorithm goes through all elements of any vector or array
	//and performs the function summands() on init and the present element
	return accumulate (A->data(), A->data()+A->num_elements(), init, summands );
}
//todo, make this act on a pointer to the matrix
double mi(const f_matrix_type A, unsigned int N ) {
	//initialize the entropy from absolute frequencies calculator with the total number of events
//	Ent_of_Freq summands(T);
//    std::cout << "\n " << A.size() << std::endl;
	//create a vector to store the abs frequencies of rows
	f_vector_type x_f(boost::extents[A.shape()[0]]);
	f_vector_type y_f(boost::extents[A.shape()[1]]);
//	x_f(boost::extents[A.size()]);
    int I =A.shape()[0];
    int J =A.shape()[1];
    for(int i = 0; i != I; ++i)
      for(int j = 0; j != J; ++j) {
    	  x_f[i]+=A[i][j];
    	  y_f[j]+=A[i][j];
      }

	//check whether there was an event recorded at each timestep (or event)
    unsigned int counter = 0.0;
	assert(std::accumulate (x_f.data(), x_f.data()+x_f.num_elements(), counter)==N);
	counter = 0.0;
	assert(std::accumulate (y_f.data(), y_f.data()+y_f.num_elements(), counter)==N);

    double mi = 0.0;
    for(int i = 0; i != I; ++i)
      for(int j = 0; j != J; ++j)
    	  mi+= ((A[i][j]>0) ? (double)A[i][j]/(double)N
    			  * log2(N*(double)A[i][j]/(double)x_f[i]/(double)y_f[j]) : 0);

//	std::copy(x_f.begin(),x_f.end(),
//	            std::ostream_iterator<double>(std::cout," "));
//	std::copy(y_f.begin(),y_f.end(),
//	            std::ostream_iterator<double>(std::cout," "));
    return mi;
}

double cmi0(const f_array_type *A, unsigned int N ) {

	f_vector_type z_f(boost::extents[A->shape()[2]]);
	f_matrix_type x_z_f(boost::extents[A->shape()[0]][A->shape()[2]]);
	f_matrix_type y_z_f(boost::extents[A->shape()[1]][A->shape()[2]]);

    unsigned int I =A->shape()[0];
    unsigned int J =A->shape()[1];
    unsigned int K =A->shape()[2];

	for (unsigned int k = 0; k != K; ++k) {
		for (unsigned int j = 0; j != J; ++j) {
			for (unsigned int i = 0; i != I; ++i) {
				y_z_f[j][k] += (*A)[i][j][k];
				x_z_f[i][k] += (*A)[i][j][k];
			}
			z_f[k] += y_z_f[j][k];
		}
	}

    double cmi0 = 0.0;
    for(unsigned int i = 0; i != I; ++i)
      for(unsigned int j = 0; j != J; ++j)
    	for (unsigned int k = 0; k != K; ++k)
			  cmi0 += (((*A)[i][j][k]>0) ? (double)(*A)[i][j][k]/(double)N
					  * log2((double)z_f[k]*(double)(*A)[i][j][k]/(double)x_z_f[i][k]/(double)y_z_f[j][k]) : 0);
    return cmi0;
}
double cmi1(const boost::multi_array<unsigned int, 4> *A, unsigned int N ) {


	boost::multi_array<unsigned int, 2> z_f(boost::extents[A->shape()[2]][A->shape()[3]]);
//	f_vector_type z_f(boost::extents[A->shape()[2]]);
	boost::multi_array<unsigned int, 3> x_z_f(boost::extents[A->shape()[0]][A->shape()[2]][A->shape()[3]]);
	boost::multi_array<unsigned int, 3> y_z_f(boost::extents[A->shape()[1]][A->shape()[2]][A->shape()[3]]);

    unsigned int I =A->shape()[0];
    unsigned int J =A->shape()[1];
    unsigned int K =A->shape()[2];
    unsigned int M =A->shape()[3];

    for (unsigned int m = 0; m != M; ++m) {
		for (unsigned int k = 0; k != K; ++k) {
			for (unsigned int j = 0; j != J; ++j) {
				for (unsigned int i = 0; i != I; ++i) {
					y_z_f[j][k][m] += (*A)[i][j][k][m];
					x_z_f[i][k][m] += (*A)[i][j][k][m];
				}
				z_f[k][m] += y_z_f[j][k][m];
			}
		}
    }
    double cmi1 = 0.0;
    for(unsigned int i = 0; i != I; ++i)
      for(unsigned int j = 0; j != J; ++j)
    	for(unsigned int k = 0; k != K; ++k)
    		for(unsigned int m = 0; m != M; ++m)
			  cmi1 += (((*A)[i][j][k][m]>0) ? (double)(*A)[i][j][k][m]/(double)N
					  * log2((double)z_f[k][m]*(double)(*A)[i][j][k][m]/(double)x_z_f[i][k][m]/(double)y_z_f[j][k][m]) : 0);
    return cmi1;
}

double cmi2(const boost::multi_array<unsigned int, 5> *A, unsigned int N ) {

	boost::multi_array<unsigned int, 3> z_f(boost::extents[A->shape()[2]][A->shape()[3]][A->shape()[4]]);
//	f_vector_type z_f(boost::extents[A->shape()[2]]);
	boost::multi_array<unsigned int, 4> x_z_f(boost::extents[A->shape()[0]][A->shape()[2]][A->shape()[3]][A->shape()[4]]);
	boost::multi_array<unsigned int, 4> y_z_f(boost::extents[A->shape()[1]][A->shape()[2]][A->shape()[3]][A->shape()[4]]);

    unsigned int I =A->shape()[0];
    unsigned int J =A->shape()[1];
    unsigned int K =A->shape()[2];
    unsigned int M =A->shape()[3];
    unsigned int P =A->shape()[4];

    for (unsigned int p = 0; p != P; ++p) {
		for (unsigned int m = 0; m != M; ++m) {
			for (unsigned int k = 0; k != K; ++k) {
				for (unsigned int j = 0; j != J; ++j) {
					for (unsigned int i = 0; i != I; ++i) {
						y_z_f[j][k][m][p] += (*A)[i][j][k][m][p];
						x_z_f[i][k][m][p] += (*A)[i][j][k][m][p];
					}
					z_f[k][m][p] += y_z_f[j][k][m][p];
				}
			}
		}
    }
    double cmi2 = 0.0;
    for(unsigned int i = 0; i != I; ++i)
      for(unsigned int j = 0; j != J; ++j)
    	for(unsigned int k = 0; k != K; ++k)
    		for(unsigned int m = 0; m != M; ++m)
    			for(unsigned int p = 0; p != P; ++p)
				  cmi2 += (((*A)[i][j][k][m][p]>0) ?
						  (double)(*A)[i][j][k][m][p]/(double)N
						  * log2((double)z_f[k][m][p]*(double)(*A)[i][j][k][m][p]/
								  (double)x_z_f[i][k][m][p]/(double)y_z_f[j][k][m][p])
						  : 0);
	return cmi2;
}
double cmi5(const boost::multi_array<unsigned int, 8> *A, unsigned int N ) {

	//z_f is the array of variables conditioned on, it contains all possible combinations of these variables and
	//stores their respective frequencies
	boost::multi_array<unsigned int, 6> z_f(boost::extents[A->shape()[2]][A->shape()[3]][A->shape()[4]][A->shape()[5]][A->shape()[6]][A->shape()[7]]);
//	f_vector_type z_f(boost::extents[A->shape()[2]]);
	boost::multi_array<unsigned int, 7> x_z_f(boost::extents[A->shape()[0]][A->shape()[2]][A->shape()[3]][A->shape()[4]][A->shape()[5]][A->shape()[6]][A->shape()[7]]);
	boost::multi_array<unsigned int, 7> y_z_f(boost::extents[A->shape()[1]][A->shape()[2]][A->shape()[3]][A->shape()[4]][A->shape()[5]][A->shape()[6]][A->shape()[7]]);

    unsigned int I =A->shape()[0];
    unsigned int J =A->shape()[1];
    unsigned int K =A->shape()[2];
    unsigned int M =A->shape()[3];
    unsigned int P =A->shape()[4];
    unsigned int Q =A->shape()[5];
    unsigned int R =A->shape()[6];
    unsigned int S =A->shape()[7];

    for (unsigned int s = 0; s != S; ++s) {
    for (unsigned int r = 0; r != R; ++r) {
    for (unsigned int q = 0; q != Q; ++q) {
    for (unsigned int p = 0; p != P; ++p) {
		for (unsigned int m = 0; m != M; ++m) {
			for (unsigned int k = 0; k != K; ++k) {
				for (unsigned int j = 0; j != J; ++j) {
					for (unsigned int i = 0; i != I; ++i) {
						y_z_f[j][k][m][p][q][r][s] += (*A)[i][j][k][m][p][q][r][s];
						x_z_f[i][k][m][p][q][r][s] += (*A)[i][j][k][m][p][q][r][s];
					}
					z_f[k][m][p][q][r][s] += y_z_f[j][k][m][p][q][r][s];
				}
			}
		}
    }
    }
    }
    }
    double cmi5 = 0.0;
    for(unsigned int i = 0; i != I; ++i)
      for(unsigned int j = 0; j != J; ++j)
    	for(unsigned int k = 0; k != K; ++k)
    	for(unsigned int m = 0; m != M; ++m)
		for(unsigned int p = 0; p != P; ++p)
		for(unsigned int q = 0; q != Q; ++q)
		for(unsigned int r = 0; r != R; ++r)
		for(unsigned int s = 0; s != S; ++s)
				  cmi5 += (((*A)[i][j][k][m][p][q][r][s]>0) ?
						  (double)(*A)[i][j][k][m][p][q][r][s]/(double)N
						  * log2((double)z_f[k][m][p][q][r][s]*(double)(*A)[i][j][k][m][p][q][r][s]/
								  (double)x_z_f[i][k][m][p][q][r][s]/(double)y_z_f[j][k][m][p][q][r][s])
						  : 0);
	return cmi5;
}
//to calculate Bertschingers \hat{A}_0 autonomy measure:
double hat_cmi0(const f_array_type *A, unsigned int N ) {

	f_vector_type z_f(boost::extents[A->shape()[2]]);
	f_vector_type y_f(boost::extents[A->shape()[1]]);
	f_matrix_type y_z_f(boost::extents[A->shape()[1]][A->shape()[2]]);

	boost::multi_array<double, 2> x_c_z_p(boost::extents[A->shape()[0]][A->shape()[2]]);

    unsigned int I =A->shape()[0];
    unsigned int J =A->shape()[1];
    unsigned int K =A->shape()[2];

	for (unsigned int k = 0; k != K; ++k) {
		for (unsigned int j = 0; j != J; ++j) {
			for (unsigned int i = 0; i != I; ++i) {
				y_z_f[j][k] += (*A)[i][j][k];
//				x_z_f[i][k] += (*A)[i][j][k];
			}
			z_f[k] += y_z_f[j][k];
			y_f[j] += y_z_f[j][k];
		}
	}

	for (unsigned int k = 0; k != K; ++k)
		for (unsigned int i = 0; i != I; ++i)
		for (unsigned int j = 0; j != J; ++j)
			//x_c_z_p[i][k] += (double)y_f[j]/(double)N * (double)y_z_f[j][k]/(double)N;
			x_c_z_p[i][k] += (((*A)[i][j][k]>0) ? (double)y_f[j]/(double)N * (double)(*A)[i][j][k]/(double)y_z_f[j][k] : 0);

//	double test1 =0.0;
//	double test2 =0.0;
//	for (unsigned int k = 0; k != K; ++k)
//		test1 += (double)z_f[k]/(double)N;
//	for (unsigned int j = 0; j != J; ++j)
//		test2 += (double)y_f[j]/(double)N;
//
//	std::cout << "test hat aut0 1: " << test1 << "\n";
//	std::cout << "test hat aut0 2: " << test2 << "\n";


    double hat_cmi0 = 0.0;
    for(unsigned int i = 0; i != I; ++i)
      for(unsigned int j = 0; j != J; ++j)
    	for (unsigned int k = 0; k != K; ++k)
    		hat_cmi0 += (((*A)[i][j][k]>0) ? (double)(*A)[i][j][k]/(double)y_z_f[j][k]
    		        * (double)y_f[j]/(double)N * z_f[k]/(double)N
					  * log2((double)(*A)[i][j][k]/(double)y_z_f[j][k] / x_c_z_p[i][k]) : 0);
    return hat_cmi0;
}
//to calculate Bertschingers \hat{A}^* autonomy measure:
double hat_star(const f_array_type *A, unsigned int N ) {
	//arrays to hold frequencies
	f_vector_type z_f(boost::extents[A->shape()[2]]);
	f_vector_type y_f(boost::extents[A->shape()[1]]);
	f_matrix_type y_z_f(boost::extents[A->shape()[1]][A->shape()[2]]);

	//arrays to hold probabilities
	boost::multi_array<double, 2> top_x_y_p(boost::extents[A->shape()[0]][A->shape()[2]]);
	boost::multi_array<double, 1> bottom_x_p(boost::extents[A->shape()[0]]);

    unsigned int I =A->shape()[0];
    unsigned int J =A->shape()[1];
    unsigned int K =A->shape()[2];

	for (unsigned int k = 0; k != K; ++k) {
		for (unsigned int j = 0; j != J; ++j) {
			for (unsigned int i = 0; i != I; ++i) {
				y_z_f[j][k] += (*A)[i][j][k];
//				x_z_f[i][k] += (*A)[i][j][k];
			}
			z_f[k] += y_z_f[j][k];
			y_f[j] += y_z_f[j][k];
		}
	}

	for (unsigned int k = 0; k != K; ++k)
		for (unsigned int i = 0; i != I; ++i)
		for (unsigned int j = 0; j != J; ++j) {
			top_x_y_p[i][j] += (((*A)[i][j][k]>0) ? (double)(*A)[i][j][k]/(double)y_z_f[j][k] * (double)z_f[k]/(double)N : 0);
			bottom_x_p[i] += (((*A)[i][j][k]>0) ? (double)(*A)[i][j][k]/(double)y_z_f[j][k] * (double)y_f[j]/(double)N * (double)z_f[k]/(double)N : 0);
		}


//	double test1 =0.0;
//	double test2 =0.0;
//	for (unsigned int i = 0; i != I; ++i){
//		for (unsigned int j = 0; j != J; ++j){
//				test1 += top_x_y_p[i][j];
//		}
//		test2 += bottom_x_p[i];
//	}
//
//	std::cout << "test star1: " << test1 << "\n";
//	std::cout << "test star2: " << test2 << "\n";

    double hat_star = 0.0;
    for(unsigned int i = 0; i != I; ++i)
      for(unsigned int j = 0; j != J; ++j)
    	for (unsigned int k = 0; k != K; ++k)
    		hat_star += (((*A)[i][j][k]>0) ? (double)(*A)[i][j][k]/(double)y_z_f[j][k]
    		        * (double)y_f[j]/(double)N * z_f[k]/(double)N
					  * log2(top_x_y_p[i][j] / bottom_x_p[i]) : 0);
    return hat_star;
}
//template <class array_type>
//double cmi<array_type>(const array_type *A, unsigned int N ) {
//	//initialize the entropy from absolute frequencies calculator with the total number of events
////	Ent_of_Freq summands(T);
////    std::cout << "\n " << A.size() << std::endl;
//	//create a vector to store the abs frequencies of rows
////	f_vector_type x_f(boost::extents[A->shape()[0]]);
////	f_vector_type y_f(boost::extents[A->shape()[1]]);
//	unsigned int D = A->num_dimensions();
//	array_type::subarray<D-2>::type z_f = A_
//	f_vector_type z_f(boost::extents[A->shape()[2]]);
//	f_matrix_type x_z_f(boost::extents[A->shape()[0]][A->shape()[2]]);
//	f_matrix_type y_z_f(boost::extents[A->shape()[1]][A->shape()[2]]);
////	x_f(boost::extents[A.size()]);
////    int I =A.shape()[0];
////    int J =A.shape()[1];
////    for(int i = 0; i != I; ++i)
////      for(int j = 0; j != J; ++j)
////    	  x_f[i]+=A[i][j];
////
////    for(int j = 0; j != J; ++j)
////      for(int i = 0; i != I; ++i)
////    	 y_f[j]+=A[i][j];
//	f_array_type::subarray<
//	std::transform
//
//	//check whether there was an event recorded at each timestep (or event)
//    unsigned int counter = 0.0;
//	assert(std::accumulate (x_f.data(), x_f.data()+x_f.num_elements(), counter)==N);
//	counter = 0.0;
//	assert(std::accumulate (y_f.data(), y_f.data()+y_f.num_elements(), counter)==N);
//
//    double mi = 0.0;
//    for(int i = 0; i != I; ++i)
//      for(int j = 0; j != J; ++j)
//    	  mi+= ((A[i][j]>0) ? (double)A[i][j]/(double)N
//    			  * log2(N*(double)A[i][j]/(double)x_f[i]/(double)y_f[j]) : 0);
//
////	std::copy(x_f.begin(),x_f.end(),
////	            std::ostream_iterator<double>(std::cout," "));
////	std::copy(y_f.begin(),y_f.end(),
////	            std::ostream_iterator<double>(std::cout," "));
//    return mi;
//}



#define USED_FUNCTIONS_H_


#endif /* USED_FUNCTIONS_H_ */
