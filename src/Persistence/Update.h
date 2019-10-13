/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 * Version       :  1.0
 * Last Modified :  Jan 3rd, 2008
 * Description


 */
#ifndef _PERSISTENCE_UPDATE_H_
#define _PERSISTENCE_UPDATE_H_
#include <LifecycleObject.h>
#include <Persistence/Query.h>
#include <Persistence/Where.h>
#include <Exceptions/NotFound.h>
#include <Core/Types/ValueType.h>

namespace Meow
{
	namespace Persistence
	{
		class Database;
		typedef Reference<Database> Database_ptr;

		class UpdateFactory;
		typedef Reference<UpdateFactory> UpdateFactory_ptr;
		class UpdateFactory : public UncollectedFactory
		{
		public:
			UpdateFactory();
			~UpdateFactory();

			virtual void __Destroy( );
			virtual LifecycleObject_ptr  GetInstance( std::string service ) throw (Meow::Exceptions::NotFound);
		};

		class Update;
		typedef Reference<Update> Update_ptr;

		class Update : public Query
		{
		public:
			Update();
			~Update();
			std::string GetWhat();
			Where_ptr GetWhere();
			void         SetWhat( std::string table );
			void         AddAssign( std::string column, std::string value );
                        void         AddAssign( std::string column, char * value );
			void             AddAssign( std::string column, bool value );
			void         AddAssign( std::string column, float value );
			void             AddAssign( std::string column, double value );
			void         AddAssign( std::string column, int value );
			void         AddAssign( std::string column, unsigned int value );
			void         AddAssign( std::string column, long value );
			void         AddAssign( std::string column, unsigned long value );
			void         AddAssign( std::string column, Core::Types::ValueType_ptr value );
			void         SetWhere( Where_ptr where );

			virtual std::string Generate( Database_ptr database );
			virtual SQLResult_ptr Execute( Database_ptr database );
			virtual std::string ToXML( );

		private:
			std::string iFrom;
			Core::Types::Array_ptr iAssigns;
			Where_ptr iWhere;
		}; // End of Insert
	} // End of NameSpace Persistence
} // End of NameSpace Meow

#endif
