#pragma once
#include "hxgrid/Interface/IAgent.h"
//#include "hxgrid/Interface/IGenericStream.h"


class XRLC_LIGHT_API byte_count
{
	u32					_count;
	
public:
			byte_count	():_count( 0 ){}
	IC u32	count		(){ return _count ; }
	IC void reset		(){ _count = 0; }
	IC void add			(u32 cnt ){ _count+=cnt; }
};



class CMemoryReader
{
	const	u32		file_size;
	mutable	u8*		data;
	mutable u32		position;
public:
					CMemoryReader	( const u32	file_size_ );
					~CMemoryReader	();
	void			r				( void *p, int cnt )const;
	u32				count			()const { return file_size - position; }
	void			*pdata			()const { return (void*)data; }		
};

class XRLC_LIGHT_API INetReader : 
	public IReaderBase<INetReader>,
	public byte_count
			
{
public:
	INetReader		(IGenericStream	 *_stream): stream(_stream), mem_reader(0)
	{
		VERIFY(stream);
	}
	~INetReader		();
	IC int			elapsed		()	const		{	VERIFY(false); return 0;		};
	void			seek		(u32 pos)		{	VERIFY(false); 		};



	IC int			tell		()	const		{	VERIFY(false); return 0;	};
	//IC void			seek		(int ptr)		{	VERIFY(false); }
	IC int			length		()	const		{	VERIFY(false); return 0; };
//	IC void*		pointer		()	const		{	VERIFY(false); return 0; 	};
	IC void			advance		(int cnt)		{	VERIFY(false);}


	void			r			(void *p,int cnt);
	
	void			r_string	(char *dest, u32 tgt_sz);
	void			r_stringZ	( char *dest );
	void			r_stringZ	( shared_str &dest );
	u32 			find_chunk  (u32 ID, BOOL* bCompressed = 0);

private:
	typedef IReaderBase<INetReader>	inherited;
	
private:
	IGenericStream*	stream;
	CMemoryReader *	mem_reader;
};

class XRLC_LIGHT_API INetWriter : 
	public IWriter,
	public byte_count
{
	IGenericStream		*stream;
	CMemoryWriter		*mem_writter;
	u32					net_block_write_data_size;
	

	virtual void	seek	(u32 pos)						{ VERIFY(false); }
	virtual u32		tell	()								{ VERIFY(false); return 0; };
			
public:
	INetWriter(IGenericStream *_stream, u32 _block_size): stream(_stream), mem_writter(0), net_block_write_data_size(_block_size)
	{
		//VERIFY(stream);
	}
	~INetWriter( );
			void			send_and_clear	();
			void			send_not_clear	(IGenericStream	 * _stream);
			void			clear			();
			void			reset_net_stream( IGenericStream	 *_stream ) { stream = _stream; }
			//IGenericStream	*net_stream		(){ return stream ; }
	virtual void			w				( const void* ptr, u32 count )	;
};