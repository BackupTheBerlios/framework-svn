#include "outputaggregation.h"

OutputAggregation::OutputAggregation()
{

}

void OutputAggregation::Initialize (vector <char *>			*fieldNames,
									vector <FieldOperation> *fieldOperations,
									vector <char *>			*fieldCriteria,
									vector <FieldFilter *>	*fieldFilter)
{
	_fieldOperations = fieldOperations;
	_fieldCriteria = fieldCriteria;
	_fieldNames = fieldNames;
	_fieldFilter = fieldFilter;
	_resolved = false;
	_count = new int[_fieldNames->size()];
	startResults();
}
OutputAggregation::~OutputAggregation()
{
	if (_count != NULL)
	{
		delete _count;
	}
}

void OutputAggregation::copyvector(vector <varValue> &results, varValue *values)
{
	results.clear();
	for(vector <varValue>::size_type n = 0; n < this->_fieldNames->size(); n++)
		results.push_back(values[n]);


}
void OutputAggregation::ProcessValues(varValue *values)
{
	if (_fieldOperations->size() == 0)
	{	// No tiene que hacer ningún aggregate... copia valores
		_results.clear();
        for(vector <varValue>::size_type n = 0; n < this->_fieldNames->size(); n++)
            if ((*_fieldFilter)[n] == NULL || EvaluateFilter((*_fieldFilter)[n], _fieldNames, values))
                _results.push_back(values[n]);
            else
                _results.push_back(new varValue(""));
		return;
	}
	// Procesa para el aggregate...
	for(vector <double>::size_type n = 0; n < _fieldOperations->size(); n++)
	{
	    if ((*_fieldFilter)[n] == NULL || EvaluateFilter((*_fieldFilter)[n], _fieldNames, values))
        {
            switch((*_fieldOperations)[n])
            {
                case Sum:
                case Average:
                    _results[n] += values[n];
                    break;
                case Average_positive:
                    if (values[n].GetNumericValue() >= 0)
                    {
                        _results[n] += values[n];
                    }
                    else
                        _count[n]--; // se asegura de que los demas no sean contabilizados
                    break;
                case Min:
                    if (values[n] < _results[n] || _count[n] == 0)
                        _results[n] = values[n];
                    break;
                case Max:
                    if (values[n] > _results[n] || _count[n] == 0)
                        _results[n] = values[n];
                    break;
                case Count:
                    if ((*_fieldCriteria)[n] == NULL)
                        ++_results[n];
                    else
                    {
                        if (values[n].type == T_String)
                        {
                            varValue sv = values[n];
                            if (sv.GetStringValue() == string((*_fieldCriteria)[n]))
                            {
                                ++_results[n];
                            }
                        }
                        else
                        {
                            if (values[n].GetNumericValue() == atof((*_fieldCriteria)[n]))
                                ++_results[n];
                        }
                    }
                    break;
                case Count_positive:
                     if (values[n].GetNumericValue() >= 0)
                            ++_results[n];
                     break;
                case Percentage:
                    if (values[n].type == T_String)
                    {
                        varValue sv = values[n];
                        if (sv.GetStringValue() == string((*_fieldCriteria)[n]))
                        {
                            ++_results[n];
                        }
                        else
                            _count[n]--;
                    }
                    else
                        if (values[n].GetNumericValue() == atof((*_fieldCriteria)[n]))
                            ++_results[n];
                        else
                            _count[n]--;
                    break;
                    /*
                    Faltan implementar:
                    Median,
                    StdDev
                    */
                default:
                    throw("Unsupported operation.");
            }
            _count[n]++;
        }
	}
}

bool OutputAggregation::EvaluateFilter(FieldFilter* _fieldFilter, vector <char *> *_fieldNames, varValue *values)
{
    // Busca el field con ese valor
    for (int n=0; n<(int)_fieldNames->size(); n++)
        if (strcmp((*_fieldNames)[n], _fieldFilter->Field) == 0)
        {   // Usa el comparador
            switch(_fieldFilter->Operator)
            {
                case 	Equal:
                    if (values[n].type == T_String)
					{
						varValue sv = values[n];
						return (sv.GetStringValue() == string(_fieldFilter->retValues));
					}
					else
						return (values[n].GetNumericValue() == atof(_fieldFilter->retValues));
                    break;
                case 	NoEqual:
                   if (values[n].type == T_String)
					{
						varValue sv = values[n];
						return (sv.GetStringValue() != string(_fieldFilter->retValues));
					}
					else
						return (values[n].GetNumericValue() != atof(_fieldFilter->retValues));
                    break;
                case 	GreaterThan:
                   if (values[n].type == T_String)
					{
						varValue sv = values[n];
						return (sv.GetStringValue() > string(_fieldFilter->retValues));
					}
					else
						return (values[n].GetNumericValue() > atof(_fieldFilter->retValues));
                    break;
                case 	LowerThan:
                   if (values[n].type == T_String)
					{
						varValue sv = values[n];
						return (sv.GetStringValue() < string(_fieldFilter->retValues));
					}
					else
						return (values[n].GetNumericValue() < atof(_fieldFilter->retValues));
                    break;
                case 	EqualOrGreaterThan:
                   if (values[n].type == T_String)
					{
						varValue sv = values[n];
						return (sv.GetStringValue() >= string(_fieldFilter->retValues));
					}
					else
						return (values[n].GetNumericValue() >= atof(_fieldFilter->retValues));
                    break;
                case 	EqualOrLowerThan:
                   if (values[n].type == T_String)
					{
						varValue sv = values[n];
						return (sv.GetStringValue() <= string(_fieldFilter->retValues));
					}
					else
						return (values[n].GetNumericValue() <= atof(_fieldFilter->retValues));
                    break;
            }
        }
    return false;
}
void OutputAggregation::BeginHeaders(FILE *file, bool isAggregate)
{
	fprintf(file,"Loop\t");
	if (isAggregate == false)
		fprintf(file,"AgentId\t");
}

