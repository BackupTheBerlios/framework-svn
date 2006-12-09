#include "framework.h"
#include "geographyaspect.h"
#include "distributions.h"

#define IABS(a,b) (a>b ? a-b : b-a)

void GeographyAspect::Evolve()
{

}
int GeographyAspect::GetInfoSize()
{
	return sizeof(GeographyInfo);
}
void GeographyAspect::InitializeAspect()
{
	/*int xFocusWidth = GetConfigInt("FocusWidth");*/
	/*int yFocusHeight = GetConfigInt("FocusHeight");*/
	Alfa = GetConfigFloat("Alfa");
	_delta = GetConfigFloat("Delta");
	// Get aspects
	//_distanceAspect = (DistanceAspect *) this->GetAspect(typeid(DistanceAspect).name());

	_seed = GetConfigLong("Seed"); Seed = &_seed;
}

void GeographyAspect::Start()
{
	float xWidth = GetConfigFloat("Width");
	float yHeight = GetConfigFloat("Height");
	
	/* Initialize values for the agents */
	GeographyInfo *element;
	for (Population::Iterator agentId = this->begin;
		agentId != this->end; ++agentId)
	{
		element = (*this)[*agentId];
		do 
			element->X = xWidth / 2 + gasdev(Seed) * xWidth / 3;
		while (element->X < 0 || element->X > xWidth);
		do 
			element->Y = yHeight / 2 + gasdev(Seed) * yHeight / 3;
		while (element->Y < 0 || element->Y > yHeight);
	}
	// Calculate n for the grid 
	int cellsX = (int) xWidth; int cellsY = (int) yHeight;
	int *count = new int[cellsX * cellsY];
	memset(count, 0, sizeof(int) * (cellsX * cellsY));
	for (Population::Iterator agentId = this->begin;
		agentId != this->end; ++agentId)
	{
		element = (*this)[*agentId];
		count[(int) (element->X) * cellsX + 
			(int) (element->Y)]++;
	}
	// Gets the sum of all probas
	int x1, x2; int y1, y2;
	float dist;
	float proba;
	float sum_of_all_probas = 0;
	for (y1 = 0; y1 < cellsY; y1++)
		for (x1 = 0; x1 < cellsX; x1++)
		{
			// Goes through each cell
			for (y2 = 0; y2 < cellsY; y2++)
				for (x2 = 0; x2 < cellsX; x2++)
				{
					dist = GeographicDistance(x1, y1, x2, y2);
					proba = this->pdistrGeographicRaw(dist);
					int c1 = count[x1 * cellsX + y1];
					int c2 = count[x2 * cellsX + y2];
					float add = proba * c1 * c2;
					if (add > 0)
						sum_of_all_probas += add;
				}
		}
	int size = this->begin->GetSize();
	_sum_of_all_probas_norm = sum_of_all_probas / (size * size);
	_norm_of_sum = 1 / (cellsX * cellsY);
	// done
	free(count);
}

float GeographyAspect::GeographicDistance(float x1, float y1, float x2, float y2)
{
   float dist=0.0;
   /*Generic Minkowski metric delta=1 Manhattan delta=2 Euclid*/
   if (x1 != x2)
	   dist += exp(_delta * log(IABS(x1, x2)));
   if (y1 != y2)
	   dist += exp(_delta * log(IABS(y1, y2)));

   if (dist)
		return exp(log(dist) / _delta);
   else
		return 0.0;
}

float GeographyAspect::pdistrGeographic(float gdist)
{
    return pdistrGeographicRaw(gdist) / _norm_of_sum * _sum_of_all_probas_norm;
}
float GeographyAspect::pdistrGeographicRaw(float gdist)
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
	if (this->_initialized == false) return;
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
