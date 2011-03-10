/*
 * File:   parser.h
 * Author: Zozzz
 *
 * Created on 2011. február 28., 19:08
 */

#ifndef PARSER_H
#define	PARSER_H

#include "../global.h"
#include "../common/unicode/unicode.h"
#include "../common/File.h"

#ifdef __DEBUG__
#include <iostream>
#include <string>
#include <list>
#include <stdlib.h>
#include <stdio.h>
#endif

namespace parser
{
	//------------------------------------------------------------------------------
	// All action is 32 bit wide:
	//------------------------------------------------------------------------------
	enum Action
	{
		//------------------------------------------------------------------------------
		// Throw unexpected char error
		// 0000 0000 0000 0000 0000 0000 0000 0000
		//									  \__/
		//									    |
		//									 Action Type
		//------------------------------------------------------------------------------
		ERROR = 0,

		//------------------------------------------------------------------------------
		// Everything is ok
		// 0000 0000 0000 0000 0000 0000 0000 0001
		//									  \__/
		//									    |
		//									 Action Type
		//------------------------------------------------------------------------------
		CONTINUE,

		//------------------------------------------------------------------------------
		// Close token
		// Params:
		//		- token type
		//		- offset: how many chars track back or forward
		// 0000 0000 0000 0000 0000 0000 0000 0010
		//					   \_______/ \__/ \__/
		//						 |		  |     |
		//				 Token type	 Offset	 Action Type
		//
		//------------------------------------------------------------------------------
		CLOSE,

		//------------------------------------------------------------------------------
		// Test char position inside token
		// Params:
		//		- token type
		//		- position[5]: max 5 position test available (0-15)
		// 0000 0000 0000 0000 0000 0000 0000 0011
		// \_______/ \__/ \__/ \__/ \__/ \__/ \__/
		//	   |	   \____|___||___|____/    |
		//	Token type		 Positions		 Action Type
		//------------------------------------------------------------------------------
		CHAR_AT,

		//------------------------------------------------------------------------------
		// Change token type
		// Params:
		//		- token type
		// 0000 0000 0000 0000 0000 0000 0000 0100
		//						    \_______/ \__/
		//							    |       |
		//						Token type	 Action Type
		//------------------------------------------------------------------------------
		CHNG_TYPE
	};

	#define BM_ACTION 0xF

	struct Token
	{
		unsigned char type;
		unsigned int line;
		unsigned short int col;
		unsigned char* buffer;
	};

	#ifdef __DEBUG__
	class PActionGraph
	{
	public:
		typedef struct Node
		{
			char	ch;
			int		pos;

			char	begin_ch;
			int		begin_pos;

			int		action;

			Node():
				ch(0),
				pos(0),
				begin_ch(0),
				begin_pos(0),
				action(0)
			{
			}

			void writeSelfId(std::string& buff)
			{
				char* _pos = new char[sizeof(int)*8+1];
				sprintf(_pos, "%d", pos);
				char* _ch = new char[sizeof(char)*8+1];
				sprintf(_ch, "%d", ch);
				buff += "n_";
				buff += _pos;
				/*buff += "_";
				buff += _ch;*/
			}

			void writeBeginId(std::string& buff)
			{
				if( begin_ch != 0 )
				{
					char* _begin_pos = new char[sizeof(int)*8+1];
					sprintf(_begin_pos, "%d", begin_pos);
					char* _begin_ch = new char[sizeof(char)*8+1];
					sprintf(_begin_ch, "%d", begin_ch);

					buff += "n_";
					buff += _begin_pos;
					/*buff += "_";
					buff += _begin_ch;*/
				}
			}

			void writeActionStyle(std::string& buff)
			{
				switch( action & BM_ACTION )
				{
					case ERROR: 
						buff += "fillcolor=\"#e62929\""; 						
					break;
					
					case CONTINUE: 
						buff += "fillcolor=\"#c1e20d\",fontcolor=\"#333333\""; 
					break;
					
					case CLOSE: 
						buff += "fillcolor=\"#325866\""; 
					break;
					
					case CHAR_AT: 
						buff += "fillcolor=\"#47ba1a\""; 
					break;
					
					case CHNG_TYPE: 
						buff += "fillcolor=\"#dc7904\""; 
					break;
				}
			}
			
			void writeActionLabel(std::string& buff)
			{
				switch( action & BM_ACTION )
				{
					case ERROR: 
						buff += "ERROR"; 						
					break;
					
					case CONTINUE: 
						buff += "OK"; 
					break;
					
					case CLOSE: 
						buff += "10"; 
					break;
					
					case CHAR_AT: 
						buff += "1|3|5"; 
					break;
					
					case CHNG_TYPE: 
						buff += "23"; 
					break;
				}
			}
			
			void write(std::string& buff)
			{
				if( begin_ch != 0 )
				{
					writeBeginId(buff);
					buff += " -> ";

					writeSelfId(buff);
					buff += "\n";
				}
				writeSelfId(buff);
				buff += " [label=\"{";

				char* _pos = new char[sizeof(int)*8+1];
				sprintf(_pos, "%d", pos);
				buff += _pos;
				buff += "|";

				if( (unsigned)ch > 127 )
					buff += "UTF8";
				else if( ch == 10 )
					buff += "\\\\n";
				else if( ch == 13 )
					buff += "\\\\r";
				else if( ch == 32 )
					buff += "SPACE";
				else if( ch == 9 )
					buff += "\\\\t";
				else if( ch == 34 || ch == 123 || ch == 125 || ch == 60 || ch == 62 || ch == 92 )
				{
					buff += "\\";
					buff += ch;
				}
				else
					buff += ch;
				buff += "|";

				/*switch( action & BM_ACTION )
				{
					case ERROR: buff += "ERROR"; break;
					case CONTINUE: buff += "CONTINUE"; break;
					case CLOSE: buff += "CLOSE"; break;
					case CHAR_AT: buff += "CHAR_AT"; break;
					case CHNG_TYPE: buff += "CHNG_TYPE"; break;
					default: buff += "undefined"; break;
				}*/
				writeActionLabel(buff);
				
				buff += "}\",";
				
				writeActionStyle(buff);

				buff += "];\n";
			}
		} Node;

