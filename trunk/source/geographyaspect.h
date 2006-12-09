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
	void Start();
	void InitializeAspect();
	int GetInfoSize();
	GeographyInfo * operator[](key agentId);
	void virtual ShowValues(int agentId, std::vector <char *> &fields,
								 varValue *values);
	void ReleaseAspect();
	float pdistrGeographic(float gdist);
	float GeographicDistance(float x1, float y1, float x2, float y2);
private:
	long *Seed;
	long _seed;
	float _sum_of_all_probas_norm;
	float _norm_of_sum;
	float Alfa;
	float _delta;
	float pdistrGeographicRaw(float gdist);
};

#endif

