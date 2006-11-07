// COMPILA CON /GR
#include <string>
#include <iostream>
#include <typeinfo>
#include "timep.h"

#include "framework.h"

#define size_pops vector<Population::SubPopulation>::size_type

Population::Population(char* iniFile)
{
	std::cout << "Starting " << fileOnly(iniFile) << "...\n";
	std::cout << LINE;
	// Gets the config file information
	LoadConfigFile(iniFile);
	// LoadSettings
	LoadSettings();
	// Creates the populations and its aspects
	CreateSubPopulations();
	// Initializes aspects
	Initialize();
	// Create output objects
	CreateOutputs();

}
void Population::LoadSettings()
{
	// Carga DeltaT
	struct basicxmlnode *globals = getchildxmlnode(_rootConfig, "Globals");
	_deltaT = (float) getchildxmlnodedoublevalue(globals, "DeltaT", 1);
}
const char *Population::fileOnly(const char *file)
{
	const char *pos = strrchr(file, '\\');
	if (pos == NULL)
		pos = strrchr(file, '/');
	if (pos == NULL)
		return file;
	else
		return pos + 1;
}
void Population::Initialize()
{
	// Guarda en una lista los aspectos para
	// ejecutar más rápido
	for (size_pops n = 0; n < _subPopulations.size(); n++)
		for (vector<AspectBase *>::size_type a = 0;
				a < _subPopulations[n]->Aspects().size(); a++)
			_runAspects.push_back(_subPopulations[n]->Aspects()[a]);
	// Distribuye esa info entre los aspectos
	for (vector<AspectBase *>::size_type a = 0; a < _runAspects.size(); a++)
	{	// Le carga un bloque para cada población
		_runAspects[a]->LoadAspects(_runAspects);
	}
	// Les reserva bloques de memoria en cada
	// aspecto a las diferentes poblaciones
	for (vector<AspectBase *>::size_type a = 0; a < _runAspects.size(); a++)
	{	// Le carga un bloque para cada población
		for (size_pops n = 0; n < _subPopulations.size(); n++)
			if (_subPopulations[n]->HasAspect(_runAspects[a]))
				_runAspects[a]->AppendBlock(_subPopulations[n]->GetSize());
			else
				_runAspects[a]->AppendBlock(0);
	}
	// Corre la inicialización
	std::cout << LINE;
	for (vector<AspectBase *>::size_type a = 0; a < _runAspects.size(); a++)
	{
		_runAspects[a]->Initialize();
		std::cout << "Aspect " << _runAspects[a]->Name << " initialized...\n";
	}
}
int Population::GetSize()
{
	int ret = 0;
	for(vector <SubPopulation *>::const_iterator iter = _subPopulations.begin();
		iter != _subPopulations.end(); ++iter)
	{
		//SubPopulation *p = (*iter);
		//ret += p.GetSize();
		ret += (*iter)->GetSize();
	}
	return ret;
}

