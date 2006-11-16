#include "framework.h"

AspectBase::AspectBase()
{
	_infosize = 0;
	_initialized = false;
	ReferenceCount = 0;
	end = 0;
}
/*
void AspectBase::ShowValues(int agentId, std::vector <char *> const &fields,
								 std::vector <double> const &values)
{
}*/
AspectBase::~AspectBase()
{
	for (vector<char *>::size_type n = 0;
			n < this->_data.size(); n++)
	{
		if (_data[n] != NULL)
		{
			free(_data[n]);
		}
	}
	_aspects.clear();
}

void AspectBase::Initialize()
{
	_infosize = GetInfoSize();
	this->InitializeAspect();
	this->_initialized = true;
}

void AspectBase::AppendBlock(unsigned int size)
{
	if (size == 0)
		_data.push_back(NULL);
	else
	{
		unsigned int sizeAlloc = size * GetInfoSize();
        char *block = (char *) malloc (sizeof(char) * sizeAlloc);
		_data.push_back(block);
	}
}

int AspectBase::GetConfigInt(char *key)
{
	 return getchildxmlnodeintvalue(this->Config, key);
}
long AspectBase::GetConfigLong(char *key)
{
	 return getchildxmlnodelongvalue(this->Config, key);
}

double AspectBase::GetConfigDouble(char *key)
{
	 return getchildxmlnodedoublevalue(this->Config, key);
}
float AspectBase::GetConfigFloat(char *key)
{
	 return (float) getchildxmlnodedoublevalue(this->Config, key);
}
std::string AspectBase::GetConfigString(char *key)
{
	 return getchildxmlnodestringvalue(this->Config, key);
}
std::string AspectBase::GetType(void)
{
	return typeid(*this).name();
}

char* AspectBase::GetInfo(key agentId)
{
	unsigned int population = AGENT_ID_POPULATION(agentId);
	unsigned int position = AGENT_ID_POSITION(agentId);
	char *a1 = _data[population-1] + position * _infosize;
	return a1;
}

void AspectBase::LoadAspects(std::vector <AspectBase *> &aspects)
{
	_aspects = aspects;
}


AspectBase * AspectBase::GetAspect(const string &typeName)
{
	AspectBase * ret = TryGetAspect(typeName);
	if (ret == NULL)
		throw ("Aspect " + typeName + " could not be found.");
	else
		return ret;
}

AspectBase * AspectBase::TryGetAspect(const string &typeName)//type_info &type)
{
	//string typeName= type.name();
	for (vector<AspectBase *>::size_type n = 0;
				n < this->_aspects.size(); n++)
	{
		if (typeName == typeid(*_aspects[n]).name())
			return _aspects[n];
	}
	return NULL;
}
