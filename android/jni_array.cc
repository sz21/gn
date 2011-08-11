// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/android/jni_array.h"

#include "base/android/jni_android.h"
#include "base/android/scoped_java_reference.h"
#include "base/logging.h"

namespace base {
namespace android {

jbyteArray ToJavaByteArray(JNIEnv* env,
                           const unsigned char* bytes,
                           size_t len) {
  jbyteArray byte_array = env->NewByteArray(len);
  if (!byte_array) {
    return NULL;
  }

  jbyte* elements = env->GetByteArrayElements(byte_array, NULL);
  DCHECK(elements);
  memcpy(elements, bytes, len);
  env->ReleaseByteArrayElements(byte_array, elements, 0);
  CheckException(env);

  return byte_array;
}

jobjectArray ToJavaArrayOfByteArray(JNIEnv* env,
                                    const std::vector<std::string>& v) {
  size_t count = v.size();
  DCHECK_GT(count, 0U);
  jclass byte_array_class = env->FindClass("[B");
  jobjectArray joa = env->NewObjectArray(count, byte_array_class, NULL);
  if (joa == NULL)
    return NULL;

  for (size_t i = 0; i < count; ++i) {
    ScopedJavaReference<jobject> byte_array(env, ToJavaByteArray(env,
            reinterpret_cast<const uint8*>(v[i].data()), v[i].length()));
    if (!byte_array.obj()) {
      env->DeleteLocalRef(joa);
      return NULL;
    }
    env->SetObjectArrayElement(joa, i, byte_array.obj());
  }
  return joa;
}

}  // namespace android
}  // namespace base
