#include <string>
#include <vector>
#include "bxmlnode.h"

#ifndef _POPULATION
	#define _POPULATION

typedef unsigned int key;
using std::string;
using std::vector;
#define LINE "----------------------------------\n"

class Population
{
	public:
		Population(char *);
		int GetSize();
		~Population();
		void Initiate();
		void Iterate(int times);
		void Iterate();
		void Run();

		///////////////////////////////////////
		//class Iterator : public std::iterator<std::forward_iterator_tag, int> {
		class Iterator : public extendedIterator {
		public:
			virtual int GetSize()
			{
				int subpopulation = 0;
				int ret = 0;
				while(true)
				{
					subpopulation = GetNextPopulation(subpopulation);
					if (subpopulation == 0) break;
					ret += _population->_subPopulations[subpopulation-1]->GetSize();
				}
				return ret;
			}
			Iterator(Population* p, AspectBase *a)
			{
				_aspect = a;
				_population = p;
				int subpopulation = GetNextPopulation(0);
				_next_limit = _population->_subPopulations[subpopulation-1]->GetSize();
				_current = MAKE_AGENT_ID(subpopulation, 0);
			}
			/*Iterator(Population* p, AspectBase *a, int nil)
			{
				_aspect = a;
				_population = p;
				_current = 0;
			}*/
			Iterator(std::iterator<std::forward_iterator_tag, int> *it)
			{
				Iterator *other = (Iterator *) it;
				_population = other->_population;
				_aspect = other->_aspect;
				_current = other->_current;
				_next_limit = other->_next_limit;
			};
			virtual ~Iterator() {}

			// The assignment and relational operators are straightforward
			Iterator& operator=(const Iterator& other)
			{
				_population = other._population;
				_aspect = other._aspect;
				_current = other._current;
				_next_limit = other._next_limit;
				return(*this);
			}

			bool operator==(const Iterator& other)
			{
				return(_population == other._population
					&& _current == other._current
					&& _aspect == other._aspect);
			}

			bool operator!=(const unsigned int& other)
			{
				return(_current != other);
			}
			bool operator!=(const Iterator& other)
			{
				return(_population != other._population
					|| _current != other._current
					|| _aspect != other._aspect);
			}

			// Update my state such that I refer to the n-next element in the
			// SQueue.
			Iterator& operator+=(int value)
			{
				if (_current == 0) return(*this);

				unsigned int subpopulation = AGENT_ID_POPULATION(_current);
				unsigned int position = AGENT_ID_POSITION(_current);
				//
				while (value > 0)
				{
					if (position + value < _next_limit)
						break; // todo ok....
					else
					{
						// Jumps to next population
						subpopulation = GetNextPopulation(subpopulation);
						position = 0;
						// updates pointer
						value -= _next_limit - position;
						// checks
						if (subpopulation == 0)
						{
							_current = 0; // EOF
							return *this;
						}
						// updates limit
						_next_limit = _population->_subPopulations[subpopulation-1]->GetSize();
					}
				}
				position += value;
				_current = MAKE_AGENT_ID(subpopulation, position);
				return *this;
			}

			// Update my state such that I refer to the next element in the
			// SQueue.
			Iterator& operator++()
			{
				return (*this+=1);
			}

			/*Iterator& operator++(int)
			{
				Iterator tmp(*this);
				++(*this);
				return(tmp);
			}*/

			// Return a reference to the value in the node.  I do this instead
			// of returning by value so a caller can update the value in the
			// node directly.
			int operator*()
			{
				return(_current);
			}

			// Return the address of the value referred to.
			/*int * operator->()
			{
				return(&*(Population::Iterator)*this);
			}*/

		private:
			Population* _population;
			AspectBase *_aspect;
			unsigned int _current;
			unsigned int _next_limit;
			int GetNextPopulation(unsigned int current)
			{
				while (++current <= _population->_subPopulations.size())
					if (_population->_subPopulations[current-1]->HasAspect(_aspect)
						&& _population->_subPopulations[current-1]->GetSize() > 0)
						return current;

				return 0;
			};
		};

	//////////////////////
	// Population private members
	private:
		class SubPopulation
		{
		public:
			SubPopulation(int size, char *name);
			~SubPopulation();
			AspectBase * TryGetAspect(AspectBase *type);
			bool HasAspect(AspectBase *type);
			void AddAspect(AspectBase *asp);
			vector <AspectBase *> const & Aspects();
			int GetSize();
			string Name;
		private:
			int _size;
			vector <AspectBase *> _aspects;
		};

		// Private fields
		std::vector <Output *> _outputs;
		vector <SubPopulation *> _subPopulations;
		vector <AspectBase *> _runAspects;
		struct basicxmlnode * _rootConfig;
		int _history;
		float _deltaT;
		// Private methods
		const char *fileOnly(const char *file);
		void _iterate();
		void doOutput(bool onlyShowOutputTotals);
		void Initialize();
		void ParseFilterString(char *filterString, FilterOperation *op, char **posStart, char **posEnd);
		void CreateOutputs(void);
		void LoadSettings();
		std::string format(double span);
		std::string format2(int p);
		AspectBase * TryGetAspect(AspectBase *type);
		void LoadConfigFile(char* iniFile);
		void CreateSubPopulations(void);
		Population::SubPopulation * getSubPopulationByName(char *populationName, int &id);
		AspectBase * getAspectByName(char *aspectName);
		void CreateAspects(SubPopulation * sp, struct basicxmlnode *subpopulation);

		void callIterate();
};
#endif
