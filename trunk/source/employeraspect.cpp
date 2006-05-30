#include "framework.h"
#include "employeraspect.h"
#include "coloraspect.h"


void EmployerAspect::Evolve()
{

}
int EmployerAspect::GetInfoSize()
{
	return sizeof(EmployerInfo);
}

void EmployerAspect::InitializeAspect()
{
	// Gets number of places
	PlacesCount = GetConfigInt("Size");
	// Start empty companies...
	for (Population::Iterator agentId = this->begin;
		agentId != this->end; ++agentId)
	{
		// Starts the 'places' array
		// for every company
		EmployerInfo *company = (*this)[*agentId];
		company->Places = new vector <int>;
		for (int n = 0; n < PlacesCount; n++)
			company->Places->push_back(0);

		company->PlacesOccupied = 0;
	}
}

void EmployerAspect::ApplyForJob(key companyId, key personId)
{
	// Compara colores y emplea al tipo
//	LaborAspect *labor = (LaborAspect *) this->GetAspect(typeid(LaborAspect).name());
	bool bMatch;
	ColorAspect *color = (ColorAspect *) this->TryGetAspect(typeid(ColorAspect).name());
	if (color != NULL)
		bMatch = (*color)[companyId]->Color == (*color)[personId]->Color;
	else
		bMatch = true;

	if (bMatch)
	{
		int n = getFreePlace((*this)[companyId]);
		if (n > -1)
		{
			// Lo acepta
//			(*labor)[personId]->companyId = companyId;
			EmployerInfo *company = (*this)[companyId];
			(*(company->Places))[n] = personId;
			company->PlacesOccupied++;
		}
	}
}
int EmployerAspect::getFreePlace(EmployerInfo* emp)
{
	for(vector <int>::size_type n = 0;
				n < emp->Places->size(); n++)
		if ((*emp->Places)[n] == 0)
			return (int) n;
	return -1;
}

void EmployerAspect::ShowValues(int agentId, std::vector <char *> & fields,
								 varValue *values)
{
	for (std::vector <char *>::size_type n = 0; n < fields.size();n++)
	{
		if (strcmp(fields[n], "EmployeesCount")==0)
			values[n] = varValue((*this)[agentId]->PlacesOccupied);
		else
			values[n] = varValue(0);
	}
}
EmployerAspect::~EmployerAspect()
{
	for (Population::Iterator agentId = this->begin;
		agentId != this->end; ++agentId)
		delete (*this)[*agentId];
}

EmployerInfo * EmployerAspect::operator[](key agentId)
{	// Get info pointers
	return (EmployerInfo *) this->GetInfo(agentId);
}
