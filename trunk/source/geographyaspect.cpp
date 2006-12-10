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
	// Gets the sum of all probas
	_sum_of_all_probas_norm = GetProbaNormalization(&_norm_of_sum);	
}
bool GeographyAspect::hasInCache(int cellsX, int cellsY, int size, float _alfa, float _delta)
{
	float ret;
	return getFromCache(cellsX, cellsY, size, _alfa, _delta, &ret) > -1;
}
float GeographyAspect::getFromCache(int cellsX, int cellsY, int size, float _alfa, float _delta, float *norm_of_sum)
{
	if (cellsX == 10 && cellsY == 10 && size == 500 && Alfa == 2.000000 && _delta == 1.000000)
	{
		*norm_of_sum = 0.010000;  
		return 0.054910;
	}
	if (cellsX == 100 && cellsY == 100 && size == 500 && Alfa == 4.000000 && _delta == 1.000000)
	{
		*norm_of_sum = 0.000100;  
		return 0.002132;
	}
	if (cellsX == 100 && cellsY == 100 && size == 500 && Alfa == 2.000000 && _delta == 1.000000)
	{
		*norm_of_sum = 0.000100;  
		return 0.003386;
	}
	if (cellsX == 100 && cellsY == 100 && size == 500 && Alfa == 0.000000 && _delta == 1.000000)
	{
		*norm_of_sum = 0.000100;  
		return 1.000000;
	}
	if (cellsX == 100 && cellsY == 100 && size == 100000 && Alfa == 2.000000 && _delta == 1.000000)
	{
		*norm_of_sum = 0.000100;  
		return 0.008573;
	}
	if (cellsX == 100 && cellsY == 100 && size == 10000 && Alfa == 0.000000 && _delta == 1.000000)
	{
		*norm_of_sum = 0.000100;
		return 0.786092;
	}
	if (cellsX == 100 && cellsY == 100 && size == 100000 && Alfa == 0.000000 && _delta == 1.000000)
	{	
		*norm_of_sum = 0.000100;  
		return 4.081978;
	}
	else
		return -1;
}

float GeographyAspect::GetProbaNormalization(float *norm_of_sum)
{
	float xWidth = GetConfigFloat("Width");
	float yHeight = GetConfigFloat("Height");
	
	int cellsX = (int) xWidth; int cellsY = (int) yHeight;
	int size = this->begin->GetSize();
	
	if (hasInCache(cellsX, cellsY, size, Alfa, _delta))
	{
		return getFromCache(cellsX, cellsY, size, Alfa, _delta, norm_of_sum);
	}

	// Calculate n for the grid 
	int *count = new int[cellsX * cellsY];
	memset(count, 0, sizeof(int) * (cellsX * cellsY));
	GeographyInfo *element;
	for (Population::Iterator agentId = this->begin;
		agentId != this->end; ++agentId)
	{
		element = (*this)[*agentId];
		count[(int) (element->X) * cellsX + 
			(int) (element->Y)]++;
	}
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
	float sum_of_all_probas_norm;
	sum_of_all_probas_norm = sum_of_all_probas / (size * size);
	*norm_of_sum = 1.0F / (cellsX * cellsY);
	// done
	free(count);
	//
	printf("\nNoncached value for: cellsX == %i && cellsY == %i && size == %i && Alfa == %f && _delta == %f\n", cellsX, cellsY, size, Alfa, _delta);
	printf("*norm_of_sum = %f;  sum_of_all_probas_norm = %f;\n\n", *norm_of_sum, sum_of_all_probas_norm);
	return sum_of_all_probas_norm;
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
    return pdistrGeographicRaw(gdist) / _sum_of_all_probas_norm * _norm_of_sum;
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
		else if (strcmp(fields[n], "Alfa")==0)
			values[n] = varValue(Alfa);
		else
			values[n] = varValue(0);
	}
}
