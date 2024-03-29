#include "stdafx.h"

namespace nope::dts::parser
{
	Token::Token(TokenType t, std::string_view val) :
		type(t),
		value(val),
		child()
	{
	}

	bool Token::operator==(Token const & that) const
	{
		return type == that.type && value == that.value;
	}

	bool Token::operator!=(Token const & that) const
	{
		return !(*this == that);
	}

	bool Token::isTerminal() const
	{
		switch (type)
		{
		case TokenType::END_OF_FILE:
		case TokenType::ID:
		case TokenType::BLANK:
		case TokenType::LINE_COMMENT:
		case TokenType::BLOCK_COMMENT:
		case TokenType::KW_CLASS:
		case TokenType::KW_INTERFACE:
		case TokenType::KW_CONST:
		case TokenType::KW_ENUM:
		case TokenType::KW_EXPORT:
		case TokenType::KW_EXTENDS:
		case TokenType::KW_FUNCTION:
		case TokenType::KW_CONSTRUCTOR:
		case TokenType::KW_IMPORT:
		case TokenType::KW_IN:
		case TokenType::KW_TYPEOF:
		case TokenType::KW_KEYOF:
		case TokenType::KW_VAR:
		case TokenType::KW_IMPLEMENTS:
		case TokenType::KW_VISIBILITY:
		case TokenType::KW_STATIC:
		case TokenType::KW_READONLY:
		case TokenType::KW_AS:
		case TokenType::KW_IS:
		case TokenType::KW_DECLARE:
		case TokenType::KW_FROM:
		case TokenType::KW_MODULE:
		case TokenType::KW_REQUIRE:
		case TokenType::P_ARROW:
		case TokenType::P_SPREAD:
		case TokenType::P_COLON:
		case TokenType::P_SEMICOLON:
		case TokenType::P_NEWLINE:
		case TokenType::P_DOT:
		case TokenType::P_COMMA:
		case TokenType::P_SLASH:
		case TokenType::P_ANTISLASH:
		case TokenType::P_QUESTION:
		case TokenType::P_STAR:
		case TokenType::P_OPEN_PAR:
		case TokenType::P_CLOSE_PAR:
		case TokenType::P_OPEN_BRACE:
		case TokenType::P_CLOSE_BRACE:
		case TokenType::P_OPEN_BRACKET:
		case TokenType::P_CLOSE_BRACKET:
		case TokenType::P_EQUAL:
		case TokenType::P_VERTICAL_BAR:
		case TokenType::P_GREATER_THAN:
		case TokenType::P_LESS_THAN:
		case TokenType::P_AMPERSAND:
		case TokenType::STRING_LITERAL:
		case TokenType::NUMBER:
			return true;
		default:
			return false;
		}
	}

	bool Token::operator<(Token const & that) const
	{
		if (type < that.type)
		{
			return true;
		}
		if (type == that.type)
		{
			return value < that.value;
		}
		return false;
	}

	Token & Token::operator<<(Token &&children)
	{
		std::cout << children.type << " => " << children.value << '\n';
		child.push_back(std::move(children));

		return *this;
	}

	Token const & Token::operator[](std::size_t i) const
	{
		return child[i];
	}

	Token & Token::operator[](std::size_t i)
	{
		return child[i];
	}

	std::size_t Token::size() const
	{
		return child.size();
	}

	Token const & Token::last() const
	{
		return child.back();
	}

	Token & Token::last()
	{
		return child.back();
	}

	bool Token::isKeyword() const
	{
		switch (this->type)
		{
		case TokenType::KW_CLASS:
		case TokenType::KW_INTERFACE:
		case TokenType::KW_CONST:
		case TokenType::KW_ENUM:
		case TokenType::KW_EXPORT:
		case TokenType::KW_EXTENDS:
		case TokenType::KW_FUNCTION:
		case TokenType::KW_IMPORT:
		case TokenType::KW_IN:
		case TokenType::KW_TYPEOF:
		case TokenType::KW_TYPE:
		case TokenType::KW_KEYOF:
		case TokenType::KW_VAR:
		case TokenType::KW_IMPLEMENTS:
		case TokenType::KW_VISIBILITY:
		case TokenType::KW_STATIC:
		case TokenType::KW_READONLY:
		case TokenType::KW_AS:
		case TokenType::KW_IS:
		case TokenType::KW_DECLARE:
		case TokenType::KW_FROM:
		case TokenType::KW_MODULE:
		case TokenType::KW_REQUIRE:
		case TokenType::KW_CONSTRUCTOR:
			return true;
		default:
			return false;
		}
	}

