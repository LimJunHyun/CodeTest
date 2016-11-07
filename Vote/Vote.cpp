#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

//#define USE_REDIRECT

struct VoteList
{
	VoteList()
	{
		count	= 0;
		up		= NULL;
		down	= NULL;
	}

	string		name;
	int			count;
	VoteList*	up;
	VoteList*	down;
};

VoteList* HeadPos = NULL;
VoteList* TailPos = NULL;

void UpdateList( VoteList* Target );

void main()
{

#ifdef USE_REDIRECT
	ifstream in("./Vote_testcase/5.input.txt");
    streambuf *cinbuf = cin.rdbuf();
    cin.rdbuf(in.rdbuf());
#endif

	map<string, VoteList*> CandidateList;
	map<string, VoteList*>::iterator it_CandidateList;
	
	string line;
	while(getline(std::cin, line)) 
    {
		//Find Exist Name
		it_CandidateList = CandidateList.find(line);

		if( it_CandidateList == CandidateList.end() )
		{
			//Add New Person.
			VoteList* NewPerson = new VoteList;
			NewPerson->name = line;
			NewPerson->count = 1;
			CandidateList.insert( make_pair( line, NewPerson ) );

			//Set First Element.
			if( NULL == HeadPos )
			{
				HeadPos = NewPerson;
				TailPos = NewPerson;
				continue;
			}
			
			//Assign Tail Position.
			TailPos->down = NewPerson;
			NewPerson->up = TailPos;
			TailPos = NewPerson;
			
			//Sort.
			UpdateList( NewPerson );

		}
		else
		{
			//Add Vote.
			it_CandidateList->second->count++;

			//Sort.
			UpdateList( it_CandidateList->second );
		}

    }

	//Print All TopCount User.
	int Topcount = HeadPos->count;
	VoteList* OutputPos = HeadPos;

	while ( NULL != OutputPos && Topcount <= OutputPos->count )
	{
		cout << OutputPos->name << endl;

		OutputPos = OutputPos->down;
	}

	while( HeadPos != TailPos )
	{
		VoteList* RemoveList = HeadPos;
		HeadPos = HeadPos->down;
		delete RemoveList;
	}

#ifdef USE_REDIRECT
	std::cin.rdbuf(cinbuf);
#endif

}


void UpdateList( VoteList* Target )
{
	//Already Top, just exit.
	if( NULL ==  Target->up ) return;

	VoteList* NewPos = Target->up;

	while( NULL != NewPos )
	{
		if( NewPos->count < Target->count ||
			( NewPos->count == Target->count && false == lexicographical_compare( NewPos->name.begin(), NewPos->name.end(), Target->name.begin(), Target->name.end() ))
			)
		{
			NewPos = NewPos->up;
		}
		else
			break;
	}

	//No Change, exit.
	if( NewPos == Target->up ) return;
	
	if( NULL == NewPos )
	{
		
		Target->up->down = Target->down;

		if( NULL != Target->down )
			Target->down->up = Target->up;

		if( TailPos == Target )
			TailPos = Target->up;

		Target->down = HeadPos;
		Target->up = NULL;		

		HeadPos->up = Target;
		HeadPos = Target;
	}
	else
	{
		Target->up->down = Target->down;

		if( NULL != Target->down )
			Target->down->up = Target->up;

		if( TailPos == Target )
			TailPos = Target->up;

		Target->down = NewPos->down;
		Target->up = NewPos;

		NewPos->down->up = Target;
		NewPos->down = Target;

	}

}