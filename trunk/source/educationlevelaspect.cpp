#include "framework.h"
#include "educationlevelaspect.h"
#include "distributions.h"

#define IABS(a,b) (a>b ? a-b : b-a)

void EducationLevelAspect::Evolve()
{

}
int EducationLevelAspect::GetInfoSize()
{
	return sizeof(EducationLevelInfo);
}
void EducationLevelAspect::InitializeAspect()
{
	/*_toleranceHomophilia = GetConfigFloat("ToleranceHomophilia");*/
	/*_toleranceHeterophilia = GetConfigFloat("ToleranceHeterophilia");*/
	Beta = GetConfigFloat("Beta");
	_levels = GetConfigInt("Levels");
	_seed	= GetConfigLong("Seed"); Seed = &_seed;
}

void EducationLevelAspect::Start()
{
	// Set random educationlevels for everybody...
	for (Population::Iterator agentId = this->begin;
		agentId != this->end; ++agentId)
	{
		(*this)[*agentId]->EducationLevel= (int)floor(_levels * ran1(Seed));
	}
	// Gets the sum of all probas
	_sum_of_all_probas_norm = GetProbaNormalization(&_norm_of_sum);	
}

float EducationLevelAspect::GetProbaNormalization(float *norm_of_sum)
{
	int size = this->begin->GetSize();
	// Calculate n for the grid 
	int *count = new int[_levels];
	memset(count, 0, sizeof(int) * (_levels));
	EducationLevelInfo *element;
	for (Population::Iterator agentId = this->begin;
		agentId != this->end; ++agentId)
	{
		element = (*this)[*agentId];
		count[element->EducationLevel]++;
	}
	int i1, i2; 
	int dist;
	float proba;
	float sum_of_all_probas = 0;
	for (i1 = 0; i1 < _levels; i1++)
	{
		// Goes through each cell
		for (i2 = 0; i2 < _levels; i2++)
		{
			dist = IABS(i1, i2);
			proba = this->pdistrEducationalRaw(dist);
			int c1 = count[i1];
			int c2 = count[i2];
			float add = proba * c1 * c2;
			if (add > 0)
				sum_of_all_probas += add;
		}
	}
	float sum_of_all_probas_norm;
	sum_of_all_probas_norm = sum_of_all_probas / (size * size);
	*norm_of_sum = 1.0F / _levels;
	// done
	free(count);
	return sum_of_all_probas_norm;
}
EducationLevelAspect::~EducationLevelAspect()
{
	;
}

float EducationLevelAspect::pdistrEducationalRaw(int gap)
{
   float proba = exp(-gap*gap*Beta / ((_levels)*(_levels)));
   return proba;
}
float EducationLevelAspect::pdistrEducational(int gap)
{
   return(this->pdistrEducationalRaw(gap) / _sum_of_all_probas_norm * _norm_of_sum);
}

/*Simmeric distance
float EducationLevelAspect::pdistrHeterophilia(int gap)
{
   float proba;

   if (_toleranceHeterophilia > 0)
	   proba = (float) (0.5 * exp(-gap * gap / (_toleranceHeterophilia * (_levels-1) * (_levels-1))));
   else
	   proba = 0.0;
   return proba;
}
*/

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
		else if (strcmp(fields[n], "Beta")==0)
			values[n] = varValue(Beta);
		else
			values[n] = varValue(0);
	}
}
