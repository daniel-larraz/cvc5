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

#ifndef CVC5__PY_TERMINATOR_H
#define CVC5__PY_TERMINATOR_H

// Python.h must come first to avoid libc macro redefinition warnings
#include <Python.h>
#include <cvc5/cvc5.h>

// Created by Cython when providing 'public api' keywords
#include "cvc5_python_base_api.h"

namespace cvc5 {

/**
 * Terminator that forwards termination requests to the `terminate()` method
 * of a Python object.
 *
 * @note This class does not hold a reference to the Python object. The Python
 *       object owns this wrapper, and the Python solver keeps the Python
 *       object alive for as long as it is connected.
 */
class PyTerminator : public Terminator
{
 public:
  PyObject* m_obj;

  PyTerminator(PyObject* obj);
  virtual ~PyTerminator();

  bool terminate() override;
};

}  // namespace cvc5

#endif /* CVC5__PY_TERMINATOR_H */
