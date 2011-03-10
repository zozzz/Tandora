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
#include "ITokenReader.h"

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
		//			      \_______/ \__/   || \__/
		//			   		 |		  |	   ||   |
		//              Token type  Offset /\ Action Type
		//					 Increment line	 Skip token
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
		CHNG_TYPE,

		//------------------------------------------------------------------------------
		// Increment line number
		// 0000 0000 0000 0000 0000 0000 0000 0101
		//									  \__/
		//									    |
		//									 Action Type
		//------------------------------------------------------------------------------
		INC_LINE
	};

	#define BM_ACTION 0xF



	#ifdef __DEBUG__
	class PActionGraph
	{
	public:
		typedef struct Node
		{
			char ch;
			int pos;
			unsigned int uid;
			unsigned int begin_uid;
			int action;

			Node():
				ch(0),
				pos(0),
				begin_uid(0),
				action(0)
			{
			}

			void writeSelfId(std::string& buff)
			{
				char* _uid = new char[sizeof(int)*8+1];
				sprintf(_uid, "%d", uid);
				buff += "n_";
				buff += _uid;
			}

			void writeBeginId(std::string& buff)
			{
				if( begin_uid != 0 )
				{
					char* _begin_pos = new char[sizeof(int)*8+1];
					sprintf(_begin_pos, "%d", begin_uid);

					buff += "n_";
					buff += _begin_pos;
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
				char* _type;
				char* _type2;
				char* _offset;

				switch( action & BM_ACTION )
				{
					case ERROR:
						buff += "ERROR";
					break;

					case CONTINUE:
						buff += "OK";
					break;

					case CLOSE:
						_type = new char[sizeof(int)*8+1];
						sprintf(_type, "%d", (action >> 12) & 0xFF);

						_offset = new char[sizeof(int)*8+1];
						sprintf(_offset, "%d", (action >> 8) & BM_ACTION);

						if( action & 0x10 )
							buff += "S|";

						if( action & 0x20 )
							buff += "I|";

						buff += _offset;
						buff += "|";
						buff += _type;
					break;

					case CHAR_AT:
						buff += "1|3|5";
					break;

					case CHNG_TYPE:
						_type = new char[sizeof(int)*8+1];
						sprintf(_type, "%d", (action >> 4));
						buff += _type;
					break;
				}
			}

			void write(std::string& buff)
			{
				if( begin_uid != 0 )
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
			_buffer += "rankdir=LR; ranksep=0.5; dpi=80; nodesep=0.1;\n";
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

		void addNode(unsigned int uid, char ch, int pos, unsigned int begin_uid, int action)
		{
			Node* n = new Node();
			n->ch = ch;
			n->pos = pos;
			n->uid = uid;
			n->begin_uid = begin_uid;
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
			//for( std::list<Node*>::iterator iter = _nodes.begin() ; iter != _nodes.end() ; ++iter )
			{
				(*iter)->write(_buffer);
			}
		}
	};
	#endif

	template<int _TokenCount, unsigned int _ActionTable[_TokenCount][128]>
	class TokenReader: public ITokenReader
	{
	public:
		typedef TokenReader<_TokenCount, _ActionTable> Self;

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

		virtual bool next(Token& t)
		{
			char ch;
			int start = _bufferPos;
			unsigned int action;

			while( (ch = buffer[_bufferPos]) )
			{
				_action = _ActionTable[_token][ch];
				#ifdef __DEBUG__
				if( _graph != NULL )
				{
					_graph->addNode(_guid, ch, _bufferPos, (_guid > 0 && start != _bufferPos ? _guid-1 : 0), _action);
					++_guid;
				}
				#endif

				action = _action & BM_ACTION;

				if( action == CLOSE )
				{
					unsigned char offset = (_action >> 8) & BM_ACTION;

					if( _action & 0x20 )
					{
						++_line;
						_col = 0;
					}

					if( _action & 0x10 )
					{
						_token = 0;
						++_bufferPos;
						continue;
					}
					else
					{
						_bufferPos -= offset - 1;
						_col -= offset - 1;
						_token = (_action >> 12);
						goto AfterWhile;
					}
				}
				else if( action == CHNG_TYPE )
				{
					_token = (_action >> 4);
				}
				else if( action == ERROR )
				{
					#ifndef __DEBUG__
					ex_throw(ParserError::UnexpectedChar, line, col);
					#endif
				}
				else if( action == CHAR_AT )
				{
					unsigned char cpos;
					unsigned char pos = _bufferPos-start;
					for( unsigned char i=4 ; i<24 ; i += 4 )
					{
						cpos = (_action >> i);
						if( cpos == 0 )
							break;

						if( cpos != pos )
						{
							_token = (_action >> 24);
							break;
						}
					}
				}
				else if( action == INC_LINE )
				{
					++_line;
					_col = 0;
				}

				++_bufferPos;

			}

			AfterWhile:

			#ifdef __DEBUG__
			if( ch == 0 && _graph != NULL )
				_graph->writeToFile();
			#endif

			t.type = _token;
			_token = 0;

			trace(_bufferLength);
			trace(_bufferPos);
			return _bufferLength-1 > _bufferPos;
		}


	protected:
		unsigned char* buffer;
		int _bufferLength;
		int _bufferPos;
		int _token;
		int _action;
		int _line;
		int _col;

		#ifdef __DEBUG__
		PActionGraph* _graph;
		unsigned int _guid;
		#endif

		#ifdef __DEBUG__
		TokenReader(const char* fileName):
			_bufferPos(0),
			_token(0),
			_guid(0),
			_line(0),
			_col(0)
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

			_bufferLength = file->size();
			ALLOC_ARRAY(buffer, unsigned char, _bufferLength + 1);
			file->read(buffer, _bufferLength);
			buffer[_bufferLength] = '\0';
		}

		void _setInput(unsigned char* buffer)
		{
		}
	};

}

#endif	/* PARSER_H */