void OutputAggregation::ShowHeaders(FILE *file)
{
	char buffer[40];
	for(vector <char *>::size_type n = 0; n < _fieldNames->size(); n++)
	{
		fprintf(file,"%s", (*_fieldNames)[n]);
		if (_fieldOperations->size() > 0) // isAggregate
		{
			fprintf(file,"_%s", operationToText((*_fieldOperations)[n],buffer));
		}
		fprintf(file,"\t");
	}
}
char *OutputAggregation::operationToText(FieldOperation operation, char *buffer)
{
	switch(operation)
	{
	case Average:
		strcpy(buffer, "mean");
		break;
	case Average_positive:
		strcpy(buffer, "meanp");
		break;
	case Sum:
		strcpy(buffer, "sum");
		break;
	case Min:
		strcpy(buffer, "min");
		break;
	case Max:
		strcpy(buffer, "max");
		break;
	case Median:
		strcpy(buffer, "med");
		break;
	case StdDev:
		strcpy(buffer, "stdv");
		break;
	case Count:
		strcpy(buffer, "count");
		break;
	case Count_positive:
		strcpy(buffer, "cntp");
		break;
	case Percentage:
		strcpy(buffer, "pin");
		break;
	default:
		throw("Operation missing in outputAggregation::OperationToText");
	}
	return buffer;
}
void OutputAggregation::Resolve(void)
{
	if (_fieldOperations->size() > 0)
	{
		// Procesa para el aggregate...
		for(vector <double>::size_type n = 0; n < _fieldOperations->size(); n++)
		{
			switch((*_fieldOperations)[n])
			{
				case Sum:
				case Min:
				case Max:
				case Count:
					break;
				case Count_positive:
                    break;
				case Average:
				case Average_positive:
				case Percentage:
					_results[n] /= _count[n];
					break;
					/*
					Faltan implementar:
					Median,
					StdDev,
					 */
				default:
					throw("Unsupported operation.");
			}
		}
	}
}
void OutputAggregation::Clear(void)
{
	startResults();
}
void OutputAggregation::startResults()
{
	// Initialize results...
	_results.clear();
	for(vector <double>::size_type n = 0; n < _fieldNames->size(); n++)
	{
		_results.push_back(varValue(0.0));
		_count[n]=0;
	}
}

void OutputAggregation::BeginLine(FILE *file, float loop)
{
	BeginLine(file, loop, 0);
}
void OutputAggregation::BeginLine(FILE *file, float loop, unsigned int agentId)
{
	fprintf(file,"%.2f\t", loop);
	if (agentId != 0)
		fprintf(file,"%#.8X\t", agentId);
}

void OutputAggregation::EndLine(FILE *file)
{
	fprintf(file,"\n");
}

void OutputAggregation::ShowValues(FILE *file)
{
	if (_resolved == false)
		Resolve();
	for(vector <char *>::size_type n = 0; n < _fieldNames->size(); n++)
	{
		variantToStringFile(file, _results[n]);
	}
	this->Clear();
}
void OutputAggregation::variantToStringFile(FILE *file, varValue &value)
{
	switch(value.type)
	{
	case T_Double:
		{
		double d = value.GetNumericValue();
		if (d != (long) d)
            fprintf(file,"%f\t", d);
		else
			fprintf(file,"%ld\t", (long) d);
		break;
		}
	case T_Integer:
		fprintf(file,"%ld\t", value.GetNumericValue());
		break;
	case T_String:
		{
		// tiene que haber una forma mejor!!
		string s = value.GetStringValue();
		for(std::basic_string<char>::iterator it = s.begin(); it != s.end();++it)
			putc(*it, file);
		putc('\t', file);
		break;
		}
	default:
		throw "unsupported variantoToString type";
	}
};

