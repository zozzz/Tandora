#include <iostream>
#include "global.h"
#include "parser/Token.h"
#include "parser/TandoraTokens.h"
#include "parser/TandoraParser.h"
#include <bitset>
#include "common/Timer.h"

#define _ROOT "D:\\Works\\cpp\\Tandora\\"

using namespace std;
using namespace common;
using namespace tandora;

int main()
{
	//_WAIT_FOR_KEYP
	//std::string dummy;
	//std::getline(std::cin, dummy);

	//TandoraParser parser;
	//parser.inputFile("../unittest/tandora_src/ftest.as");

	// JavaScriptCompiler compiler;
	// compiler.optimize();
	// compiler.compile(parser);

	//stringstream str;

	//File f(_ROOT"test\\utf8.txt", File::READ);

	//cout << f.exists() << endl;
	//cout << f.readc() << endl;

	/*utf8::FileIO fs(_ROOT"test\\utf8.txt");
	utf8::uchar ch = fs.readc();

	_DUMP_UCHAR(ch);

	_DUMP_UCHAR(0xc381);
	 * */

	#define _IT_ 100
	Timer t1;

	for(int i= 0 ; i<_IT_ ; i++)
		utf8::FileIO fio(_ROOT"test\\UIComponent.as");


	t1.stop();
	cout << t1.last() << " | " << t1.last() / _IT_ << endl;


	

	/*while( ch = fio.readc() )
	{
		_DUMP_UCHAR(ch);
	}*/

	return 0;
}
