#include "framework.h"
#include "educationlevelaspect.h"
#include "distributions.h"

void EducationLevelAspect::Evolve()
{

}
int EducationLevelAspect::GetInfoSize()
{
	return sizeof(EducationLevelInfo);
}
void EducationLevelAspect::InitializeAspect()
{
	_toleranceHomophilia = GetConfigFloat("ToleranceHomophilia");
	_toleranceHeterophilia = GetConfigFloat("ToleranceHeterophilia");
	_levels = GetConfigInt("Levels");
	_seed	= GetConfigLong("Seed"); Seed = &_seed;
	// Set random educationlevels for everybody...
	for (Population::Iterator agentId = this->begin;
		agentId != this->end; ++agentId)
	{
		(*this)[*agentId]->EducationLevel= (int)floor(_levels * ran1(Seed));
	}
}

EducationLevelAspect::~EducationLevelAspect()
{
	;
}

float EducationLevelAspect::pdistrHomophilia(int gap)
{
   float proba;
   /*For members of the same circle of interests*/
   proba = exp(-gap*gap / (_toleranceHomophilia * _levels * _levels));	
   return proba;
}

float EducationLevelAspect::pdistrHeterophilia(int gap)
{
   float proba;
   /*For members not in the same circle of interests*/
   if (_toleranceHeterophilia > 0)
	   proba = (float) (0.5 * exp(-gap * gap / (_toleranceHeterophilia * _levels * _levels)));	
   else 
	   proba = 0.0;	
   return proba;
}

EducationLevelInfo * EducationLevelAspect::operator[](key agentId)
{
	return (EducationLevelInfo *) this->GetInfo(agentId);
}

void EducationLevelAspect::ShowValues(int agentId, std::vector <char *> & fields,
								 varValue *values)
{
	for (std::vector <char *>::size_type n = 0; n < fields.size();n++)
	{
		if (strcmp(fields[n], "EducationLevel")==0)
			values[n] = varValue((*this)[agentId]->EducationLevel);
		else
			values[n] = varValue(0);
	}
}
