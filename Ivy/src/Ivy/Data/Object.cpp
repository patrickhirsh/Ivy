#include "IvyPCH.h"
#include "Object.h"

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