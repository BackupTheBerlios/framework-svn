#ifndef GEOASP
  #define GEOASP

struct GeographyInfo {
	float X;
	float Y;
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
	float pdistrGeographic(float gdist);
private:
	long *Seed;
	long _seed;
	float Alfa;
};

#endif

