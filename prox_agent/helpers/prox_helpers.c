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

/**
 * @file prox_helpers.c
 * @brief Proximety Cloud Agent - Helpers
 * */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "prox_helpers.h"

/**
 * \brief Concatenate 4 IP address digits into one 4-byte variable
 */
uint32_t prox_2uint_ip(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
	union { uint32_t i; uint8_t ip[4]; } ip_int = {.ip = {a, b, c, d}};
	return ip_int.i;
}

/**
 * \brief Converts IPv4 address from string to 4-bytes variable
 */
uint32_t prox_str2bin_ip(const char *str)
{
	const char *ip0 = str;
	const char *ip1 = strchr(ip0, '.');
	if (ip1++ == NULL)
		return 0;
	const char *ip2 = strchr(ip1, '.');
	if (ip2++ == NULL)
		return 0;
	const char *ip3 = strchr(ip2, '.');
	if (ip3++ == NULL)
		return 0;
	return prox_2uint_ip(atoi(ip0), atoi(ip1), atoi(ip2), atoi(ip3));
}
