//
// Created by reuben on 3/4/17.
//

#ifndef AEVO_OPENCL_UTILS_HPP
#define AEVO_OPENCL_UTILS_HPP

#include <CL/cl_platform.h>

namespace opencl {
	class Utils {
	public:
		static const char *getErrorString(cl_int);
	};
}

#define handleError() if(ret != CL_SUCCESS){    throw runtime_error(opencl::Utils::getErrorString(ret));    }

#endif //AEVO_OPENCL_UTILS_HPP
