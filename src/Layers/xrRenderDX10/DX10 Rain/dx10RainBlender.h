#ifndef	dx10RainBlender_included
#define	dx10RainBlender_included

class CBlender_rain	: public IBlender  
{
public:
	virtual		LPCSTR		getComment()	{ return "INTERNAL: DX10 Rain blender";	}
	virtual		BOOL		canBeDetailed()	{ return FALSE;	}
	virtual		BOOL		canBeLMAPped()	{ return FALSE;	}

	virtual		void		Compile			(CBlender_Compile& C);
};

#endif	//	dx10RainBlender_included