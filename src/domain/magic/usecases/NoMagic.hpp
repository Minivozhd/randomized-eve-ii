#pragma once
#include <iostream>
namespace domain::magic::no_magic {
// USA gameplay package; validates every preimage before changing any byte.
void apply(std::iostream &gameplay);
} // namespace domain::magic::no_magic
