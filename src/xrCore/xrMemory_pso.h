#pragma once

typedef void	__stdcall	pso_MemFill			(void* dest,  int value, size_t count);
typedef void	__stdcall	pso_MemFill32		(void* dest,  size_t value, size_t count);
typedef void	__stdcall	pso_MemCopy			(void* dest,  const void* src, size_t count);
