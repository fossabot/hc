﻿/*
* This is an open source non-commercial project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
*/
/*!
 * \brief   The file contains backend test interface
 * \author  \verbatim
            Created by: Alexander Egorov
            \endverbatim
 * \date    \verbatim
            Creation date: 2016-08-27
            \endverbatim
 * Copyright: (c) Alexander Egorov 2009-2017
 */

#pragma once

#include "gtest.h"
#include <tchar.h>
#include <apr_file_info.h>
#include <apr.h>
#include <apr_pools.h>
#include <apr_errno.h>
#include <backend.h>

static apr_pool_t* pool_;

class BackendTest : public ::testing::Test {

protected:

    static void TearDownTestCase() {
        bend_complete();
        apr_pool_destroy(pool_);
        apr_terminate();
    }

    static void SetUpTestCase() {
        auto argc = 1;

        const char* const argv[] = { "1" };

        auto status = apr_app_initialize(&argc, (const char *const **)&argv, nullptr);

        if(status != APR_SUCCESS) {
            throw status;
        }
        apr_pool_create(&pool_, NULL);
        bend_init(pool_);
    }
};