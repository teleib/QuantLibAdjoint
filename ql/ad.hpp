﻿/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2015 CompatibL

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/license.shtml>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

#ifndef quantlib_ad_hpp
#define quantlib_ad_hpp

namespace QuantLib {}

// Namespace of the library from which tdouble is invoked
// Used only if a separate flag is set to avoid operators in global scope
#define cl_ext QuantLib

#include <cl/tape/tape.hpp>

// Define QL_REAL as double replacement variable
#define QL_REAL cl::tdouble

namespace QuantLib
{
    template <typename > class Null;

    // Specialization of Null template to make it work with non-native double
    template <typename Base>
    class Null<cl::tape_wrapper<Base>>
    {
    public:
        Null() {}
        cl::tape_wrapper<Base> operator -()
        {
            return -std::numeric_limits<double>::max();
        }

        inline operator cl::tape_wrapper<Base>()
        {
            return cl::tape_wrapper<Base>(std::numeric_limits<double>::max());
        }
    };
}

#endif
