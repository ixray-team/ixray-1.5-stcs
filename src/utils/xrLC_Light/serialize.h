#pragma once


template<typename T>
void r_pod_vector( IReader	&r, xr_vector<T> & v )
{
	u32 cnt	= r.r_u32();
	v.resize(cnt);
	r.r(&*v.begin(),cnt*sizeof( T ));
}

template<typename T>
void w_pod_vector( IWriter	&w, const xr_vector<T> & v )
{
	u32 cnt	= v.size();
	w.w_u32( cnt );
	w.w(&*v.begin(),cnt*sizeof( T ));
}

template <typename T>
void r_pod( IReader	&r, T& v  )
{
	r.r(&v, sizeof( T ) );
}

template <typename T>
void w_pod( IWriter	&w, const T& v  )
{
	w.w(&v, sizeof( T ) );
}

template<typename T>
void r_vector( IReader	&r, xr_vector<T> & v )
{
	u32 cnt	= r.r_u32();
	v.resize(cnt);
	xr_vector<T>::iterator i= v.begin(), e = v.end();
	for(;i!=e;++i)
		i->read(r);

}

template<typename T>
void w_vector( IWriter	&w, const xr_vector<T> & v )
{
	u32 cnt	= v.size();
	w.w_u32( cnt );
	xr_vector<T>::const_iterator i= v.begin(), e = v.end();
	for(;i!=e;++i)
		i->write(w);
}


template< typename T, const int dim >
void r_vector( IReader	&r, svector< T, dim > & v )
{
	u32 cnt	= r.r_u32();
	v.resize(cnt);
	svector<T,dim>::iterator i= v.begin(), e = v.end();
	for(;i!=e;++i)
		i->read(r);

}

template< typename T, const int dim >
void w_vector( IWriter	&w, const svector< T, dim > & v )
{
	u32 cnt	= v.size();
	w.w_u32( cnt );
	svector<T,dim>::const_iterator i= v.begin(), e = v.end();
	for(;i!=e;++i)
		i->write(w);
}


static void w_sphere( IWriter	&w, const Fsphere &v )
{
	w.w(&v,sizeof(Fsphere));
}
static void r_sphere(IReader &r, Fsphere &v )
{
	r.r( &v,sizeof(Fsphere) );
}


template < class action >
class vector_serialize;

template <typename T>
class t_write
{
public:
	typedef  T type;
private:
	typedef	vector_serialize< t_write<T> > t_serialize;
	friend class t_serialize;
	const xr_vector<T*>& vec;
	

	void write( IWriter &w ) const 
	{
		xr_vector<T*>::const_iterator i = vec.begin(), e =  vec.end();
		w.w_u32(vec.size());
		for( ;i!= e; ++i )
				(*i)->write(w);
	}

	void	write	( IWriter &w, const T* f ) const
	{
		w.w_u32( t_serialize::get_id(  f, vec ) );
	}

	t_write(const xr_vector<T*>	&_vec): vec( _vec )
	{
		
		//xr_vector<T*>::const_iterator i = vec.begin(), e =  vec.end();	
		//std::sort(i, e);
	}

};		

template <typename T>
class t_read
{
public:
	typedef  T type;
private:
	typedef	vector_serialize< t_read<T> > t_serialize;
	friend class t_serialize;
	xr_vector<T*>& vec;		
		
	void read(IReader &r) 
	{
		vec.resize(r.r_u32());
		xr_vector<T*>::iterator i = vec.begin(), e =  vec.end();
		for( ;i!= e; ++i )
		{
			(*i) = T::read_create();
			(*i)->read(r);
		}
	}
	void	read	( IReader &r, T* &f ) const
	{
		VERIFY( !f );
		u32 id = r.r_u32();
		if( id == t_serialize::id_none )
			return;
		f = vec[ id ];
	}
	t_read(xr_vector<T*>	&_vec): vec( _vec )
	{
		_vec.clear();	
	}

};

template <   typename action >
class vector_serialize
{
typedef	typename	action::type	type					;
					action			serialize				;
public:
	static const	u32				id_none		= u32(-1)	;
public:

	vector_serialize(xr_vector<type*>	*_vec): serialize(*_vec)
	{

	}
	vector_serialize(const xr_vector<type*>	*_vec): serialize(*_vec)
	{

	}

static	u32		get_id			(  const type* f, const xr_vector<type*>& vec )
	{
		if( f == 0 )
			return id_none;
		xr_vector<type*>::const_iterator F = std::find( vec.begin(), vec.end(), f );
		VERIFY( F != vec.end() );
		return u32( F - vec.begin() );
	}
	void read(IReader &r)
	{
		serialize.read( r );
	}
	void write(IWriter &w) const
	{
		serialize.write( w );
	}
	void read( IReader &r, type* &f ) const
	{
		serialize.read( r, f );
	}
	void write( IWriter &w, const type* f ) const
	{
		serialize.write( w, f );
	}
};

void write(IWriter	&w, const b_texture &b);
void read(IReader &r, b_texture &b);





