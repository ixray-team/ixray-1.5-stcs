#include "stdafx.h"

#include "uv_tri.h"
#include "xrface.h"



void UVtri ::read( IReader	&r )
{
	_TCF::read( r );
	VERIFY( read_faces );
	read_faces->read( r, owner );
}
void UVtri ::write( IWriter	&w ) const
{
	_TCF::write( w );
	VERIFY( owner );
	VERIFY( write_faces );
	write_faces->write( w, owner );
}