void Population::LoadConfigFile(char* iniFile)
{
	// Loads the data file population information
	_rootConfig = NULL;
	FILE * fp = fopen(iniFile, "rt");
	if (!fp) { throw("open failed"); }
	_rootConfig = readbasicxmlnode(fp);
	fclose(fp);
	if (!_rootConfig) { throw("read failed"); }
}
void Population::CreateOutputs()
{
	struct basicxmlnode *outputs;
	struct basicxmlnode *outputinfo;
	outputs = getchildxmlnode(_rootConfig, "Outputs");
	// Recorre cada output
	int i = 0;
	outputinfo = getchildxmlnode(outputs, i++);
	Output *output;
	while (outputinfo != NULL)
	{
		// Creates the output object
		output = new Output(_outputs);
		// Gets all the information
		char *filename = trygetchildxmlnodestringvalue(outputinfo, "Filename");
		bool headers = getchildxmlnodeboolvalue(outputinfo, "Headers", true);
		char *populationName = trygetchildxmlnodestringvalue(outputinfo, "Population");
		char *typeName = trygetchildxmlnodestringvalue(outputinfo, "Type");
		int loops = getchildxmlnodeintvalue(outputinfo, "Loops", 0xFFFFFF);
		int timespan = getchildxmlnodeintvalue(outputinfo, "Timespan", 0xFFFFFF);
		bool restarteverycycle = getchildxmlnodeboolvalue(outputinfo, "RestartEveryCycle", false);
		// Checks pointers and names
		if (filename == NULL)
			throw ("Output config error: filename for output block was not found.");
		if (populationName == NULL)
			throw ("Output config error: population for output block was not found.");
		if (typeName == NULL)
			throw ("Output config error: type for output block was not found.");
		if (strcmp(typeName,"Aggregate") != 0 &&
			strcmp(typeName,"List") != 0)
			throw ("Output config error: type value is invalid. Only Aggregate and List are valid values.");
		bool typeIsAggregate = (strcmp(typeName,"Aggregate") == 0);
		int id;
		Population::SubPopulation *subp = this->getSubPopulationByName(populationName, id);
		if (subp == NULL)
			throw ("Output config error: the population " + string(populationName) + " count not be found.");
		output->SubPopulation = (void *) subp;
		output->SubPopulationId = id;
		output->File = NULL;
		// Fills the output object
		//time_t t; time(&t);
		double t = get_time();
		output->Filename = filename;
		output->Headers = headers;
		output->IsAggregate = typeIsAggregate;
		output->Loops = loops;
		output->Timespan= timespan;
		output->lastTime = t; // initialize time
		output->restarteverycycle = restarteverycycle;

		// Get fields for the output
		struct basicxmlnode *fields;
		struct basicxmlnode *aspectfield;
		fields = getchildxmlnode(outputinfo, "Fields");
		// Recorre los fields
		int af = 0;
		aspectfield = getchildxmlnode(fields, af++);
		while (aspectfield != NULL)
		{
			// Gets the aspect
			char *aspectName = getxmlnodetag(aspectfield);
			AspectBase *aspect = this->getAspectByName(aspectName);
			//if (aspect == NULL)
			//	throw ("Output config error: aspect " + string(aspectName) + " could not be found.");
			if (aspect != NULL)
			{
				FieldGroup *group = new FieldGroup();
				group->Aspect = aspect;
				// Gets all its children...
				int cf = 0;
				struct basicxmlnode *fieldinfo;
				fieldinfo = getchildxmlnode(aspectfield, cf++);
				char *fieldname;
				while (fieldinfo != NULL)
				{
					// Appends the information
					// Gets the field name
					fieldname = getxmlnodetext(fieldinfo);
					group->FieldNames.push_back(fieldname);
					// Get the operation, if any
					FieldOperation op;
					char *operationName = getchildxmlnodeattributestringvalue(fieldinfo, "Operation");
					char *value = NULL;
					if (output->IsAggregate == true)
					{
						if (operationName == NULL || strcmp(operationName, "Average")==0)
							op = Average;
						else if (strcmp(operationName, "Sum")==0)
							op = Sum;
						else if (strcmp(operationName, "Min")==0)
							op = Min;
						else if (strcmp(operationName, "Max")==0)
							op = Max;
						else if (strcmp(operationName, "Count")==0)
							op = Count;
						else if (strcmp(operationName, "Median")==0)
							op = Median;
						else if (strcmp(operationName, "StdDev")==0)
							op = StdDev;
						else if (strcmp(operationName, "Percentage")==0)
						{
							op = Percentage;
							char *valueString = getchildxmlnodeattributestringvalue(fieldinfo, "Value");
							if (valueString == 0)
								throw ("Output config error: the percetange operation must indicate a Value parameter.");
							value = valueString;
						}
						else
							throw ("Output config error: the operation " + string(operationName) + " was not recognized.");

						group->FieldOperations.push_back(op);
					} // endif is aggregate
					group->FieldCriteria.push_back(value);
					// loops
					fieldinfo = getchildxmlnode(aspectfield, cf++);
				}
				//
				group->retValues = new varValue[group->FieldNames.size()];
				//
				group->OutputAggregationManager.Initialize(&group->FieldNames,
													&group->FieldOperations,
													&group->FieldCriteria);
				// appends the field vector
				output->FieldGroups.push_back(group);
			}
			// loops
			aspectfield = getchildxmlnode(fields, af++);
		}
		_outputs.push_back(output);
		//output->outputs = _outputs;
		// done...
		outputinfo = getchildxmlnode(outputs, i++);
	}

}
Population::SubPopulation * Population::getSubPopulationByName(char *populationName, int &id)
{
	std::string name = populationName;
	for(vector <Population::SubPopulation *>::size_type i = 0; i < _subPopulations.size(); i++)
	{
		if (_subPopulations[i]->Name == name)
		{
			id = (int) i + 1;
			return _subPopulations[i];
		}
	}
	return NULL;
}

AspectBase * Population::getAspectByName(char *aspectName)
{
	std::string name = aspectName;
	for(vector <AspectBase *>::size_type i = 0; i < _runAspects.size(); i++)
	{
		if (_runAspects[i]->Name == name)
			return _runAspects[i];
	}
	return NULL;
}

