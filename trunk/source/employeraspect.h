struct EmployerInfo
{
	int PlacesOccupied;
	vector <int> *Places;
};

class EmployerAspect : public AspectBase
{
public:
	void Evolve();
	~EmployerAspect();
	void InitializeAspect();
	int GetInfoSize();
	EmployerInfo * operator[](key agentId);
	void ApplyForJob(key companyId, key personId);
	int getFreePlace(EmployerInfo* emp);
	int PlacesCount;
	void virtual ShowValues(int agentId, std::vector <char *> &fields,
								 varValue *values);
private:

};
