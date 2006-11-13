#ifndef EDUCASP
  #define EDUCASP

struct EducationLevelInfo {
	int EducationLevel;
	};

class EducationLevelAspect : public AspectBase
{
public:
	void Evolve();
	void InitializeAspect();
	int GetInfoSize();
	EducationLevelInfo * operator[](key agentId);
	void virtual ShowValues(int agentId, std::vector <char *> &fields,
								 varValue *values);
	~EducationLevelAspect();
	float pdistrEducational(int gap);
	/*float pdistrHomophilia(int gap);*/
	/*float pdistrHeterophilia(int gap);*/

private:
	long *Seed;
	long _seed;
	int _levels;
	/*float _toleranceHeterophilia;*/
	/*float _toleranceHomophilia;*/
	float Beta;
};

#endif


