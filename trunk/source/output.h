#include <string>
#include <vector>
#include "timep.h"

#include "aspect.h"
#include "outputaggregation.h"

using std::vector;
#ifndef _OUTPUT
	#define _OUTPUT

struct FieldGroup
{
	AspectBase *				Aspect;
	OutputAggregation 			OutputAggregationManager;
	vector <char *>				FieldNames;
	vector <char *>				FieldCriteria;
	vector <FieldFilter *>		FieldFilters;
	vector <FieldOperation>		FieldOperations;
	varValue *					retValues;
};


class Output
{
public:
	Output(std::vector <Output *> &outputs_ref);
	~Output();

	FILE*	File;
	char *	Filename;
	bool	Headers;
	void *	SubPopulation;
	int		SubPopulationId;
	bool	IsAggregate;
	vector <FieldGroup *> FieldGroups;
	float	Loops;
	int		StartLoop;
	double	Timespan;
	double	lastTime;
	bool	restarteverycycle;
	bool	enabled;
	std::vector <Output *> &outputs;
	// Functions
    void	CheckFileOpen();
	void	CheckFileHeaders();
	void	CheckFileClose();
	void	Close();
private:
	bool	_wroteHeaders;
	bool	_fileOwner;
};
#endif
