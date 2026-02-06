# Copyright (C) 2026 Sergey Naumov <sergey@naumov.io>
# SPDX-License-Identifier: 0BSD

from CustomPackage import VectorInt, CustomTask

if __name__ == '__main__':

    vector = VectorInt()

    for i in range(5):
        vector.push_back(i)

    print("type: ", type(vector))
    print("size: ", len(vector))
    print("sum: ", CustomTask.vectorSum(vector))
