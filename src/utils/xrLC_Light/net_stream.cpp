#include "stdafx.h"

#include "net_stream.h"



void INetReader::r(void *p,int cnt)
{
	if(cnt==0)
		return;
	add( cnt );
	for(;;)
	{
		if( mem_reader )
		{
			u32 read_cnt = _min( cnt, mem_reader->count() ); 
			mem_reader->r( p, read_cnt );
			cnt-=read_cnt;
		}
		if( mem_reader && mem_reader->count() == 0 )
				xr_delete( mem_reader );

		if( cnt==0 )
				return;
		
		u32 block_size ;
		stream->Read(&block_size, sizeof(block_size) );
		mem_reader = xr_new<CMemoryReader>( block_size );
		stream->Read( mem_reader->pdata(), block_size );
	}
}
INetReader::~INetReader		()
{
	R_ASSERT(!mem_reader||mem_reader->count()==0);
	xr_delete(mem_reader);
}
/*
	IC void			w_string(const char *p)			{	w(p,(u32)xr_strlen(p));w_u8(13);w_u8(10);	}
	IC void			w_stringZ(const char *p)		{	w(p,(u32)xr_strlen(p)+1);			}
	IC void			w_stringZ(const shared_str& p) 	{	w(*p?*p:"",p.size());w_u8(0);		}
	IC void			w_stringZ(shared_str& p)		{	w(*p?*p:"",p.size());w_u8(0);		}
*/

void	INetReader::r_string	(char *dest, u32 tgt_sz)
{
	
	R_ASSERT( tgt_sz < 1024 );
	char buf[ 1024 ];
	
	buf[ 0 ] = r_u8();
	u32 i = 1;
	for( ; i < 1024 ; ++i )
	{
		
		buf[ i ] = r_u8();
		if( buf[ i-1 ] == 13 && buf[ i ] == 10 )
			break;
	}
	u32 lenght = i-1;
	R_ASSERT2( lenght < (tgt_sz-1) ,"Dest string less than needed." );
	//R_ASSERT	(!IsBadReadPtr((void*)src,sz));

	
	buf[lenght] = 0;

	strncpy_s	(dest,tgt_sz, buf, lenght+1 );

    //dest[sz]	= 0;
}


void	INetReader::r_stringZ	( char *dest )
{
	
	//R_ASSERT( tgt_sz < 1024 );
	
	u32 i = 0;
	for( ; i < 1024 ; ++i )
	{
		dest[i] = r_u8();

		if( dest[i] == 0  )
			break;
	}

}
void	INetReader::r_stringZ	( shared_str &dest )
{
	char buf[ 1024 ];
	r_stringZ( buf );
	dest._set( buf );
}

void	INetWriter::	send_and_clear()
{
	if( !mem_writter )
		return;
	R_ASSERT( stream );
	send_not_clear( stream );
	clear();
}

void	INetWriter::	send_not_clear(IGenericStream* _stream)
{
 	R_ASSERT(mem_writter);
	R_ASSERT(_stream);
	u32 block_size = mem_writter->tell();
	_stream->Write( &block_size, sizeof( block_size) );
	_stream->Write( mem_writter->pointer(), block_size );
}

void	INetWriter::	clear		  ()
{
	xr_delete( mem_writter );
}

void INetWriter::w(const void* ptr, u32 count)
{
	if(count==0)
		return;
	add( count );
	for(;;)
	{
		if( mem_writter )
		{
			u32 write_cnt = _min( count, net_block_write_data_size - mem_writter->tell()  ); 
			mem_writter->w( ptr, write_cnt );
			count-=write_cnt;
		} else
			mem_writter = xr_new<CMemoryWriter>();

		if( mem_writter && net_block_write_data_size == mem_writter->tell() )
		{
			R_ASSERT(u32(-1)!=net_block_write_data_size);

			send_and_clear ();
			
			mem_writter = xr_new<CMemoryWriter>();
		}
			
		if( count == 0 )
				return;
		
	}
}
INetWriter::~INetWriter()
{
	if(mem_writter)
		send_and_clear ();
}
void CMemoryReader::r(void *p,int cnt)const
{	
	CopyMemory	( p, data+position, cnt );
	position	+=cnt;
	R_ASSERT(position<=file_size);
}

CMemoryReader::CMemoryReader	( const u32	file_size_ ): file_size( file_size_ ), position( 0 )
{
	 data = (u8*)	Memory.mem_alloc	(file_size_
#ifdef DEBUG_MEMORY_NAME
			,		"CMemoryReader - storage"
#endif // DEBUG_MEMORY_NAME
			);
}

CMemoryReader::	~CMemoryReader	()
{
	Memory.mem_free( data );
}

#include "../../xrCore/FS_impl.h"
u32 INetReader::find_chunk				(u32 ID, BOOL* bCompressed)
{
	R_ASSERT( false );
	return inherited::find_chunk(ID, bCompressed);
}

