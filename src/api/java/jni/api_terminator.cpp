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
#include "api_terminator.h"

#include <string>

using namespace cvc5;

ApiTerminator::ApiTerminator(JNIEnv* env, jobject terminator)
    : d_vm(nullptr), d_terminator(nullptr), d_terminateMethod(nullptr)
{
  env->GetJavaVM(&d_vm);
  d_terminator = env->NewGlobalRef(terminator);
  jclass terminatorClass = env->GetObjectClass(d_terminator);
  d_terminateMethod = env->GetMethodID(terminatorClass, "terminate", "()Z");
  env->DeleteLocalRef(terminatorClass);
}

ApiTerminator::~ApiTerminator()
{
  JNIEnv* env = getEnv();
  if (env != nullptr && d_terminator != nullptr)
  {
    env->DeleteGlobalRef(d_terminator);
  }
}

JNIEnv* ApiTerminator::getEnv() const
{
  JNIEnv* env = nullptr;
  if (d_vm == nullptr
      || d_vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6)
             != JNI_OK)
  {
    return nullptr;
  }
  return env;
}

bool ApiTerminator::terminate()
{
  JNIEnv* env = getEnv();
  if (env == nullptr)
  {
    // The solver runs in a thread that is not attached to the Java VM, we
    // cannot call back into Java.
    throw CVC5ApiException(
        "Terminator called from a thread that is not attached to the Java VM");
  }
  jboolean result = env->CallBooleanMethod(d_terminator, d_terminateMethod);
  if (env->ExceptionCheck())
  {
    // Convert the pending Java exception into a C++ exception, which is
    // propagated to the JNI entry point and rethrown as a Java exception.
    jthrowable exception = env->ExceptionOccurred();
    env->ExceptionClear();
    std::string message = "Exception in AbstractTerminator.terminate()";
    jclass throwableClass = env->FindClass("java/lang/Throwable");
    jmethodID toString =
        env->GetMethodID(throwableClass, "toString", "()Ljava/lang/String;");
    jstring jMessage =
        static_cast<jstring>(env->CallObjectMethod(exception, toString));
    if (!env->ExceptionCheck() && jMessage != nullptr)
    {
      const char* s = env->GetStringUTFChars(jMessage, nullptr);
      message += ": ";
      message += s;
      env->ReleaseStringUTFChars(jMessage, s);
    }
    else
    {
      env->ExceptionClear();
    }
    throw CVC5ApiException(message);
  }
  return result == JNI_TRUE;
}
