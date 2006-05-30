#include "framework.h"
#include "linksaspect.h"
#include "distributions.h"

/** Continues definition of linksAspect **/
// m_degree: OK
// clustering: OK

float LinksAspect::clustering(LinksInfo *agentInfo)
{
   	/*This routine calculates clustering coefficients for all nodes
	For degree 0 and 1 a clustering value of -1 is returned
	This of order O(n*k^2) wher k is the mean degree.
	this is also O(n*e) where e is the number of edges
	*/
	ListElmt   *e1,*e2;
	LinksInfo * neighborInfo;
	int k,kount;
	
	k=agentInfo->Degree;
	kount=0;
	if (k>1) 
	{
		// Clear flags
		for (Population::Iterator iter = this->begin;
				iter != this->end; ++iter)
			(*this)[*iter]->Clusterflag = 0;
		// Mark neighbors
		for(e1 = agentInfo->Table.head; e1 != NULL; e1 = e1->next) 
			(*this)[e1->data->v2]->Clusterflag = 1;
		// Walk neighbors
		for(e1 = agentInfo->Table.head ;e1!=NULL;e1 = e1->next) 
		{
			neighborInfo = (*this)[e1->data->v2];
			for(e2=neighborInfo->Table.head;e2!=NULL;
						e2=e2->next) 
			{
				if ((*this)[e2->data->v2]->Clusterflag)
					kount++;
				/*there is a link between neighbors of node*/
			}
		}
		return (float)kount/(k*(k-1));
	}
	else 
		return -1.0;	
}

void LinksAspect::multi_eval()
{
    /* Solve values for:
    
	mean_shortest[j]=l mean shortest path (1/closeness)
    closeness[j]=closeness 1/l 
    betweenness[j]=betweenness
	size_component[j]=size of the component
	max_diameter[j]=l max diameter
	
    */

	int kmax;

	if (fifov == NULL)
		fifov = new LinksInfo *[Members];

	kmax=0;
	// Gets the max k and restart values
	LinksInfo * item;
	LinksInfo * possibleNeighborInfo;
	for (Population::Iterator iter = this->begin;
			iter != this->end; ++iter)
	{
		item = (*this)[*iter];
		if (item->Degree > kmax) 
			kmax = item->Degree;
		item->t_btw = 0.0;
		item->closeness = 0.0;
		item->betweenness = 0.0;
		item->mean_shortest = 0.0;
		item->size_component = 0;
		item->max_diameter = 0;
	}
	// Allocs for predecesor buffers for each node 
	// of size kMax+1
	if (_lastPrecSize < kmax)
	{
		// Update size...
		_lastPrecSize = kmax + 10;
		// Resize buffers
		for (Population::Iterator iter = this->begin;
			iter != this->end; ++iter)
		{
			item = (*this)[*iter];
			if (item->Predecesors != NULL)
				delete(item->Predecesors);
			item->Predecesors = new LinksInfo *[_lastPrecSize];
		}
		// Done...
	}
    
	LinksInfo *agentInfo;
	for (Population::Iterator iter = this->begin;
			iter != this->end; ++iter)
	{
		agentInfo = (*this)[*iter];

		if (agentInfo->Degree > 0) 
		{   
			/*if connected*/
			agentInfo->size_component = shortest_paths(fifov,agentInfo,&(agentInfo->max_diameter));
			/* If component size is greater than 2*/
			if (agentInfo->size_component > 2) 
				betweenness(fifov,agentInfo);
			for (Population::Iterator possibleNeighbor = this->begin;
				possibleNeighbor != this->end; ++possibleNeighbor)
			{	/*loop over probable neighbors*/
				possibleNeighborInfo= (*this)[*possibleNeighbor];
				if (possibleNeighborInfo->t_dist < Members) 
				{	/*only if belongs to the same component (true neighbor)*/
					possibleNeighborInfo->mean_shortest += (float) possibleNeighborInfo->t_dist;
					/*closeness*/
					if (possibleNeighborInfo->t_dist) 
						possibleNeighborInfo->closeness += (float) 1.0/(float)possibleNeighborInfo->t_dist;	
					possibleNeighborInfo->betweenness += possibleNeighborInfo->t_btw;
					/*closeness only within the main component*/
				}
			}
		}
		else { /*isolate*/
			agentInfo->size_component = 1;
			agentInfo->mean_shortest = -1.0; /*no lenght defined*/
			agentInfo->betweenness = -1.0; /*no betweeness*/
		}
	}
	
	/*normalization of distance & closeness*/
	for (Population::Iterator iter = this->begin;
			iter != this->end; ++iter)
	{
		agentInfo = (*this)[*iter];
		if(agentInfo->Degree > 0) 
		{
			agentInfo->mean_shortest /= (float) (agentInfo->size_component - 1.0); /*mean length normalized over cluster size -1*/
			agentInfo->closeness /= (Members-1);  /*closeness normalized over the entire network -1*/
		}
	}
	return;
}	
/*This routine calculates the contribution to betweenness centrality 
of the geodesics to node_i using
the breadth first search algorithm shortest_paths
This is order O(n)*/
void LinksAspect::betweenness(LinksInfo **fifov, LinksInfo *agentInfo)
{
	int i,j;
	LinksInfo *node;
	int sizec = agentInfo->size_component;
	for(i = 0; i < sizec; i++) 
		fifov[i]->t_btw = 1.0; /*only for cluster members*/
	for(i = sizec - 1; i >= 0; i--) 
	{
		node = fifov[i];
		for(j=0;j < node->Predecesors_end;j++) 
			node->Predecesors[j]->t_btw = (float)node->t_btw / node->Predecesors_end;
	}
	for(i=1; i <= sizec; i++) {
		fifov[i]->t_btw--; /*substract the endpoints*/
		fifov[i]->t_btw /= (sizec-1)*(sizec-2); /*normalize*/
	}
	agentInfo->t_btw = 0;
	return;
}


