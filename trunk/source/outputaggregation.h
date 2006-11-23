#include <string>
#include <vector>
#include <stdio.h>
#include "variant.h"

using std::vector;
using std::string;
#ifndef _OUTPUT_AG
	#define _OUTPUT_AG

enum FieldOperation
{
	Average,
	Average_positive,
	Sum,
	Min,
	Max,
	Median,
	StdDev,
	Count,
	Count_positive,
	Percentage
};



class OutputAggregation
{
    public:
        OutputAggregation();
        ~OutputAggregation();
        void Initialize (	vector <char *> *fieldNames,
							vector <FieldOperation> *fieldOperations,
                            vector <char *> *fieldCriteria);
        void ProcessValues(varValue *values);
        void Resolve(void);
        void Clear(void);
        void static BeginLine(FILE *file, float loop);
        void static BeginLine(FILE *file, float loop, unsigned int agentId);
        void static BeginHeaders(FILE *file, bool isAggregate);
        void static EndLine(FILE *file);
        void ShowValues(FILE *file);
        void ShowHeaders(FILE *file);


    private:
        vector <char *>				*_fieldNames;
        vector <FieldOperation>		*_fieldOperations;
        vector <char *>				*_fieldCriteria;
        vector <varValue>		_results;
        int							*_count;
        FILE						*_file;
        bool						_resolved;
		// Functions
        void						startResults();
        void						copyvector(vector <varValue> &results, varValue *values);
        char						*operationToText(FieldOperation operation, char *buffer);
		void						variantToStringFile(FILE *file, varValue &value);

};

#endif
