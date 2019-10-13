/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer 
 * Version       :  1.0
 * Last Modified :  Jan 3rd, 2008
 * Description


 */
#ifndef _PERSISTENCE_SELECT_H_
#define _PERSISTENCE_SELECT_H_
#include <LifecycleObject.h>
#include <Persistence/Query.h>
#include <Persistence/Where.h>
#include <Exceptions/NotFound.h>
#include <UncollectedFactory.h>
namespace Meow
{
	namespace Persistence
	{
		class Database;
		typedef Reference<Database> Database_ptr;

		class SelectFactory;
		typedef Reference<SelectFactory> SelectFactory_ptr;
		class SelectFactory : public UncollectedFactory
		{
		public:
			SelectFactory();
			~SelectFactory();

			virtual void __Destroy( );
			virtual LifecycleObject_ptr  GetInstance( std::string service ) throw (Meow::Exceptions::NotFound);
		};


		class Join;
		typedef Reference<Join> Join_ptr;

		class Join : public Object
		{
		public:
			Join();
			~Join();
			std::string mytable;
			std::string mode;
			std::string table;
			std::string leftkey;
			std::string rightkey;
		};

		class XMLReconstruction;

		class Select;
		typedef Reference<Select> Select_ptr;
		class Select : public Query
		{
		public:
			Select();
			~Select();

			void         SetFrom( std::string table );
			void         AddColumn( std::string column, std::string as="", std::string isExpression="false" );
			void         SetLimit( unsigned int offset, unsigned int limit );
			void         AddJoin( std::string jointype, std::string tablename, std::string leftkey, std::string rightkey, std::string mytable="" );
			void         SetWhere( Where_ptr where );
			void         SetDistinct( );
			void         SetOrderBy( std::string column, int userdefined=0 );
			void         SetGroupBy( std::string column );
			void         SetDescending( );

			virtual std::string Generate( Database_ptr database );
			virtual Meow::Persistence::SQLResult_ptr Execute( Database_ptr database );
			virtual std::string ToXML( );

		private:
			friend class XMLReconstruction;
			std::string iFrom;
			Core::Types::Array_ptr iColumns;
			Core::Types::Array_ptr iJoins;
			bool iLimited;
			unsigned int iOffset;
			unsigned int iLimit;
			bool iDistinct;
			bool iOrdered;
			bool iGroupBy;
			std::string iOrderColumnOriginal;
			Core::Types::Array_ptr iOrderByColumn;
			std::string iGroupByColumn;
			std::string iOrderDirection;
			Where_ptr iWhere;
		}; // End of Insert
	} // End of NameSpace Persistence
} // End of NameSpace Meow

#endif
