/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 * Version       :  1.0
 * Last Modified :  Jan 3rd, 2008
 * Description

   The Insert class is a super class to Insert, Update, Delete, Select

 */
#ifndef _PERSISTENCE_INSERT_H_
#define _PERSISTENCE_INSERT_H_
#include <Persistence/Query.h>
#include <Core/Types/ValueType.h>
#include <UncollectedFactory.h>
namespace Meow
{
	namespace Persistence
	{
		class Database;
		typedef Reference<Database> Database_ptr;

		class InsertFactory;
		typedef Reference<InsertFactory> InsertFactory_ptr;
		class InsertFactory : public UncollectedFactory
		{
		public:
			InsertFactory();
			~InsertFactory();

			virtual void __Destroy( );
			virtual LifecycleObject_ptr
			GetInstance( std::string service )
				throw(Meow::Exceptions::NotFound);
		};

		class Insert;
		typedef Reference<Insert> Insert_ptr;

		class Insert : public Query
		{
		public:
			Insert();
			~Insert();

			void         SetInto( std::string table );
			void         SetFrom( std::string table );
			void         AddAssign( std::string column, char * value );
			void         AddAssign( std::string column, std::string value );
			void         AddAssign( std::string column, bool value );
			void         AddAssign( std::string column, float value );
			void         AddAssign( std::string column, double value );
			void         AddAssign( std::string column, int value );
			void         AddAssign( std::string column, unsigned int value );
			void         AddAssign( std::string column, long value );
			void         AddAssign( std::string column, unsigned long value );
			void         AddAssign( std::string column, Core::Types::ValueType_ptr value );
			virtual std::string Generate( Database_ptr database );
			virtual SQLResult_ptr Execute( Database_ptr database );
			virtual std::string ToXML( );


		private:

			std::string iFrom;
			Core::Types::Array_ptr iAssigns;
		}; // End of Insert
	} // End of NameSpace Persistence
} // End of NameSpace Meow

#endif
