/**
 * @file common.h
 * @brief Common found plugin functionalities which are used throughout the plugins.
 *
 * Copyright (c) 2022 Deutsche Telekom AG.
 *
 * This source code is licensed under BSD 3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef SRPC_COMMON_H
#define SRPC_COMMON_H

#include "types.h"

#include <stdbool.h>
#include <sysrepo_types.h>
#include <sysrepo.h>

/**
 * Safely call a function and jump to an error point if and error occurs - uses assumed variable name "error".
 *
 * @param func_call Written function call like printf("Hello World").
 * @param jump_point Where to jump using goto if an error occurs.
 *
 */
#define SRPC_SAFE_CALL(func_call, jump_point)                                                                          \
    do                                                                                                                 \
    {                                                                                                                  \
        if ((error = func_call) != 0)                                                                                  \
        {                                                                                                              \
            SRPLG_LOG_ERR(PLUGIN_NAME, "%s:%d %s error (%d)", __FILE__, __LINE__, #func_call, error);                  \
            goto jump_point;                                                                                           \
        }                                                                                                              \
    } while (0)

/**
 * Safely call a function and jump to an error point if and error occurs - checks for returned pointer to be NULL.
 *
 * @param ptr_var Pointer variable to be set to the function return.
 * @param func_call Written function call like printf("Hello World").
 * @param jump_point Where to jump using goto if an error occurs.
 *
 */
#define SRPC_SAFE_CALL_PTR(ptr_var, func_call, jump_point)                                                             \
    do                                                                                                                 \
    {                                                                                                                  \
        if ((ptr_var = func_call) == NULL)                                                                             \
        {                                                                                                              \
            SRPLG_LOG_ERR(PLUGIN_NAME, "%s:%d %s error (NULL)", __FILE__, __LINE__, #func_call);                       \
            goto jump_point;                                                                                           \
        }                                                                                                              \
    } while (0)

/**
 * Safely call a function and jump to an error point if and error occurs - checks for returned error code to be 0.
 *
 * @param err_var Int error code variable to be set to the function return.
 * @param func_call Written function call like printf("Hello World").
 * @param jump_point Where to jump using goto if an error occurs.
 *
 */
#define SRPC_SAFE_CALL_ERR(err_var, func_call, jump_point)                                                             \
    do                                                                                                                 \
    {                                                                                                                  \
        if ((err_var = func_call) != 0)                                                                                \
        {                                                                                                              \
            SRPLG_LOG_ERR(PLUGIN_NAME, "%s:%d %s error (%d)", __FILE__, __LINE__, #func_call, err_var);                \
            goto jump_point;                                                                                           \
        }                                                                                                              \
    } while (0)

/**
 * Check wether the datastore contains any data or not based on the provided path to check.
 *
 * @param session Sysrepo session to the datastore to check.
 * @param path Path to the data for checking.
 * @param empty Boolean value to set.
 *
 * @return Error code - 0 on success.
 */
int srpc_check_empty_datastore(sr_session_ctx_t *session, const char *path, bool *empty);

/**
 * Iterate changes for the provided xpath and use callback on each change.
 *
 * @param priv Private user data - pass plugin context.
 * @param session Sysrepo session to use for iteration.
 * @param xpath XPath for the changes iterator.
 * @param cb Callback to call on each change.
 *
 * @return Error code - 0 on success.
 */
int srpc_iterate_changes(void *priv, sr_session_ctx_t *session, const char *xpath, srpc_change_cb cb);

/**
 * Copy file from source to destination.
 *
 * @param source Source file path.
 * @param destination Destination file path.
 *
 * @return Error code - 0 on success.
 */
int srpc_copy_file(const char *source, const char *destination);

/**
 * Get information about a feature from the current session.
 *
 * @param session Sysrepo session.
 * @param module YANG module to use.
 * @param feature Feature in the YANG module.
 * @param enabled Set wether the feature is enabled or not.
 *
 * @return Error code - 0 on success.
 */
int srpc_check_feature_status(sr_session_ctx_t *session, const char *module, const char *feature, bool *enabled);

#endif // SRPC_COMMON_H