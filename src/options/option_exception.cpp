/******************************************************************************
 * Top contributors (to current version):
 *   Andrew Reynolds, Andres Noetzli
 *
 * This file is part of the cvc5 project.
 *
 * Copyright (c) 2009-2025 by the authors listed in the file AUTHORS
 * in the top-level source directory and their institutional affiliations.
 * All rights reserved.  See the file COPYING in the top-level source
 * directory for licensing information.
 * ****************************************************************************
 *
 * Options-related exceptions.
 */

#include "options/option_exception.h"

namespace cvc5::internal {
const std::string OptionException::s_errPrefix = "Error in option parsing: ";
const std::string FatalOptionException::s_errPrefix =
    "Fatal error in option parsing: ";
}  // namespace cvc5::internal
