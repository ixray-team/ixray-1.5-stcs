#include "stdafx.h"
#include "Level.h"
#include "../xrGameSpy/gamespy/md5.h"

extern	void	GetCDKey_FromRegistry(char* CDKeyStr);
char const * ComputeClientDigest(string128& dest)
{
	string128 cd_key;
	string128 md5hash;
	GetCDKey_FromRegistry(cd_key);		//cd_key is not longer than 64 bytes !
	int cd_keylen = xr_strlen(cd_key);
	if (!cd_keylen)
	{
		dest[0] = 0;
		return dest;
	}
	MD5Digest(reinterpret_cast<unsigned char*>(cd_key), cd_keylen, md5hash);
	md5hash[33] = 0;
	strcpy_s(dest, sizeof(dest), md5hash);
	return dest;
};

void CLevel::SendClientDigestToServer()
{
	string128 tmp_digest;
	NET_Packet P;
	P.w_begin(M_SV_DIGEST);
	P.w_stringZ(ComputeClientDigest(tmp_digest));
	Send(P, net_flags(TRUE, TRUE, TRUE, TRUE));
}