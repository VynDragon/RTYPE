#pragma once

template<typename T>
void	delFunction(const void* object)
{
	T newobj = (T)object;
	delete newobj;
}