#ifndef TESTING_CORE_HPP
#define TESTING_CORE_HPP

#include "Expectation.hpp"
#include "testing-api.hpp"

#define expect(expr) test::expect(__FILE__, __LINE__, expr)

#endif
