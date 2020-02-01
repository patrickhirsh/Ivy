#include "Core/IvyPCH.h"
#include "Data/Object.h"

namespace Ivy
{
	Ivy::Ref<Object> Object::Create(
		std::string objPath)
	{
		Ivy::Ref<Object> object = Ivy::Ref<Object>(new Object());
		object->_objPath = objPath;
		return object;
	}
}