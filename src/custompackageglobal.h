// Copyright (C) 2026 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef CUSTOMPACKAGEGLOBAL_H
#define CUSTOMPACKAGEGLOBAL_H

#if defined _WIN32 || defined __CYGWIN__
#  define DECL_EXPORT __declspec(dllexport)
#  define DECL_IMPORT __declspec(dllimport)
#else
#  define DECL_EXPORT __attribute__((visibility("default")))
#  define DECL_IMPORT __attribute__((visibility("default")))
#endif

#if defined(BUILD_CUSTOM_PACKAGE_IMPL_LIB)
#  define CUSTOM_PACKAGE_EXPORT DECL_EXPORT
#else
#  define CUSTOM_PACKAGE_EXPORT DECL_IMPORT
#endif

#endif // CUSTOMPACKAGEGLOBAL_H