void Population::CreateSubPopulations()
{
	struct basicxmlnode *populations;
	struct basicxmlnode *subpopulationinfo;
	populations = getchildxmlnode(_rootConfig, "Populations");
	// Recorre cada población
	int i = 0;
	subpopulationinfo = getchildxmlnode(populations, i++);
	while (subpopulationinfo != NULL)
	{
		// Create the subpopulation
		int size = getchildxmlnodeintvalue(subpopulationinfo, "Size");
		SubPopulation * sp = new SubPopulation(size, getxmlnodetag(subpopulationinfo));
		std::cout << "Population " << sp->Name << " created (N=" << size << ")...\n";
		// Create or add existing aspects
		CreateAspects(sp, subpopulationinfo);
		// Appends to population
		_subPopulations.push_back(sp);
		// loops...
		subpopulationinfo = getchildxmlnode(populations, i++);
	}
	// Crea los iterators
	for(vector <Population::SubPopulation *>::size_type i = 0; i < _subPopulations.size(); i++)
		for(vector <AspectBase *>::size_type a = 0; a < _subPopulations[i]->Aspects().size(); a++)
			if (_subPopulations[i]->Aspects()[a]->begin == NULL)
			{
				_subPopulations[i]->Aspects()[a]->begin = new
							Population::Iterator(this, _subPopulations[i]->Aspects()[a]);
				/*_subPopulations[i]->Aspects()[a]->end = new
							Population::Iterator(this, _subPopulations[i]->Aspects()[a], NULL);*/
			}
}

void Population::CreateAspects(SubPopulation * sp, struct basicxmlnode *subpopulation)
{
	struct basicxmlnode *aspects;
	struct basicxmlnode *aspect;
	aspects = getchildxmlnode(subpopulation, "Aspects");
	int n = 0;
	while ((aspect = getchildxmlnode(aspects, n++)) != NULL)
	{
		string aspectName = string(getxmlnodetag(aspect));
		// Gets the aspect type
		AspectBase *aspectTypeObject = Activator::GetAspectByName(aspectName);
		// Looks for the aspect type at the population
		AspectBase *asp = this->TryGetAspect(aspectTypeObject);
		if (asp != NULL)
		{
			// Register existing aspect
			sp->AddAspect(asp);
			delete aspectTypeObject;
		}
		else
		{
			// Keeps new aspect instance
			sp->AddAspect(aspectTypeObject);
			aspectTypeObject->Name = getxmlnodetag(aspect);
			aspectTypeObject->begin = NULL;
			aspectTypeObject->end = 0;
			aspectTypeObject->DT = _deltaT;
			// Appends to the aspect the XML node info pointer
			basicxmlnode *aspectsInfo = getchildxmlnode(this->_rootConfig, "Aspects");
			if (aspectsInfo != NULL)
			{
				basicxmlnode *aspectInfo = getchildxmlnode(aspectsInfo, getxmlnodetag(aspect));
				aspectTypeObject->Config = aspectInfo;
			}

		}
	}
}

AspectBase * Population::TryGetAspect(AspectBase *type)
{
	// Recorre las subpopulations buscando
	// alguna que le provea de ese aspect
	AspectBase * ret;
	for (size_pops n = 0; n < _subPopulations.size(); n++)
	{
		ret = _subPopulations[n]->TryGetAspect(type);
		if (ret != NULL)
			return ret;
	}
	return NULL;
}

Population::~Population()
{
	// runaspects
	for (vector<AspectBase *>::size_type n = 0; n < _runAspects.size(); n++)
	{
		string a = _runAspects[n]->Name;
		_runAspects[n]->ReleaseAspect();
	}
	// free aspects
	for (vector<AspectBase *>::size_type n = 0; n < _runAspects.size(); n++)
	{
		string a = _runAspects[n]->Name;
			delete _runAspects[n];
	}
	_runAspects.clear();
	// free subpopulations
	for (vector<SubPopulation *>::size_type n = 0;
				n < _subPopulations.size(); n++)
		delete _subPopulations[n];
	_subPopulations.clear();
	// free outputs
	for (vector <Output *>::size_type o = 0; o < _outputs.size(); o++)
		_outputs[o]->Close();
	for (vector <Output *>::size_type o = 0; o < _outputs.size(); o++)
		delete _outputs[o];
	// free config file data
	if (_rootConfig != NULL)
		deletebasicxmlnode(_rootConfig);
}


void Population::Iterate(int times)
{
	_history = 0;
	float pc;
	struct basicxmlnode *run = getchildxmlnode(_rootConfig, "Run");
	int interval = getchildxmlnodeintvalue(run, "StatusUpdateStep");
	double last = -interval;
	//time_t start_time;
	//time_t current_time;
	//time_t span;
	//time_t est;
	double start_time;
	double current_time;
	double span;
	double est;
	double avg;
	//time(&start_time);
	start_time = get_time();
	for (int n = 0; n < times; n++)
	{
		pc = ((float)n) / times * 100;
		if (last + interval <= pc)
		{
			//time(&current_time);
			current_time = get_time();
			span = current_time - start_time;
			if (n > 0)
				avg = (double) span / n;
			else
				avg = 0;
			est = (double) (times * avg);
			avg = floor(avg * 10000) / 10000;
			std::cout << "(" << format2((int)pc) << "% of " << times << ") - "
				<< "Elapsed: " << format(span) << " - "
				<< "Estimated: " << format(est) << " - "
				<< "Secs/loop: " << avg << "\n";
			last = pc;
		}

		_iterate();
		//callIterate();
	}
	std::cout << "(100% of " << times << ")\n";
}

