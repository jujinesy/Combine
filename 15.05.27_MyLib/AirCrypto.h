/**----------------------------------------------------------------------------
 * AIRCrypto.h
 *-----------------------------------------------------------------------------
 * OpenSSL ���̺귯���� �̿��� ��ȣȭ ���
 *-----------------------------------------------------------------------------
 * All rights reserved by somma (fixbrain@gmail.com, unsorted@msn.com)
 *-----------------------------------------------------------------------------
 * 20:9:2011   15:22 created
**---------------------------------------------------------------------------*/
#pragma once

#include <intsafe.h>
#include "StatusCode.h"

DTSTATUS 
AirCryptBuffer(
	IN unsigned char* PassPhrase,
	IN UINT32 PassPhraseLen,
	IN unsigned char* Input, 
	IN UINT32 InputLength, 
	OUT unsigned char*& Output, 
	OUT UINT32& OutputLength, 
	IN BOOL Encrypt
	);