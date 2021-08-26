//
//  ASAvailability.h
//  Texture
//
//  Copyright (c) Facebook, Inc. and its affiliates.  All rights reserved.
//  Changes after 4/13/2017 are: Copyright (c) Pinterest, Inc.  All rights reserved.
//  Licensed under Apache 2.0: http://www.apache.org/licenses/LICENSE-2.0
//

#import <CoreFoundation/CFBase.h>

#pragma once

#ifdef __i386__
  #define AS_TLS_AVAILABLE 0
#else
  #define AS_TLS_AVAILABLE 1
#endif

#ifndef kCFCoreFoundationVersionNumber_iOS_10_0
  #define kCFCoreFoundationVersionNumber_iOS_10_0 1348.00
#endif

#ifndef kCFCoreFoundationVersionNumber_iOS_11_0
  #define kCFCoreFoundationVersionNumber_iOS_11_0 1438.10
#endif

#ifndef __IPHONE_11_0
  #define __IPHONE_11_0 110000
#endif

#ifndef __IPHONE_13_0
  #define __IPHONE_13_0 130000
#endif

#define AS_AT_LEAST_IOS10  (kCFCoreFoundationVersionNumber >= kCFCoreFoundationVersionNumber_iOS_10_0)
#define AS_AT_LEAST_IOS11  (kCFCoreFoundationVersionNumber >= kCFCoreFoundationVersionNumber_iOS_11_0)
#define AS_AT_LEAST_IOS13  (__IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_13_0)

// Use __builtin_available if we're on Xcode >= 9, AS_AT_LEAST otherwise.
#if __has_builtin(__builtin_available)
  #define AS_AVAILABLE_IOS(ver)               __builtin_available(iOS ver, *)
  #define AS_AVAILABLE_TVOS(ver)              __builtin_available(tvOS ver, *)
  #define AS_AVAILABLE_IOS_TVOS(ver1, ver2)   __builtin_available(iOS ver1, tvOS ver2, *)
#else
  #define AS_AVAILABLE_IOS(ver)               (TARGET_OS_IOS && AS_AT_LEAST_IOS##ver)
  #define AS_AVAILABLE_TVOS(ver)              (TARGET_OS_TV && AS_AT_LEAST_IOS##ver)
  #define AS_AVAILABLE_IOS_TVOS(ver1, ver2)   (AS_AVAILABLE_IOS(ver1) || AS_AVAILABLE_TVOS(ver2))
#endif

#define AS_PIN_REMOTE_IMAGE __has_include(<PINRemoteImage/PINRemoteImage.h>)
