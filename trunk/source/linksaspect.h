#include "list.h"

#include "distanceaspect.h"

struct LinksInfo {
	int		Degree;
	List	Table;
	/* For evaluation calculations */
    unsigned char	Clusterflag;
	LinksInfo **	Predecesors;
	int				Predecesors_end;
	// Results for the net evaluation
	int				size_component;
	int             num_component;
	float			closeness;
	float			betweenness;
	float			mean_shortest;
	float			max_diameter;
	// Temporary values for calculations
	float			t_btw;
	int				t_dist;
	};

class LinksAspect : public AspectBase
{
public:
	void Evolve();
	void Start();
	void InitializeAspect();
	int GetInfoSize();
	LinksInfo * operator[](key agentId);
	void virtual ShowValues(int agentId, std::vector <char *> &fields,
								 varValue *values);
	~LinksAspect();
	void ReleaseAspect();
private:
	int   Members;
	int	  Links;
	DistanceAspect			*_distanceAspect;
	EducationLevelAspect	*_educationLevelAspect;
	SocialCircleAspect		*_socialCircleAspect;
	GeographyAspect	        *_geographyAspect;

	void insert_biedge(int i, int j);
	void insert_biedge(int i, int j, LinksInfo *iInfo,
								LinksInfo *jInfo);
	void delete_biedge(int i, int j);

	void adjlst_insert (LinksInfo *li, Edge *data);
	bool must_create_new_link(int agentId1, int agentId2);

	float Friend_Probability;
	float Random_Probability;
	float Startup_Links;
	/*float Alfa;*/
	/*float Homophilia;*/
	/*float Heterophilia;*/
	float Delete_Probability;

	float _totalByFriends;
	float _totalByRandom;

	long *Seed;
	long _seed;
	/*** Network evaluation ***/
	/* Buffers */
	int			_lastPrecSize;
	bool		bTimeStepChanged;
	LinksInfo ** fifov;
	/* Functions */
	float		clustering(LinksInfo *agentInfo);
	float		mean_shortest(LinksInfo *agentInfo);
	int			shortest_paths(LinksInfo **fifov, LinksInfo *agentInfo, float *lmax);
	void		checkTimeStepChange();
	void		betweenness(LinksInfo **fifov, LinksInfo *agentInfo);
	void		multi_eval();
};

