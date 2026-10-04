/******************************************************************************
 * This file is part of the cvc5 project.
 *
 * Copyright (c) 2009-2026 by the authors listed in the file AUTHORS
 * in the top-level source directory and their institutional affiliations.
 * All rights reserved.  See the file COPYING in the top-level source
 * directory for licensing information.
 * ****************************************************************************
 *
 * Wrapper for the cvc5 Terminator C++ class
 */

#include "py_terminator.h"

#include <stdexcept>

namespace cvc5 {

PyTerminator::PyTerminator(PyObject* obj) : m_obj(obj)
{
  // Provided by "cvc5_python_base_api.h"
  if (import_cvc5__cvc5_python_base())
  {
    throw std::runtime_error("Error executing import_cvc5__cvc5_python_base");
  }
}

PyTerminator::~PyTerminator() {}

bool PyTerminator::terminate()
{
  if (this->m_obj)
  {
    std::string error;
    // Call the (overridden) terminate() method of the Python object
    bool result = cy_call_bool_func(this->m_obj, "terminate", &error);
    if (!error.empty()) throw std::runtime_error(error);
    return result;
  }
  else
  {
    throw std::runtime_error("'Terminator' object has not been set");
  }
}

}  // namespace cvc5
