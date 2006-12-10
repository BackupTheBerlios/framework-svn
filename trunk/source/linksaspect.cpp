#include "framework.h"
#include "linksaspect.h"
#include "distributions.h"


int LinksAspect::GetInfoSize()
{
	return sizeof(LinksInfo);
}
void LinksAspect::InitializeAspect()
{
	_initialized = true;
	// Evaluation buffers
	_lastPrecSize = -1;
	fifov = NULL;

	// Load parameters
	_seed = GetConfigLong("Seed"); Seed = &_seed;
	Friend_Probability = GetConfigFloat("Friend_Probability");
	/*Alfa = GetConfigFloat("Alfa");*/
	/*Homophilia = GetConfigFloat("Homophilia");*/
	/*Heterophilia = GetConfigFloat("Heterophilia");*/
        Startup_Links = GetConfigFloat("Startup_Links");
	Random_Probability = GetConfigFloat("Random_Probability");
	Delete_Probability = GetConfigFloat("Delete_Probability");

	// Get aspects
	_distanceAspect = (DistanceAspect *) this->GetAspect(typeid(DistanceAspect).name());
	// Optional...
	_educationLevelAspect = (EducationLevelAspect *) this->TryGetAspect(typeid(EducationLevelAspect).name());
	_socialCircleAspect = (SocialCircleAspect *) this->TryGetAspect(typeid(SocialCircleAspect).name());
	_geographyAspect = (GeographyAspect *) this->TryGetAspect(typeid(GeographyAspect).name());

	// Initialize the link elements...
	Members = this->begin->GetSize();
	LinksInfo *element;
	for (Population::Iterator agentId = this->begin;
		agentId != this->end; ++agentId)
	{
		 element = (*this)[*agentId];
		 element->Degree = 0;
		 element->Predecesors = NULL;
		 list_init(&element->Table);
	}
}

void LinksAspect::Evolve()
{
  float DT = this->DT;

  bTimeStepChanged = true;
  //former links_step(AdjLst *alst, int **social, int **geo, float *par, int *dim, long *Seed)
  ListElmt  *element;
  ListElmt  *prev;
  int n, m;

  /*All probabilities are probability rates. Hence we multiply these P by the coarse
  grain of time evolution DT*/
  LinksInfo *agentLinkInfo;
  LinksInfo *friendLinkInfo;
  LinksInfo *friendOfFriendLinkInfo;
  LinksInfo *randomLinkInfo;
  int friendId;
  int randomId;
  int friendOfFriendId;
  int deleteId;
  int agentId;

  for (Population::Iterator agentIditer = this->begin;
						agentIditer != this->end; ++agentIditer)
  {
	agentId = *agentIditer;
    /**********************************************************************************/
    /* Create contacts from friends of friends or random                              */
    /**********************************************************************************/
    agentLinkInfo = (*this)[agentId];

	if (agentLinkInfo->Table.size != agentLinkInfo->Degree)
		throw ("Invalid data at agentLinkInfo");

	/* Explore one friend of friend with probability Friend_Probability * DT */
	if (agentLinkInfo->Degree > 0 && ran1(Seed) < Friend_Probability * DT)
	{
		/* Randomly chosen friend */
		m = (int)floor(agentLinkInfo->Degree * ran1(Seed));
		/* Gets the friend information */
		friendId = get_nth_table_item(&agentLinkInfo->Table, m);
		friendLinkInfo = (*this)[friendId];
		/* Get the friend info */
		if (friendLinkInfo->Degree > 1)
		{
		   friendOfFriendId = agentId;
		   while(friendOfFriendId == agentId)
		   {
			  n = (int)floor(friendLinkInfo->Degree * ran1(Seed));
			  friendOfFriendId = get_nth_table_item(&friendLinkInfo->Table, n);
		   }
		   friendOfFriendLinkInfo = (*this)[friendOfFriendId];
		   /*Note that even when we are excluding transitivity returns: l can be equal to n,
		   we are not excluding the possibility of transitivity returns to another freind of n.
		   There would be no problem with this, since insert biedge can deal with this*/
		   int tertiusId = friendOfFriendId;
		   if (must_create_new_link(agentId, tertiusId)){
		       {
		   		insert_biedge(agentId, tertiusId, agentLinkInfo, friendOfFriendLinkInfo);
		   		_totalByFriends++;
		       }

		  }
		}
	}
	/* Explore one contact at random with probability Random_Probability * DT */
	if (ran1(Seed) < Random_Probability * DT)
	{
	  randomId = agentId;
	  while (randomId == agentId)
	  {
		m = (int)floor(Members * ran1(Seed));
		Population::Iterator iter = this->begin;
		iter += m; // fetches to the selected guy
		randomId = *iter;
	  }
	  /* In this case we want a different m != m */
	  randomLinkInfo = (*this)[randomId];
	  if (must_create_new_link(agentId, randomId)) {
		insert_biedge(agentId, randomId, agentLinkInfo, randomLinkInfo);
        _totalByRandom++;
	  }
	}
    /**********************************************************************************/
    /* Delete contacts with probability Delete_Probabilily * DT This runs for all links*/
    /**********************************************************************************/
	int r=0;
	element = agentLinkInfo->Table.head;
	prev = NULL;
	while(element != NULL)
	{
	  if (ran1(Seed) < Delete_Probability * DT)
	  {
		deleteId = element->data->v2;
		/* Link is always removed. It is also possible to weight this probability using distance */
		delete_biedge(agentId, deleteId);


		if (prev != NULL)
			element = prev->next;
		else
			element = agentLinkInfo->Table.head;
	  }
	  else
	  {
		prev = element;
		element = element->next;
	  }
	  r++;
	} // end of the while
  } // end of the for... agents
}