/*This routine calculates the shortest paths between node_i and all the 
other nodes in the same component (for other components a distance = nodes is assigned) 
using a simple breadth first search. 
It reads the structure of graph both as a neighbor table *ntable
and as a graph stucture (not yet implemented)
an array of preceding nodes is returned in **spath and the distances
are either assigned to the graph structure or returned in the *dist
vector. 
On success returns the size of component (and length of fifov)
A precedence table is also retrieved
This sobroutine is order O(edges)
*/


int LinksAspect::shortest_paths(LinksInfo **fifov, LinksInfo *agentInfo, float *lmax)
{
	ListElmt   *e1;
	int   fifo_ini,fifo_end;
	
	LinksInfo *item;
	for (Population::Iterator iter = this->begin;
			iter != this->end; ++iter)
	{
		item = (*this)[*iter];
		item->t_dist = Members;	
		item->Predecesors_end=0;
	}

	fifo_ini=-1; 
	fifo_end=0;
	fifov[0]=agentInfo; //node_i;
	agentInfo->t_dist = 0;
	
	LinksInfo *kInfo;
	LinksInfo *neighborInfol;
	while(fifo_ini++<fifo_end) 
	{
		kInfo=fifov[fifo_ini];
		for(e1 = kInfo->Table.head; e1 != NULL;
			e1 = e1->next) 
		{
			neighborInfol = (*this)[e1->data->v2]; /*j-th neighbor of node k*/
			if(neighborInfol->t_dist == Members) 
			{  
				neighborInfol->t_dist = kInfo->t_dist + 1; /* assign a distance to node l*/
				neighborInfol->Predecesors[neighborInfol->Predecesors_end++] = kInfo; /*make k a predecesor of l*/
				fifov[++fifo_end]=neighborInfol; /*put l on fifo*/
			}
			else if (neighborInfol->t_dist == kInfo->t_dist+1) { /*we were here! and we have an alternative path*/
				neighborInfol->Predecesors[neighborInfol->Predecesors_end++] = kInfo; /* another predecesor for l!*/
				/*l is in the fifo already*/
			}	 	
			else {
				/*do nothing*/
			}
		}
		/*neighbors of k completed*/	
	}
	(*lmax) = (float)kInfo->t_dist; /*returns max distance*/
	return(fifo_end);
}
