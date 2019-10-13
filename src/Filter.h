/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */
#ifndef _FILTER_H_
#define _FILTER_H_
#include <TransientObject.h>


namespace Meow {
	class Registry;
	typedef Reference<Registry> Registry_ptr;

	class Filter;
	typedef Reference<Filter> Filter_ptr;

	class Filter : public TransientObject
	{
	public:
		Filter();
		~Filter();
		// the filter interface
		virtual void Reset( )=0;
		virtual void Bra( )=0;
		virtual void Ket( )=0;
		virtual void LogicalAnd()=0;
		virtual void LogicalOr()=0;
		virtual void Expression( std::string column, std::string op, Meow::Core::ptr value, std::string valuetype )=0;
		virtual void Expression( std::string column, std::string op, std::string value )=0;
		virtual void Expression( std::string column, std::string op, int value )=0;
		virtual void Expression( std::string column, std::string op, long value )=0;
		virtual void Expression( std::string column, std::string op, unsigned int value )=0;
		virtual void Expression( std::string column, std::string op, unsigned long value )=0;
		virtual void Expression( std::string column, std::string op, float value )=0;
		virtual void Expression( std::string column, std::string op, double value )=0;
		virtual std::string ToXML()=0;
		virtual Core::Types::Array_ptr GetClauses()=0;
	};
};
#endif
