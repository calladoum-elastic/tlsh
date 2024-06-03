/*
 * TLSH is provided for use under two licenses: Apache OR BSD.
 * Users may opt to use either license depending on the license
 * restictions of the systems with which they plan to integrate
 * the TLSH code.
 */

/* ==============
 * Apache License
 * ==============
 * Copyright 2013 Trend Micro Incorporated
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* ===========
 * BSD License
 * ===========
 * Copyright (c) 2013, Trend Micro Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.

 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "tlsh.h"

#include <errno.h>
#include <string.h>

#include <stdexcept>

#include "stdio.h"
#include "tlsh_impl.h"
#include "tlsh_util.h"

/////////////////////////////////////////////////////
// C++ Implementation


Tlsh::Tlsh() : m_Implementation{std::make_unique<TlshImpl>()}
{
}


void
Tlsh::display_notice()
{
    printf(
        "   =========================================================================\n"
        "   ==  NOTICE file for use with the Apache License, Version 2.0,          ==\n"
        "   ==  in this case for the Trend Locality Sensitive Hash distribution.   ==\n"
        "   =========================================================================\n"
        "\n"
        "   Trend Locality Sensitive Hash (TLSH)\n"
        "   Copyright 2010-2014 Trend Micro\n"
        "\n"
        "   This product includes software developed at\n"
        "   Trend Micro (http://www.trendmicro.com/)\n"
        "\n"
        "   Refer to the following publications for more information:\n"
        "   \n"
        "     Jonathan Oliver, Chun Cheng and Yanggui Chen,\n"
        "     \"TLSH - A Locality Sensitive Hash\"\n"
        "     4th Cybercrime and Trustworthy Computing Workshop, Sydney, November 2013\n"
        "     https://github.com/trendmicro/tlsh/blob/master/TLSH_CTC_final.pdf\n"
        "    \n"
        "     Jonathan Oliver, Scott Forman and Chun Cheng,\n"
        "     \"Using Randomization to Attack Similarity Digests\"\n"
        "     Applications and Techniques in Information Security. Springer Berlin Heidelberg, "
        "2014. 199-210.\n"
        "     https://github.com/trendmicro/tlsh/blob/master/Attacking_LSH_and_Sim_Dig.pdf\n"
        "\n"
        "     Jonathan Oliver and Jayson Pryde\n"
        "     "
        "http://blog.trendmicro.com/trendlabs-security-intelligence/"
        "smart-whitelisting-using-locality-sensitive-hashing/\n"
        "\n"
        "\n"
        "\n"
        "\n"
        "\n"
        "\n"
        "\n"
        "\n"
        "    \n"
        "   SHA1 of first 242 lines of LICENSE - so that we can append NOTICE.txt to LICENSE\n"
        "   $ head -n 242 LICENSE | openssl dgst -sha1\n"
        "   (stdin)= 11e8757af16132dd60979eacd73a525a40ff31f0\n"
        "\n");
}


void
Tlsh::update(std::vector<u8> const &data)
{
    //
    // threaded and private options only available to
    //	windowsize == 5
    //	calling final - without calling update first
    //
    const u32 tlsh_option = 0;
    m_Implementation->update(data.data(), data.size(), tlsh_option);
}

void
Tlsh::final(u32 tlsh_option)
{
    m_Implementation->final(tlsh_option);
}

std::vector<u8> const &
Tlsh::getHashBytes(u8 showvers) const
{
    return m_Implementation->hash(showvers);
}

std::string const &
Tlsh::getHashString(u8 showvers)
{
    // if the version number is illegal, just ignore
    if (showvers >= 10)
    {
        throw std::runtime_error("invalid version");
    }

    if (this->m_stringHashes[showvers].empty())
    {
        this->m_stringHashes[showvers] = [this, showvers]()
        {
            auto const &res = this->getHashBytes(showvers);
            auto hex        = std::vector<u8>(TLSH_STRING_LEN_REQ);

            if (0 < showvers && showvers < 10)
            {
                hex[0] = 'T';
                hex[1] = '0' + showvers;
                to_hex(res.data(), res.size(), &hex[2]);
            }
            else
            {
                hex.resize(TLSH_STRING_LEN_REQ - 2);
                to_hex(res.data(), res.size(), &hex[0]);
            }

            return res.size() == 0 ? "" : std::string((char *)hex.data(), hex.size());
        }();
    }

    return this->m_stringHashes[showvers];
}

void
Tlsh::reset()
{
    m_Implementation->reset();
    m_stringHashes.fill("");
}


bool
Tlsh::operator==(const Tlsh &other) const
{
    if (this == &other) [[unlikely]]
    {
        return true;
    }

    return 0 == m_Implementation->compare(*other.m_Implementation);
}

bool
Tlsh::operator!=(const Tlsh &other) const
{
    return !this->operator==(other);
}

int
Tlsh::Lvalue()
{
    return m_Implementation->Lvalue();
}
int
Tlsh::Q1ratio()
{
    return m_Implementation->Q1ratio();
}
int
Tlsh::Q2ratio()
{
    return m_Implementation->Q2ratio();
}
int
Tlsh::Checksum(int k)
{
    return m_Implementation->Checksum(k);
}
int
Tlsh::BucketValue(int bucket)
{
    if (bucket >= (EFF_BUCKETS - 1))
        return -1;

    return m_Implementation->BucketValue(bucket);
}

int
Tlsh::HistogramCount(int bucket)
{
    if (bucket >= (EFF_BUCKETS - 1))
        return -1;

    return m_Implementation->HistogramCount(bucket);
}

int
Tlsh::totalDiff(const Tlsh &other, bool len_diff) const
{
    if (this->impl().get() == other.impl().get())
    {
        return 0;
    }

    return this->m_Implementation->totalDiff(other.impl(), len_diff);
}

int
Tlsh::fromTlshStr(const std::string &str)
{
    return m_Implementation->fromTlshStr(str);
}

int
Tlsh::fromTlshBytes(const std::vector<u8> &bytes)
{
    return m_Implementation->fromTlshBytes(bytes);
}

bool
Tlsh::isValid() const
{
    return m_Implementation->isValid();
}
