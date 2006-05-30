#include "framework.h"
// Includes for all Aspects
#include "marriageaspect.h"
#include "coloraspect.h"
#include "employeraspect.h"
#include "geographyaspect.h"
#include "socialcircleaspect.h"
#include "educationlevelaspect.h"
#include "distanceaspect.h"
#include "linksaspect.h"

// Case for all Aspects
AspectBase * Activator::GetAspectByName(std::string &name)
{
	if(name == "Marriage")
		return new MarriageAspect();
	if(name == "Color")
		return new ColorAspect();
	if(name == "Employer")
		return new EmployerAspect();
	if(name == "Geography")
		return new GeographyAspect();
	if(name == "Links")
		return new LinksAspect();
	if(name == "SocialCircle")
		return new SocialCircleAspect();
	if(name == "Distance")
		return new DistanceAspect();
	if(name == "EducationLevel")
		return new EducationLevelAspect();

	throw("Unknown aspect " + name);
}