std::string Population::format(double dspan)
{
	int span = (int) dspan;
	char buf[50];
	sprintf(buf,"%02i:%02i:%02i", (int)(span / 3600),
					((int)((span % 3600)/60)),
					(int)(span % 60));
	return buf;
}
std::string Population::format2(int p)
{
	char buf[50];
	sprintf(buf,"%2i", p);
	return buf;
}
void Population::Iterate()
{
	_history = 0;
	this->_iterate();
}
void Population::_iterate()
{
	for (vector<AspectBase *>::size_type a = 0; a < _runAspects.size(); a++)
	{
		_history++;
		_runAspects[a]->Evolve();
		doOutput();
	}
}
void Population::doOutput()
{
	// Llama a cada output interno
	for (vector<Output *>::size_type n = 0;
				n < _outputs.size(); n++)
	{
		// Va población por población iterando por
		// sus agentes y llamando a los aspects
		// que corresponda...
		// 1. Cheque si le toca...
		Output *out = _outputs[n];
		//time_t t; time(&t);
		double t = get_time();
		if (_history % out->Loops == 0 ||  (t - out->lastTime) > out->Timespan)
		{
			// Le toca...
			out->lastTime = t;
			int size = ((Population::SubPopulation *) out->SubPopulation)->GetSize();
			// Se fija si tiene que abrir el archivo
			out->CheckFileOpen();
			// Write headers if it overwrites everytime
			out->CheckFileHeaders();
			// Hace el for de 1 a size de esa población
			for (int i = 0; i < size; i++)
			{
				key agentId = MAKE_AGENT_ID(out->SubPopulationId, i);
				if (out->IsAggregate == false)
					OutputAggregation::BeginLine(out->File, this->_history, agentId);

				FieldGroup *fg;
				// Se mueve por los fieldGroups
				for (vector <FieldGroup *>::size_type g = 0;
					g < out->FieldGroups.size(); g++)
				{
					fg = out->FieldGroups[g];
					// Le pide los valores de los fields al aspect
					// para ese agentid
					fg->Aspect->ShowValues(agentId, fg->FieldNames, fg->retValues);
					// Keeps values
					fg->OutputAggregationManager.ProcessValues(fg->retValues);
					// Si no hace aggregate, las muestra
					if (out->IsAggregate == false)
						fg->OutputAggregationManager.ShowValues(out->File);
				}
				if (out->IsAggregate == false)
					OutputAggregation::EndLine(out->File);

			}
			if (out->IsAggregate == true)
			{
				OutputAggregation::BeginLine(out->File, this->_history);
				for (vector <FieldGroup *>::size_type g = 0;
								g < out->FieldGroups.size(); g++)
				{
					// Los muestra...
					FieldGroup *fg = out->FieldGroups[g];
					fg->OutputAggregationManager.ShowValues(out->File);
				}
				OutputAggregation::EndLine(out->File);
			}
			// Check close
			out->CheckFileClose();
		}
	}
}
void Population::Run()
{
	struct basicxmlnode *run = getchildxmlnode(_rootConfig, "Run");
	int times = getchildxmlnodeintvalue(run, "Loops", 1);
	std::cout << LINE;
    std::cout << "Running from empty net... 1 of " << times << " loops...\n";
    Iterate(times);
	std::cout << LINE;
	std::cout << "Finished.\n";

}


Population::SubPopulation::~SubPopulation()
{

}

Population::SubPopulation::SubPopulation(int size, char* name)
{
	_size = size;
	Name = string(name);
}


int Population::SubPopulation::GetSize()
{
	return _size;
}

void Population::SubPopulation::AddAspect(AspectBase *aspect)
{
	_aspects.push_back(aspect);
	aspect->ReferenceCount++;
}

bool Population::SubPopulation::HasAspect(AspectBase *type)
{
	return (NULL != TryGetAspect(type));
}


AspectBase * Population::SubPopulation::TryGetAspect(AspectBase *type)
{
	string typeName= typeid(*type).name();
	for (vector<AspectBase *>::size_type n = 0;
				n < this->_aspects.size(); n++)
	{
		if (typeName == typeid(*_aspects[n]).name())
			return _aspects[n];
	}
	return NULL;
}

vector <AspectBase *> const & Population::SubPopulation::Aspects()
{
	return _aspects;
}
