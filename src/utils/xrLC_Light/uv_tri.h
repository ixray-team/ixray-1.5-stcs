#include "hash2D.h"

#include "xrfacedefs.h"
#include "tcf.h"


struct XRLC_LIGHT_API UVtri : public _TCF		
{
	Face*	owner;
	void	read				( IReader	&r );
	void	write				( IWriter	&w ) const ;
};

typedef hash2D<UVtri*,128,128>	HASH;