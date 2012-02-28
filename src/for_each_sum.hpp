// Copyright 2002 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software 
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Boost.MultiArray Library
//  Authors: Ronald Garcia
//           Jeremy Siek
//           Andrew Lumsdaine
//  The ugly sum hack is by martin biehl, sorry.
//  See http://www.boost.org/libs/multi_array for documentation.

#ifndef FOR_EACH_SUM_HPP
#define FOR_EACH_SUM_HPP

//
// for_each_sum.hpp - Writing an algorithm to sum each element of
//   a multi_array
//

#include "boost/type.hpp"

template <typename Array, typename Element, typename Functor>
void for_each_sum (const boost::type<Element>& type_dispatch,
               Array A, Functor& xform, double* sum) {
  for_each_sum(type_dispatch,A.begin(),A.end(),xform,sum);
}

template <typename Element, typename Functor>
void for_each_sum (const boost::type<Element>&,Element& Val, Functor& xform, double* sum) {
  *sum += xform(Val);
}

template <typename Element, typename Iterator, typename Functor>
void for_each_sum (const boost::type<Element>& type_dispatch,
               Iterator begin, Iterator end,
               Functor& xform, double* sum) {
  while (begin != end) {
    for_each_sum(type_dispatch,*begin,xform,sum);
    ++begin;
  }
}


template <typename Array, typename Functor>
void for_each_sum (Array& A, Functor xform, double* sum) {
  // Dispatch to the proper function
  for_each_sum(boost::type<typename Array::element>(),A.begin(),A.end(),xform,sum);
}


#endif // FOR_EACH_SUM_HPP
