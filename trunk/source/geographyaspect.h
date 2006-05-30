#ifndef GEOASP
  #define GEOASP

struct GeographyInfo {
	int X;
	int Y;
	};

class GeographyAspect : public AspectBase
{
public:
	void Evolve();
	void InitializeAspect();
	int GetInfoSize();
	GeographyInfo * operator[](key agentId);
	void virtual ShowValues(int agentId, std::vector <char *> &fields,
								 varValue *values);
	void ReleaseAspect();
private:
	long *Seed;
	long _seed;
};

#endif

