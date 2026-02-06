// Copyright (C) 2026 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "customtask.h"

#if BUILD_WITH_HOLON
// include Holon header
#endif

#include <numeric>

class CustomTaskPrivate
{
};

CustomTask::CustomTask() : d_ptr(new CustomTaskPrivate)
{
}

CustomTask::~CustomTask()
{
}

int CustomTask::vectorSum(const std::vector<int> &vector)
{

#if BUILD_WITH_HOLON

    // interop with holon

#endif

    return std::accumulate(vector.begin(), vector.end(), 0);
}
