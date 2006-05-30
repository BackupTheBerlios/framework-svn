#include "geographyaspect.h"
#include "socialcircleaspect.h"
#include "educationlevelaspect.h"

#ifndef DISTASP
  #define DISTASP

class DistanceAspect : public AspectBase
{
public:
	void Evolve();
	void InitializeAspect();
	int GetInfoSize();
	void virtual ShowValues(int agentId, std::vector <char *> &fields,
								 varValue *values);
	float GeographicDistance(key agentId1, key agentId2);
	int EducationDistance(key agentId1, key agentId2);
	int CircleDistance(key agentId1, key agentId2);
private:
	GeographyAspect			*_geographyAspect;
	EducationLevelAspect	*_educationLevelAspect;
	SocialCircleAspect		*_socialCircleAspect;
	int						_delta;
};

#endif

