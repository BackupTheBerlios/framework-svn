#include "variant.h"

double variantGetValue(varValue &value)
{
	switch(value.type)
	{
	case T_Double:
		return value.dValue;
	case T_Integer:
		return value.iValue;
	default:
		throw "unsupported getValue type";
	}
};

void variantToStringFile(FILE *file, varValue &value)
{
	switch(value.type)
	{
	case T_Double:
		if (value.dValue != (long) value.dValue)
            fprintf(file,"%f\t", value.dValue);
		else
			fprintf(file,"%ld\t", (long) value.dValue);
		break;
	case T_Integer:
		fprintf(file,"%ld\t", value.iValue);
		break;
	case T_String:
		// tiene que haber una forma mejor!!
		for(std::basic_string<char>::iterator it = value.sValue.begin(); it != value.sValue.end();++it) 
			putc(*it, file);
		putc('\t', file);
		break;
	default:
		throw "unsupported variantoToString type";
	}
};

