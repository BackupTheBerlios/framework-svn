#include <string>
#include <vector>
#include <typeinfo>
#include "bxmlnode.h"
#include "variant.h"

#define MAKE_AGENT_ID(s, p) (((s) << 24) | (p))
#define AGENT_ID_POPULATION(a) ((a) >> 24)
#define AGENT_ID_POSITION(a) ((a) & 0xFFFFFF)

#ifndef _ASPECT
	#define _ASPECT
typedef unsigned int key;
using std::string;
using std::vector;


//#include "population.h"
class extendedIterator : public std::iterator <std::forward_iterator_tag, int>
{
public:
	virtual int GetSize() = 0;// { throw "not implemeted"; };
};
class AspectBase
{
public:
	AspectBase();
	virtual			~AspectBase();
	void virtual	Evolve(void) = 0;
	void virtual	InitializeAspect(void) = 0;
	void virtual	ReleaseAspect(void) { ; };
	
	int virtual		GetInfoSize(void) = 0;
	int				GetConfigInt(char *key);
	std::string		GetConfigString(char *key);
	double			GetConfigDouble(char *key);
	float			GetConfigFloat(char *key);
	long			GetConfigLong(char *key);
	
	void			Initialize();
	int				ReferenceCount;
	float			DT;
	basicxmlnode	*Config;
	std::string		Name;
	std::string		virtual GetType(void);
	//std::iterator<std::forward_iterator_tag, int> *begin;
	extendedIterator *begin;
	//std::iterator<std::forward_iterator_tag, int> *end;
	unsigned int	end;
	void			AppendBlock(unsigned int size);
	char *			GetInfo(key agentId);
	AspectBase *	GetAspect(const std::string &typeName);//type_info &type)
	AspectBase *	TryGetAspect(const std::string &typeName);
	void			LoadAspects(std::vector <AspectBase *> &_aspects);
	void virtual	ShowValues(int agentId, std::vector <char *> &fields,
								 varValue * values) = 0;
private:
	std::vector <char *>		_data;
	std::vector <AspectBase *>	_aspects;
	int							_infosize;
};

#endif

