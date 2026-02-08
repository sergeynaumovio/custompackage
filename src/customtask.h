// Copyright (C) 2026 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef CUSTOMTASK_H
#define CUSTOMTASK_H

#include "custompackageglobal.h"
#include <vector>

class CustomTaskPrivate;

class CUSTOM_PACKAGE_EXPORT CustomTask
{
    CustomTaskPrivate *const d_ptr;

public:
    CustomTask();
    CustomTask(const CustomTask &) = delete;
    CustomTask &operator=(const CustomTask &) = delete;
    ~CustomTask ();

    static int vectorSum(const std::vector<int> &vector);
};

#endif // CUSTOMTASK_H
