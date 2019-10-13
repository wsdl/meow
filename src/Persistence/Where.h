/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 * Version       :  1.0
 * Last Modified :  Jan 3rd, 2008
 * Description

   The Insert class is a super class to Insert, Update, Delete, Select

 */
#ifndef _PERSISTENCE_WHERE_H_
#define _PERSISTENCE_WHERE_H_
#include <Persistence/Query.h>
#include <UncollectedFactory.h>

namespace Meow
{
	namespace Persistence
	{
		class Database;
		typedef Reference<Database> Database_ptr;

		class WhereFactory;
		typedef Reference<WhereFactory> WhereFactory_ptr;
		class WhereFactory : public UncollectedFactory
		{
		public:
			WhereFactory();
			~WhereFactory();

			virtual void __Destroy( );
			virtual LifecycleObject_ptr GetInstance( std::string service ) throw(Meow::Exceptions::NotFound);
		};


		class Clause;
		typedef Reference<Clause> Clause_ptr;

		class Clause : public Object
		{
		public:
			Clause();
			~Clause();
			std::string type;
			std::string column;
			std::string op;
			Core::ptr value;
			std::string valuetype;
			bool complex;
			Factory::SchemaColumnType stype;
		};

		class Where;
		typedef Reference<Where> Where_ptr;

		class Where : public Query
		{
		public:
			Where();
			~Where();
			virtual void Reset( );
			virtual void Bra( );
			virtual void Ket( );
			virtual void LogicalAnd();
			virtual void LogicalOr();
			virtual void Expression( std::string column, std::string op, Core::ptr value, std::string valuetype, Factory::SchemaColumnType stype=Factory::String );
			virtual void UserDefinedExpression( std::string expression );
			virtual void Expression( std::string column, std::string op, std::string value );
			virtual void Expression( std::string column, std::string op, int value );
			virtual void Expression( std::string column, std::string op, long value );
			virtual void Expression( std::string column, std::string op, unsigned int value );
			virtual void Expression( std::string column, std::string op, unsigned long value );
			virtual void Expression( std::string column, std::string op, float value );
			virtual void Expression( std::string column, std::string op, double value );
			virtual void LinkColumn( std::string column, std::string op, std::string right_column );
			virtual void ComplexExpression( std::string left, std::string op, std::string right );
			virtual std::string ToXML();
			virtual Core::Types::Array_ptr GetClauses();
			std::string Generate( Database_ptr database );

		private:
			Core::Types::Array_ptr iClauses;
			int nesting;
		}; // End of Insert
	} // End of NameSpace Persistence
} // End of NameSpace Meow

#endif