	bool Token::isReserved() const
	{
		switch (this->type)
		{
		case TokenType::KW_CLASS:
		case TokenType::KW_CONST:
		case TokenType::KW_ENUM:
		case TokenType::KW_EXPORT:
		case TokenType::KW_EXTENDS:
		case TokenType::KW_FUNCTION:
		case TokenType::KW_IMPORT:
		case TokenType::KW_IN:
		case TokenType::KW_TYPEOF:
		case TokenType::KW_VAR:
			return true;
		default:
			return false;
		}
	}

	std::string Token::json() const
	{
		std::stringstream ss;

		ss << "{\"type\":\"" << this->type << "\", ";
		
		if (this->isTerminal())
		{
			ss << "\"value\":\"" << this->value << "\"}";
		}
		else
		{
			ss << "\"child\":[";

			for (std::size_t i = 0; i < child.size(); ++i)
			{
				if (i != 0)
				{
					ss << ", ";
				}
				ss << child[i].json();
			}
			ss << "]}";
		}
		return ss.str();
	}

	std::string Token::code() const
	{

		if (this->isTerminal())
		{
			return std::string(this->value);
		}

		std::stringstream ss;

		for (std::size_t i = 0; i < child.size(); ++i)
		{
			if (i != 0)
			{
				ss << ' ';
			}
			ss << child[i].code();
		}

		return ss.str();
	}

	std::string Token::xml() const
	{
		std::stringstream ss;

		ss << '<' << this->type;

		if (this->isTerminal())
		{
			 ss << " value=\"" << this->value << "\"/>";
		}
		else
		{
			ss << '>';
			for (auto const &c : this->child)
			{
				ss << c.xml();
			}
			ss << "</" << this->type << '>';
		}

		return ss.str();
	}

	bool operator<(TokenType l, TokenType r)
	{
		return static_cast<int>(l) < static_cast<int>(r);
	}

