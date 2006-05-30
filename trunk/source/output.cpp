#include "output.h"

Output::Output(std::vector <Output *> &outputs_ref) : outputs (outputs_ref)
{

}

Output::~Output()
{
	for (vector <FieldGroup *>::size_type g = 0;
		g < FieldGroups.size(); g++)
	{
		//delete FieldGroups[g]->retValues;
		delete FieldGroups[g];
	}
}

void Output::CheckFileOpen()
{
	// Is opened?
	if (this->File != NULL)
		return;

	// Is opened by another output? (shares the open handle)
	for (std::vector <Output *>::size_type o = 0; o < outputs.size(); o++)
		if (strcmp(this->Filename, outputs[o]->Filename) == 0
			&& outputs[o]->File != NULL)
		{
			// gets a 'reference'
			this->_fileOwner = false;
			this->File = outputs[o]->File;
			// done 
			return;
		}
	// Gets the file...
	this->File = fopen(this->Filename, "w");
	if (this->File == NULL)
		throw "Could not open file: " + string(this->Filename);
	this->_fileOwner = true;
	this->_wroteHeaders = false;
}

void Output::CheckFileHeaders()
{
	if (this->_wroteHeaders == false && this->Headers)
	{
		// Write headers	
		this->_wroteHeaders = true;
		OutputAggregation::BeginHeaders(this->File, this->IsAggregate);
		for (vector <FieldGroup *>::size_type g = 0;
						g < this->FieldGroups.size(); g++)
		{
			// Los muestra...
			FieldGroup *fg = this->FieldGroups[g];
			fg->OutputAggregationManager.ShowHeaders(this->File);
		}
		OutputAggregation::EndLine(this->File);
	}
}
void Output::CheckFileClose()
{
	if (this->restarteverycycle)
		Close();
}
void Output::Close()
{
	if (this->File != NULL && this->Filename != NULL && this->_fileOwner)
	{
		fclose(this->File);
		this->File = NULL;
	}
}
