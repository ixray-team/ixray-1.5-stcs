function normal		(shader, t_base, t_second, t_detail)
--	shader	:begin		("portal","simple_color")
	shader	:begin		("portal","portal")
			:fog		(true)
			:zb 		(true,false)
			:sorting	(3, true)
			:blend		(true,blend.srcalpha,blend.invsrcalpha)
	shader	:sampler	("s_tonemap")   :texture	("$user$tonemap")
end