void LinksAspect::Start()
{
  int nLinks = (int) (Startup_Links * this->begin->GetSize());

  _totalByFriends = 0;
  _totalByRandom = 0;

  LinksInfo *agentLinkInfo;
  LinksInfo *randomLinkInfo;
  int randomId;
  int agentId;

  int linksCreated = 0;

  while (linksCreated < nLinks)
  {
      for (Population::Iterator agentIditer = this->begin;
                            agentIditer != this->end; ++agentIditer)
      {
        agentId = *agentIditer;
        agentLinkInfo = (*this)[agentId];

        /* Find a contact */
        randomId = agentId;
        while (randomId == agentId)
        {
            int m = (int)floor(Members * ran1(Seed));
            Population::Iterator iter = this->begin;
            iter += m; // fetches to the selected guy
            randomId = *iter;
        }
        /* In this case we want a different m != m */
        randomLinkInfo = (*this)[randomId];
        if (must_create_new_link(agentId, randomId)) {
            insert_biedge(agentId, randomId, agentLinkInfo, randomLinkInfo);
            linksCreated++;
        }
        if (linksCreated >= nLinks)
            break;
      } // end of the for... agents
	} // end of the while
}


 void LinksAspect::insert_biedge(int i, int j)
{
	insert_biedge(i, j, (*this)[i], (*this)[j]);
}
void LinksAspect::insert_biedge(int i, int j, LinksInfo *iInfo,
								LinksInfo *jInfo)
{
   if (i == j) {

	return; // cancel (target = source)
   }

   Edge *edge1,*edge2;
   edge1 = (Edge *) malloc (sizeof (Edge));
   //edge1->v1 = i;
   edge1->v2 = j;
   edge1->weight = 1.0;
   adjlst_insert(iInfo, edge1);
   edge2 = (Edge *) malloc (sizeof (Edge));
   //edge2->v1 = j;
   edge2->v2 = i;
   edge2->weight = 1.0;
   adjlst_insert(jInfo, edge2);
}

void LinksAspect::adjlst_insert (LinksInfo *li, Edge *data)
{
  int j;
  ListElmt *element;
  // Checks the request
  //i = ((Edge *)data)->v1;
  j = ((Edge *)data)->v2;

  for (element = li->Table.head; element != NULL; element = element->next)
	if (j == element->data->v2)
	{

		return; // cancel (target already exists)
	}


  // Adds the link
  list_ins_next(&li->Table, li->Table.tail, data);

  Links++;
  li->Degree++;
}
void LinksAspect::delete_biedge(int i, int j)
{
  LinksInfo *iInfo = (*this)[i];
  LinksInfo *jInfo = (*this)[j];

  ListElmt *element, *prev;

  if (i == j) return;
  prev = NULL;
  for (element = iInfo->Table.head; element != NULL;
		element = element->next)
  {
    if ( j == element->data->v2)
	{
	  list_rem_next(&iInfo->Table, prev);
	  Links--;
	  iInfo->Degree--;
	  break;
	}
    prev = element;
   }
   prev = NULL;
   for (element = jInfo->Table.head; element != NULL;
	   element = element->next)
   {
	 if ( i == element->data->v2)
	 {
	   list_rem_next (&jInfo->Table, prev);

	   Links--;
	   jInfo->Degree--;
	   break;
	 }
     prev = element;
   }
}

