/*******************************************************************************
*
* Proximetry AMP Agent Application
*
* Copyright (C) 2016 Proximetry Inc. All rights reserved.
* Authors: Proximetry (www.proximetry.io)
*
* THIS SOFTWARE IS PROVIDED BY PROXIMETRY "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
* EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL PROXIMETRY BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
* OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************/


#ifndef prox_helpers_h
#define prox_helpers_h

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

/**
 * \brief Concatenate 4 IP address digits into one 4-byte variable
 */
// inline uint32_t ip_int(uint8_t a, uint8_t b, uint8_t c, uint8_t d);

/**
 * \brief Converts IPv4 address from string to 4-bytes variable
 */
uint32_t read_ip(const char *str);

#ifndef ARRAY_ELEMS
    #define ARRAY_ELEMS(arr)  (sizeof(arr) / sizeof(arr[0]))
#endif

#define FLOAT_TO_INT_POINT_INT_FORMAT  		"%d.%d"
#define FLOAT_TO_INT_POINT_INT_VALUES(x) 	(int)x,(int)((int)(x*10)%10)

#define UNUSED(x) (void)(x)

#define STR(x) #x
#define TOSTR(x) STR(x)

#endif // prox_helpers_h
