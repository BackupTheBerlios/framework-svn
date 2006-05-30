#include "framework.h"
#include "marriageaspect.h"
#include "coloraspect.h"

void MarriageAspect::Evolve()
{
	ColorAspect *color = (ColorAspect *) this->TryGetAspect(typeid(ColorAspect).name());

	for (Population::Iterator agentId = this->begin;
		agentId != this->end; ++agentId)
	{
		// Averigua el color de la persona...
		// (ejemplo de acceso a atributos de otra dimensión
		//  para un mismo agente)
		if (color != NULL)
			int col = (*color)[*agentId]->Color;
	}
}
int MarriageAspect::GetInfoSize()
{
	return sizeof(MarriageInfo);
}

void MarriageAspect::InitializeAspect()
{
	for (Population::Iterator agentId = this->begin;
		agentId != this->end; ++agentId)
	{
		// Pone a todo solteros
		(*this)[*agentId]->Couple = 0;
	}
}
void MarriageAspect::ShowValues(int agentId, std::vector <char *> & fields,
								 varValue *values)
{

}
MarriageInfo * MarriageAspect::operator[](key agentId)
{
	return (MarriageInfo *) this->GetInfo(agentId);
}
