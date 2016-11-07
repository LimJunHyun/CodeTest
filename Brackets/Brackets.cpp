#include <iostream>
#include <fstream>
#include <string>
#include <queue>

using namespace std;

#define INPUT_LIMIT 10000

//#define USE_REDIRECT

struct Bracket
{
	Bracket()
	{
		value	= 0;
		left	= NULL;
		right	= NULL;
	}

	int value;
	Bracket* left;
	Bracket* right;
};

int Calc( char* input );
bool CheckSideBrackets( Bracket* Target );

void main()
{

#ifdef USE_REDIRECT
	ifstream in("./Brackets_testcase/5.input.txt");
    streambuf *cinbuf = cin.rdbuf();
    cin.rdbuf(in.rdbuf());
#endif

	char input[INPUT_LIMIT + 1];
	int test_num;

	cin >> test_num;

	for(int i = 0 ; i < test_num ; ++i )
	{
		cin >> input;
		cout << Calc( input ) << endl;
	}

#ifdef USE_REDIRECT
	std::cin.rdbuf(cinbuf);
#endif

}

int Calc( char* input )
{	
	Bracket Buffer[INPUT_LIMIT];

	queue<Bracket*> CheckList;
	
	int BracketLen = 0;
	int InputLen = strlen( input );
		
	for( int i = 0 ; i < InputLen ; ++i )
	{
		switch(input[i])
		{
		case '(':
			{
				if( ')' == input[i+1] )
				{
					Buffer[BracketLen].value = 1;
					CheckList.push(&Buffer[BracketLen]);
					++i;
				}
				else
					Buffer[BracketLen].value = -1;
			}
			break;		
		case '{':
			{
				if( '}' == input[i+1] )
				{
					Buffer[BracketLen].value = 2;
					CheckList.push(&Buffer[BracketLen]);
					++i;
				}
				else
					Buffer[BracketLen].value = -2;
			}
			break;		
		case '[':
			{
				if( ']' == input[i+1] )
				{
					Buffer[BracketLen].value = 3;
					CheckList.push(&Buffer[BracketLen]);
					++i;
				}
				else
					Buffer[BracketLen].value = -3;
			}
			break;
		case ')':
			Buffer[BracketLen].value = -5;
			break;
		case '}':
			Buffer[BracketLen].value = -7;
			break;
		case ']':
			Buffer[BracketLen].value = -11;
			break;
		}

		if( 0 < BracketLen )
		{
			Buffer[BracketLen].left = &Buffer[BracketLen - 1];
			Buffer[BracketLen - 1].right = &Buffer[BracketLen];
		}

		BracketLen++;
	}

	bool IsCalculting = false;
	int CheckCount = CheckList.size();
	
	if(CheckList.empty()) return 0;

	Bracket* Target = CheckList.front();
	CheckList.pop();
	
	while(!( NULL == Target->left && NULL == Target->right ))
	{
		if( 0 == CheckCount )
		{
			if( false == IsCalculting ) return 0;

			IsCalculting = false;
			CheckCount = CheckList.size();
		}

		if( 0 == Target->value )
		{
			Target = CheckList.front();			
			CheckList.pop();
			CheckCount--;
			continue;
		}

		while( NULL !=  Target->right && Target->right->value > 0 )
		{
			Target->value += Target->right->value;
			Target->right->value = 0;
			Target->right = Target->right->right;

			if( NULL != Target->right )
				Target->right->left = Target;

			IsCalculting |= true;
		}

		while( NULL !=  Target->left && Target->left->value > 0 )
		{
			Target->value += Target->left->value;
			Target->left->value = 0;
			Target->left = Target->left->left;

			if( NULL != Target->left )
				Target->left->right = Target;

			IsCalculting |= true;
		}

		while(CheckSideBrackets(Target))
		{
			IsCalculting |= true;
		}

		CheckList.push(Target);

		Target = CheckList.front();
		CheckList.pop();
		CheckCount--;
	}

	/*
	for( int i = 0 ; i < BracketLen ; i++ )
		cout << Buffer[i].value << " ";
	cout << endl;
	*/

	return Target->value;
}

bool CheckSideBrackets( Bracket* Target )
{
	if( NULL == Target->left ) return false;
	if( NULL == Target->right ) return false;

	if( -1 != Target->left->value &&
		-2 != Target->left->value &&
		-3 != Target->left->value ) return false;
	if( -5 != Target->right->value &&
		-7 != Target->right->value &&
		-11 != Target->right->value ) return false;

	int multiplier = 0;

	switch( Target->left->value * Target->right->value )
			{
			case 5:
				multiplier = 1;
				break;
			case 14:	
				multiplier = 2;
				break;
			case 33:
				multiplier = 3;
				break;
			default:
				return false;
				break;
			}

	Target->value *= multiplier;
	Target->right = Target->right->right;
	Target->left = Target->left->left;

	if( NULL != Target->right )
		Target->right->left = Target;

	if( NULL != Target->left )
		Target->left->right = Target;

	return true;
}