	std::ostream & operator<<(std::ostream & os, TokenType t)
	{
		std::string s;

		switch (t)
		{
		case TokenType::UNKNOWN:
			s = "UNKNOWN";
			break;
		case TokenType::END_OF_FILE:
			s = "EOF";
			break;
		case TokenType::ID:
			s = "ID";
			break;
		case TokenType::BLANK:
			s = "BLANK";
			break;
		case TokenType::LINE_COMMENT:
			s = "LINE_COMMENT";
			break;
		case TokenType::BLOCK_COMMENT:
			s = "BLOCK_COMMENT";
			break;
		case TokenType::KW_CLASS:
			s = "KW_CLASS";
			break;
		case TokenType::KW_INTERFACE:
			s = "KW_INTERFACE";
			break;
		case TokenType::KW_CONST:
			s = "KW_CONST";
			break;
		case TokenType::KW_ENUM:
			s = "KW_ENUM";
			break;
		case TokenType::KW_EXPORT:
			s = "KW_EXPORT";
			break;
		case TokenType::KW_EXTENDS:
			s = "KW_EXTENDS";
			break;
		case TokenType::KW_FUNCTION:
			s = "KW_FUNCTION";
			break;
		case TokenType::KW_CONSTRUCTOR:
			s = "KW_CONSTRUCTOR";
			break;
		case TokenType::KW_IMPORT:
			s = "KW_IMPORT";
			break;
		case TokenType::KW_IN:
			s = "KW_IN";
			break;
		case TokenType::KW_TYPEOF:
			s = "KW_TYPEOF";
			break;
		case TokenType::KW_TYPE:
			s = "KW_TYPE";
			break;
		case TokenType::KW_KEYOF:
			s = "KW_KEYOF";
			break;
		case TokenType::KW_VAR:
			s = "KW_VAR";
			break;
		case TokenType::KW_IMPLEMENTS:
			s = "KW_IMPLEMENTS";
			break;
		case TokenType::KW_VISIBILITY:
			s = "KW_VISIBILITY";
			break;
		case TokenType::KW_STATIC:
			s = "KW_STATIC";
			break;
		case TokenType::KW_READONLY:
			s = "KW_READONLY";
			break;
		case TokenType::KW_AS:
			s = "KW_AS";
			break;
		case TokenType::KW_IS:
			s = "KW_IS";
			break;
		case TokenType::KW_DECLARE:
			s = "KW_DECLARE";
			break;
		case TokenType::KW_FROM:
			s = "KW_FROM";
			break;
		case TokenType::KW_MODULE:
			s = "KW_MODULE";
			break;
		case TokenType::KW_REQUIRE:
			s = "KW_REQUIRE";
			break;
		case TokenType::P_COLON:
			s = "P_COLON";
			break;
		case TokenType::P_SEMICOLON:
			s = "P_SEMICOLON";
			break;
		case TokenType::P_NEWLINE:
			s = "P_NEWLINE";
			break;
		case TokenType::P_ARROW:
			s = "P_ARROW";
			break;
		case TokenType::P_SPREAD:
			s = "P_SPREAD";
			break;
		case TokenType::P_DOT:
			s = "P_DOT";
			break;
		case TokenType::P_COMMA:
			s = "P_COMMA";
			break;
		case TokenType::P_SLASH:
			s = "P_SLASH";
			break;
		case TokenType::P_ANTISLASH:
			s = "P_ANTISLASH";
			break;
		case TokenType::P_QUESTION:
			s = "P_QUESTION";
			break;
		case TokenType::P_STAR:
			s = "P_STAR";
			break;
		case TokenType::P_OPEN_PAR:
			s = "P_OPEN_PAR";
			break;
		case TokenType::P_CLOSE_PAR:
			s = "P_CLOSE_PAR";
			break;
		case TokenType::P_OPEN_BRACE:
			s = "P_OPEN_BRACE";
			break;
		case TokenType::P_CLOSE_BRACE:
			s = "P_CLOSE_BRACE";
			break;
		case TokenType::P_OPEN_BRACKET:
			s = "P_OPEN_BRACKET";
			break;
		case TokenType::P_CLOSE_BRACKET:
			s = "P_CLOSE_BRACKET";
			break;
		case TokenType::P_EQUAL:
			s = "P_EQUAL";
			break;
		case TokenType::P_VERTICAL_BAR:
			s = "P_VERTICAL_BAR";
			break;
		case TokenType::P_GREATER_THAN:
			s = "P_GREATER_THAN";
			break;
		case TokenType::P_LESS_THAN:
			s = "P_LESS_THAN";
			break;
		case TokenType::P_AMPERSAND:
			s = "P_AMPERSAND";
			break;
		case TokenType::STRING_LITERAL:
			s = "STRING_LITERAL";
			break;
		case TokenType::NUMBER:
			s = "NUMBER";
			break;
		case TokenType::ElementKey:
			s = "ElementKey";
			break;
		case TokenType::DotId:
			s = "DotId";
			break;
		case TokenType::AnonymousType:
			s = "AnonymousType";
			break;
		case TokenType::TypeDef:
			s = "TypeDef";
			break;
		case TokenType::Type:
			s = "Type";
			break;
		case TokenType::LambdaType:
			s = "LambdaType";
			break;
		case TokenType::TypeGroup:
			s = "TypeGroup";
			break;
		case TokenType::UnionType:
			s = "UnionType";
			break;
		case TokenType::FunctionTypePredicate:
			s = "FunctionTypePredicate";
			break;
		case TokenType::Variable:
			s = "Variable";
			break;
		case TokenType::MapObject:
			s = "MapObject";
			break;
		case TokenType::ParameterPack:
			s = "ParameterPack";
			break;
		case TokenType::ObjectCallable:
			s = "ObjectCallable";
			break;
		case TokenType::Function:
			s = "Function";
			break;
		case TokenType::Constructor:
			s = "Constructor";
			break;
		case TokenType::Parameter:
			s = "Parameter";
			break;
		case TokenType::Property:
			s = "Property";
			break;
		case TokenType::GenericParameter:
			s = "GenericParameter";
			break;
		case TokenType::GenericParameterPack:
			s = "GenericParameterPack";
			break;
		case TokenType::ClassElement:
			s = "ClassElement";
			break;
		case TokenType::Class:
			s = "Class";
			break;
		case TokenType::GlobalFunction:
			s = "GlobalFunction";
			break;
		case TokenType::GlobalVariable:
			s = "GlobalVariable";
			break;
		case TokenType::NamespaceElement:
			s = "NamespaceElement";
			break;
		case TokenType::Namespace:
			s = "Namespace";
			break;
		case TokenType::Import:
			s = "Import";
			break;
		case TokenType::Export:
			s = "Export";
			break;
		case TokenType::FileElement:
			s = "FileElement";
			break;
		case TokenType::File:
			s = "File";
			break;
		default:
			s = std::to_string(static_cast<int>(t));
			break;
		}

		return os << s;
	}
}