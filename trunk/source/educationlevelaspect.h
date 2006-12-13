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
	float pdistrEducationalRaw(int gap);
	void Start();
	/*float pdistrHomophilia(int gap);*/
	/*float pdistrHeterophilia(int gap);*/

private:
	long *Seed;
	long _seed;
	int _levels;
	float _sum_of_all_probas_norm;
	float _norm_of_sum;
	/*float _toleranceHeterophilia;*/
	/*float _toleranceHomophilia;*/
	float GetProbaNormalization(float *norm_of_sum);
	float Beta;
};

#endif


