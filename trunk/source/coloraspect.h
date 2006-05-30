struct ColorInfo {
	int Color;
	};

class ColorAspect : public AspectBase
{
public:
	void Evolve();
	void InitializeAspect();
	int GetInfoSize();
	ColorInfo * operator[](key agentId);
	void virtual ShowValues(int agentId, std::vector <char *> &fields,
								 varValue *values);
};