		PActionGraph(char* outFile):
			_buffer(),
			_outFile(outFile)
		{
			_buffer += "digraph G {\n";
		}

		~PActionGraph()
		{
			_nodes.clear();
			delete[] _outFile;
		}

		void writeToFile()
		{
			_buffer += "rankdir=LR; ranksep=0.5; dpi=72; nodesep=0.1;\n";
			_buffer += "node[shape=record, fontsize=10, height=0.25, width=1.8, fontname=\"monospace\", style=filled, fontcolor=\"#FFFFFF\", fixedsize=true];\n";

			_fillData();
			_buffer += "}";
			common::File f(_outFile, common::File::WRITE);
			f.write(_buffer.c_str(), _buffer.size());
			
			/*
			const char* _dot = "\"C:\\Program Files (x86)\\Graphviz2.26.3\\bin\\dot.exe\" -v";
			int cmdLen = strlen(_outFile) * 2 + 24 + strlen(_dot);
			char* cmd = new char[cmdLen+1];
			strcat(cmd, _dot);
			strcat(cmd, "-Tsvg -Kdot -o \"");
			strcat(cmd, _outFile);
			strcat(cmd, ".svg\" \"");
			strcat(cmd, _outFile);
			strcat(cmd, "\"");
			
			cmd[cmdLen] = '\0';
			
			common::File bat("create_graph.bat", common::File::WRITE);
			bat.write(cmd, cmdLen);
			
			trace(cmd);
			
			trace(system("create_graph.bat"));
			 **/
		}

		void addNode(Node* n)
		{
			_nodes.push_back(n);
		}

		void addNode(char ch, int pos, char begin_ch, int begin_pos, int action)
		{
			Node* n = new Node();
			n->ch = ch;
			n->pos = pos;

			n->begin_ch = begin_ch;
			n->begin_pos = begin_pos;

			n->action = action;

			addNode(n);
		}

	private:
		std::string _buffer;
		std::list<Node*> _nodes;
		char* _outFile;

		void _fillData()
		{
			for( std::list<Node*>::reverse_iterator iter = _nodes.rbegin() ; iter != _nodes.rend() ; ++iter )
			{
				(*iter)->write(_buffer);
			}
		}
	};
	#endif

	template<int _TokenCount, unsigned int _ActionTable[_TokenCount][128], class _Token>
	class TokenReader
	{
	public:
		typedef _Token Token;
		typedef TokenReader<_TokenCount, _ActionTable, _Token> Self;

		static Self* createFromFile(const char* fileName)
		{
			Self* ret = new Self(fileName);
			
			common::File file(fileName, common::File::READ);
			ret->_setInput(&file);
			
			return ret;
		};

		static Self* createFromString(const unsigned char* buffer)
		{
			#ifdef __DEBUG__
			print("Graph creation disabled in TokenReader::createFromString");
			#endif
			return new Self(NULL);
		};

		bool next(Token& t)
		{
			char ch;
			char pch;
			int start = _bufferPos;
			bool close = false;
			
			while( (ch = buffer[_bufferPos]) )
			{
				_action = _ActionTable[_token][ch];
				#ifdef __DEBUG__
				if( _graph != NULL )
					if( _bufferPos == start )
						_graph->addNode(ch, _bufferPos, 0, 0, _action);
					else
						_graph->addNode(ch, _bufferPos, pch, _bufferPos-1, _action);
				#endif

				switch( _action & BM_ACTION )
				{
					case CLOSE:
						_token = 0;
						close = true;
					break;
				}

				++_bufferPos;
				pch = ch;

				if( close )
					return true;
			}

			#ifdef __DEBUG__
			if( ch == 0 && _graph != NULL )
				_graph->writeToFile();
			#endif

			return false;
		}


	protected:
		unsigned char* buffer;
		int _bufferPos;
		int _token;
		int _action;

		#ifdef __DEBUG__
		PActionGraph* _graph;
		#endif

		#ifdef __DEBUG__
		TokenReader(const char* fileName):
			_bufferPos(0),
			_token(0)
		#else
		TokenReader():
			_token(0)
		#endif
		{
			#ifdef __DEBUG__
			if( fileName )
			{
				char* _gfile = new char[strlen(fileName)+3];
				memcpy(_gfile, fileName, strlen(fileName));
				strcat(_gfile+strlen(fileName), ".g");
				_gfile[strlen(fileName)+2] = '\0';
				_graph = new PActionGraph(_gfile);
			}
			#endif
		}
		
		void _setInput(common::File* file)
		{
			AssertExit(file ,!=, NULL);

			ALLOC_ARRAY(buffer, unsigned char, file->size());

			file->read(buffer, file->size());
		}
		
		void _setInput(unsigned char* buffer)
		{
		}
	};

}

#endif	/* PARSER_H */

