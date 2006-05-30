struct MarriageInfo
{
	int Couple;
};

class MarriageAspect : public AspectBase
{
public:
	void Evolve();
	void InitializeAspect();
	int GetInfoSize();
	MarriageInfo * operator[](key agentId);
	void virtual ShowValues(int agentId, std::vector <char *> &fields,
								 varValue *values);
};
