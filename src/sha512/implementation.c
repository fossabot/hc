/*!
 * \brief   The file contains SHA256 calculator implementation
 * \author  \verbatim
            Created by: Alexander Egorov
            \endverbatim
 * \date    \verbatim
            Creation date: 2010-08-24
            \endverbatim
 * Copyright: (c) Alexander Egorov 2009-2010
 */

#include "targetver.h"
#include "implementation.h"

apr_status_t CalculateDigest(apr_byte_t* digest, const void* input, apr_size_t inputLen)
{
    hash_context_t context = { 0 };

    SHA512Init(&context);
    SHA512Update(&context, input, inputLen);
    SHA512Final(digest, &context);
    return APR_SUCCESS;
}

apr_status_t InitContext(hash_context_t* context)
{
    SHA512Init(context);
    return APR_SUCCESS;
}

apr_status_t FinalHash(apr_byte_t* digest, hash_context_t* context)
{
    SHA512Final(digest, context);
    return APR_SUCCESS;
}

apr_status_t UpdateHash(hash_context_t* context, const void* input, apr_size_t inputLen)
{
    SHA512Update(context, input, inputLen);
    return APR_SUCCESS;
}