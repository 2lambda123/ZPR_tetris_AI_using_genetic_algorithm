#include "func.hpp"

/**
 * Example of class documented using Doxygen
 */
class FuncClass {};

/**
 * Example of struct documented using Doxygen
 *  - list_elem1
 *      -# e1
 *      -# e2
 *  - list_elem2
 */
struct FuncStruct2 {
    /// Exemplary enum
    enum Enum { A, B, C };
    /// Enum class
    enum class EnumClass { D, E, F };
};

int add(int a, int b) { return a + b; }
