#include "framework.h"
#include "distanceaspect.h"

#define IABS(a,b) (a>b ? a-b : b-a)

/* Sample of a dynamic binary aspect */

float DistanceAspect::GeographicDistance(key agentId1, key agentId2)
{
   if (_geographyAspect == NULL) return 0;

	float dist=0.0;
   /*Generic Minkowski metric delta=1 Manhattan delta=2 Euclid*/
   GeographyInfo *agentId1_Info = (*_geographyAspect)[agentId1];
   GeographyInfo *agentId2_Info = (*_geographyAspect)[agentId2];
   dist = _geographyAspect->GeographicDistance(agentId1_Info->X, agentId1_Info->Y, agentId2_Info->X, agentId2_Info->Y);
   
   if (dist)
		return exp(log(dist) / _delta);
   else
		return 0.0;
}
/*
float DistanceAspect::GeographicDistance(float x1, float y1, float x2, float y2)
{
   float dist=0.0;
   // Generic Minkowski metric delta=1 Manhattan delta=2 Euclid
   if (x1 != x2)
	   dist += exp(_delta * log(IABS(x1, x2)));
   if (y1 != y2)
	   dist += exp(_delta * log(IABS(y1, y2)));

   if (dist)
		return exp(log(dist) / _delta);
   else
		return 0.0;
}*/
/*symmetric distance*/
int DistanceAspect::EducationDistance(key agentId1, key agentId2)
{
   if (_educationLevelAspect == NULL) return 0;

   return IABS((*_educationLevelAspect)[agentId1]->EducationLevel,
	   (*_educationLevelAspect)[agentId2]->EducationLevel);
}
int DistanceAspect::CircleDistance(key agentId1, key agentId2)
{
    if (_socialCircleAspect == NULL) return 0;

	return IABS((*_socialCircleAspect)[agentId1]->SocialCircle,
	   (*_socialCircleAspect)[agentId2]->SocialCircle);
}

void DistanceAspect::Evolve()
{
}
int DistanceAspect::GetInfoSize()
{
	return 0;
}
void DistanceAspect::InitializeAspect()
{
	// aspects
	_geographyAspect = (GeographyAspect *) this->TryGetAspect(typeid(GeographyAspect).name());
	_educationLevelAspect = (EducationLevelAspect *) this->TryGetAspect(typeid(EducationLevelAspect).name());
	_socialCircleAspect = (SocialCircleAspect *) this->TryGetAspect(typeid(SocialCircleAspect).name());
	// params
	_delta = GetConfigInt("Delta");
}

void DistanceAspect::ShowValues(int agentId, std::vector <char *> & fields,
								 varValue *values)
{
}
