#include "framework.h"
#include "geographyaspect.h"
#include "distributions.h"

void GeographyAspect::Evolve()
{

}
int GeographyAspect::GetInfoSize()
{
	return sizeof(GeographyInfo);
}
void GeographyAspect::InitializeAspect()
{
	int xWidth = GetConfigInt("Width");
	int yHeight = GetConfigInt("Height");
	int xFocusWidth = GetConfigInt("FocusWidth");
	int yFocusHeight = GetConfigInt("FocusHeight");
	Alfa = GetConfigFloat("Alfa");

	_seed = GetConfigLong("Seed"); Seed = &_seed;
	// Set random socialcircles for everybody...
	GeographyInfo *element;
	for (Population::Iterator agentId = this->begin;
		agentId != this->end; ++agentId)
	{
		element = (*this)[*agentId];
		element->X = (int)floor(xWidth/2 * gasdev(Seed) / xFocusWidth);
		element->Y = (int)floor(yHeight/2 * gasdev(Seed) / yFocusHeight);
	}
}

float GeographyAspect::pdistrGeographic(float gdist)
{
    float proba;
    proba = (float) exp(-Alfa * log(gdist + 1.0F));
    return proba;
}

GeographyInfo * GeographyAspect::operator[](key agentId)
{
	return (GeographyInfo *) this->GetInfo(agentId);
}

void GeographyAspect::ReleaseAspect()
{

}

void GeographyAspect::ShowValues(int agentId, std::vector <char *> & fields,
								 varValue *values)
{
	for (std::vector <char *>::size_type n = 0; n < fields.size();n++)
	{
		if (strcmp(fields[n], "X")==0)
			values[n] = varValue((*this)[agentId]->X);
		else if (strcmp(fields[n], "Y")==0)
			values[n] = varValue((*this)[agentId]->Y);
		else
			values[n] = varValue(0);
	}
}
