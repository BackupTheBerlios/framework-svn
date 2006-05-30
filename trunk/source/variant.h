#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

using std::vector;
using std::string;
#ifndef _VARTYPE
	#define _VARTYPE

enum FieldType
{
	T_Double,
	T_Integer,
	T_String
};

typedef struct varValue
{
protected:
    int		        iValue;
	double	        dValue;
	std::string     sValue;
	friend class    stringValue;
public:
	
	FieldType type;
	std::string &GetStringValue()
	{
		switch(type)
		{
		case T_String:
			return sValue;
		default:
			throw "unsupported getValue type";
		}
	};

	double GetNumericValue()
	{
		switch(type)
		{
		case T_Double:
			return dValue;
		case T_Integer:
			return iValue;
		default:
			throw "unsupported getValue type";
		}
	};
	void operator++()
	{
		iValue++;
		dValue++;
	}
	void operator--()
	{
		iValue++;
		dValue++;
	}
	void operator-=(double d)
	{
		if (type == T_Double)
			dValue-=d;
		else
			iValue-= (int) d;
	}
	void operator+=(double d)
	{
		if (type == T_Double)
			dValue+=d;
		else
			iValue+=(int) d;
	}
	void operator-=(varValue v)
	{
		double d;
		if (v.type == T_Double)
			d = v.dValue;
		else if (v.type == T_Integer)
			d = v.iValue;
		else
			throw "invalid operation on variant";
		if (type == T_Double)
			dValue-=d;
		else
			iValue-=(int) d;
	}
	void operator+=(varValue v)
	{
		double d;
		if (v.type == T_Double)
			d = v.dValue;
		else if (v.type == T_Integer)
			d = v.iValue;
		else
			throw "invalid operation on variant";
		if (type == T_Double)
			dValue+=d;
		else
			iValue+=(int) d;
	}
	void operator/=(double d)
	{
		if (type == T_Double)
			dValue/=d;
		else
			iValue/=(int) d;
	}
	void operator*=(double d)
	{
		if (type == T_Double)
			dValue*=d;
		else
			iValue*=(int) d;
	}
	bool operator <(varValue &v)
	{
		if (v.type == T_String && type == T_String)
			return sValue < v.sValue;
		if (v.type == T_String || type == T_String)
			throw "invalid string vs number comparison";
		double d1;
		if (type == T_Double)	d1 = dValue;
		else if (type == T_Integer) d1 = iValue;
		else throw "unsupported comparison";
		double d2;
		if (v.type == T_Double)	d2 = v.dValue;
		else if (v.type == T_Integer) d2 = v.iValue;
		else throw "unsupported comparison";
		return d1 < d2;
	}
	bool operator <=(varValue &v)
	{
		return *this < v || *this == v;
	}
	bool operator >=(varValue &v)
	{
		return *this > v || *this == v;
	}
	bool operator ==(varValue &v)
	{
		if (v.type == T_String && type == T_String)
			return sValue == v.sValue;
		if (v.type == T_String || type == T_String)
			throw "invalid string vs number comparison";
		double d1;
		if (type == T_Double)	d1 = dValue;
		else if (type == T_Integer) d1 = iValue;
		else throw "unsupported comparison";
		double d2;
		if (v.type == T_Double)	d2 = v.dValue;
		else if (v.type == T_Integer) d2 = v.iValue;
		else throw "unsupported comparison";
		return d1 == d2;
	}
	bool operator >(varValue &v)
	{
		if (v.type == T_String && type == T_String)
			return sValue > v.sValue;
		if (v.type == T_String || type == T_String)
			throw "invalid string vs number comparison";
		double d1;
		if (type == T_Double)	d1 = dValue;
		else if (type == T_Integer) d1 = iValue;
		else throw "unsupported comparison";
		double d2;
		if (v.type == T_Double)	d2 = v.dValue;
		else if (v.type == T_Integer) d2 = v.iValue;
		else throw "unsupported comparison";
		return d1 > d2;
	}
	varValue()
	{
	}

	varValue(FieldType t)
	{
		type = t;
	}

	varValue(double d)
	{
		type = T_Double;
		dValue = d;
	}
	varValue(int i)
	{
		type = T_Integer;
		iValue = i;
	}

	varValue(const char *s)
	{
		type = T_String;
		sValue = std::string(s);
	}

	varValue(std::string &s)
	{
		type = T_Integer;
		sValue = s;
	}

	varValue(varValue *v)
	{
		type = v->type;
		iValue = v->iValue;
		dValue = v->dValue;
		if (type == T_String)
			sValue = v->sValue;
	}
} varValue;
/*
struct intValue : public varValue
{
	int &Value;
	intValue(varValue &i) : Value(iValue), varValue(i.type)
	{
	    if (i.type != T_Integer) throw "Invalid assignment";
		iValue = ((intValue *)&i)->iValue;
	}
	intValue(intValue &i) : Value(iValue), varValue(i.type)
	{
		type = T_Integer;
		iValue = i.iValue;
	}
	intValue(int i) : Value(iValue), varValue(T_Integer)
	{
		iValue = i;
	}
};
struct stringValue : public varValue
{
	string &Value;
	stringValue(varValue &s) : Value(sValue), varValue(s.type)
	{
	    if (s.type != T_String) throw "Invalid assignment";
		sValue = s.sValue;
	}
	stringValue(stringValue &s) : Value(sValue), varValue(T_String)
	{
		sValue = s.sValue;
	}
	stringValue(char *s) : Value(sValue), varValue(T_String)
	{
		sValue = string(s);
	}
	stringValue(string &s) : Value(sValue), varValue(T_String)
	{
		sValue = s;
	}
};

struct doubleValue : public varValue
{
	double &Value;
	doubleValue(varValue &d) : Value(dValue), varValue(d.type)
	{
	    if (d.type != T_Double) throw "Invalid assignment";
		dValue = ((doubleValue *)&d)->dValue;
	}
	doubleValue(doubleValue &d) : Value(dValue), varValue(d.type)
	{
		dValue = d.dValue;
	}
	doubleValue(double d) : Value(dValue), varValue(T_Double)
	{
		dValue = d;
	}
};

double variantGetValue(varValue &value);
void variantToStringFile(FILE *file, varValue &value);
*/
#endif