bool LinksAspect::must_create_new_link(int agentId1, int agentId2)
{
	float geographicDistance;
	int educationDistance;
	/*int circleDistance;*/
	geographicDistance = _distanceAspect->GeographicDistance(agentId1, agentId2);
	educationDistance = _distanceAspect->EducationDistance(agentId1, agentId2);
	/*circleDistance = _distanceAspect->CircleDistance(agentId1, agentId2);*/

	float p1,p2,proba;
    /*Same circle*/
    /*if (circleDistance == 0)
	{*/
	if (_educationLevelAspect != NULL)
		p1 = (float) _educationLevelAspect->pdistrEducational(educationDistance);
	else
		p1 = 1.0;
	p2 = (float) _geographyAspect->pdistrGeographic(geographicDistance);
	/* Warning: we assume that total probability is the normalized sum of all probabilities,
	but this can be modified as a generalized sum (e. g. Minkowsi metrics)*/
    //proba = (float) ((p1 + p2) / (2.0));
    proba = p2;
	/*Second Normalization*/
	if(ran1(Seed) < proba)
		return true;
   return false;
}

void LinksAspect::ReleaseAspect()
{
	if (this->_initialized == false)
		return;

	for (Population::Iterator agentId = this->begin;
		agentId != this->end; ++agentId)
	{
		 LinksInfo *element = (*this)[*agentId];
		 list_destroy(&element->Table);
		 if (element->Predecesors != NULL)
			delete(element->Predecesors);
	}
	// Buffers
	if (fifov != NULL)
		delete (fifov);
}

LinksInfo * LinksAspect::operator[](key agentId)
{
	return (LinksInfo *) this->GetInfo(agentId);
}

LinksAspect::~LinksAspect()
{

}
void LinksAspect::ShowValues(int agentId, std::vector <char *> & fields,
								 varValue *values)
{
	LinksInfo *agentInfo = (*this)[agentId];
	for (std::vector <char *>::size_type n = 0; n < fields.size();n++)
	{
		if (strcmp(fields[n], "LinksDistance") ==0)
		{
			if (agentInfo->Degree == 0)
			{
				values[n] = varValue(0);
			}
			else
			{
				float totalDistance = 0;
				for (ListElmt *element = agentInfo->Table.head; element != NULL; element = element->next)
				{
				    //GeographyInfo *geoMyAgent_Info = (*_geographyAspect)[agentId1];
				    //GeographyInfo *geoTargetAgent_Info = (*_geographyAspect)[element->data->v2];
				    totalDistance += _distanceAspect->GeographicDistance(agentId, element->data->v2);
				}
				values[n] = varValue(totalDistance / agentInfo->Degree);
			}
		}
		else if (strcmp(fields[n], "Links")		 ==0 ||
		    strcmp(fields[n], "LinksUnique") ==0)
		{
			if (agentInfo->Degree == 0)
			{
				values[n] = varValue('\0');
			}
			else
			{
				bool unique = (strcmp(fields[n], "LinksUnique")==0);
				// string data type
				char *buffer;
				buffer = new char[agentInfo->Degree * 11 + 1];
				char *ptr = buffer;
				for (ListElmt *element = agentInfo->Table.head; element != NULL; element = element->next)
				{
					if (!unique || agentId < element->data->v2)
					{
						sprintf(ptr,"%#.8X,", element->data->v2);
						ptr+=11;
					}
				}
				if (ptr != buffer)ptr--;
				*ptr  ='\0';
				values[n] = varValue(buffer);
				delete(buffer);
			}
		}
		else
		{
			double retval;
			if (strcmp(fields[n], "Degree")==0)
				retval = agentInfo->Degree;
			else if (strcmp(fields[n], "Friend_Probability")==0)
				retval = Friend_Probability;
			else if (strcmp(fields[n], "Random_Probability")==0)
				retval = Random_Probability;
			else if (strcmp(fields[n], "Delete_Probability")==0)
				retval = Delete_Probability;
			else if (strcmp(fields[n], "Clustering")==0)
				retval = clustering(agentInfo);
			else if (strcmp(fields[n], "MeanShortest")==0)
			{
				checkTimeStepChange();
				retval = agentInfo->mean_shortest;
			}
			else if (strcmp(fields[n], "Closeness")==0)
			{
				checkTimeStepChange();
				retval = agentInfo->closeness;
			}
			else if (strcmp(fields[n], "Betweenness")==0)
			{
				checkTimeStepChange();
				retval = agentInfo->betweenness;
			}
			else if (strcmp(fields[n], "SizeComponent")==0)
			{
				checkTimeStepChange();
				retval = agentInfo->size_component;
			}
			else if (strcmp(fields[n], "MaxDiameter")==0)
			{
				checkTimeStepChange();
				retval = agentInfo->max_diameter;
			}
			else if (strcmp(fields[n], "FriendsRate")==0)
			{
				retval = (float) (_totalByFriends / (_totalByFriends + _totalByRandom));
			}
			else
				retval = 0;
			// Pushes the value...
			values[n] = varValue(retval);
		}
	}
}
void LinksAspect::checkTimeStepChange()
{
	if (bTimeStepChanged)
	{
		multi_eval();
		bTimeStepChanged = false;
	}
}
