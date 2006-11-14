#include "framework.h"
#include "socialcircleaspect.h"
#include "distributions.h"

void SocialCircleAspect::Evolve()
{

}
int SocialCircleAspect::GetInfoSize()
{
	return sizeof(SocialCircleInfo);
}
void SocialCircleAspect::InitializeAspect()
{
	int circlesCount = GetConfigInt("Circles");
	_seed = GetConfigLong("Seed"); Seed = &_seed;
	// create circlesize array
	//_circleSizes = (int *) malloc(sizeof(int) * circlesCount);
	_circleSizes = new int[circlesCount];// (int *) malloc(sizeof(int) * circlesCount);
	memset(_circleSizes, 0, sizeof(int) * circlesCount);
	// Set random socialcircles for everybody...
	int circle;
	for (Population::Iterator agentId = this->begin;
		agentId != this->end; ++agentId)
	{
		circle = (int)floor(circlesCount * ran1(Seed));
    	(*this)[*agentId]->SocialCircle = circle;
		_circleSizes[circle]++;
	}
}

SocialCircleInfo * SocialCircleAspect::operator[](key agentId)
{
	return (SocialCircleInfo *) this->GetInfo(agentId);
}

void SocialCircleAspect::ShowValues(int agentId, std::vector <char *> & fields,
								 varValue *values)
{
	for (std::vector <char *>::size_type n = 0; n < fields.size();n++)
	{
		if (strcmp(fields[n], "Circle")==0)
			values[n] = varValue((*this)[agentId]->SocialCircle);
		else
			values[n] = varValue(0);
	}
}
void SocialCircleAspect::ReleaseAspect()
{
	delete(_circleSizes);
}
int SocialCircleAspect::CircleSizeOfAgent(int agentId)
{
	int circle = (*this)[agentId]->SocialCircle;
	return _circleSizes[circle];
}

SocialCircleAspect::~SocialCircleAspect()
{
	;
}
