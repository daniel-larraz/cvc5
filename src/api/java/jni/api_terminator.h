/******************************************************************************
 * This file is part of the cvc5 project.
 *
 * Copyright (c) 2009-2026 by the authors listed in the file AUTHORS
 * in the top-level source directory and their institutional affiliations.
 * All rights reserved.  See the file COPYING in the top-level source
 * directory for licensing information.
 * ****************************************************************************
 *
 * The cvc5 Java API.
 */

#ifndef CVC5__API_TERMINATOR_H
#define CVC5__API_TERMINATOR_H
#include <cvc5/cvc5.h>
#include <jni.h>

/**
 * Terminator that forwards termination requests to the `terminate()` method
 * of a Java object of (a subclass of) class AbstractTerminator.
 */
class ApiTerminator : public cvc5::Terminator
{
 public:
  /**
   * Constructor.
   * @param env        The JNI environment of the calling thread.
   * @param terminator The Java terminator object. This class creates (and
   *                   owns) a global reference to this object.
   */
  ApiTerminator(JNIEnv* env, jobject terminator);
  /** Destructor, releases the global reference to the Java object. */
  ~ApiTerminator() override;

  /**
   * Call the `terminate()` method of the Java terminator object.
   *
   * @note The JNI environment is retrieved from the Java VM for the calling
   *       thread, which allows the solver to run in a different (Java) thread
   *       than the one that connected the terminator.
   *
   * @return True if the solver should be terminated.
   */
  bool terminate() override;

 private:
  /** Retrieve the JNI environment of the current thread. */
  JNIEnv* getEnv() const;

  /** The Java VM. */
  JavaVM* d_vm;
  /** Global reference to the Java terminator object. */
  jobject d_terminator;
  /** Method id of `terminate()`. */
  jmethodID d_terminateMethod;
};

#endif  // CVC5__API_TERMINATOR_H
