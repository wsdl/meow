/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 * Version       :  1.0
 * Last Modified :  Dec 9th, 2010
 * Description

   The database driver class is a friend of Persistence::Database  and has access to the internals of same. It provides a similar interface to wherever any
   specialise options are.

   Database drivers should inherit from this and implement the virtual functions.

 */
#ifndef _PERSISTENCE_DATABASEDRIVER_H_
#define _PERSISTENCE_DATABASEDRIVER_H_
#include <LifecycleObject.h>
#include <Persistence/Query.h>
#include <Persistence/Exceptions/SQLError.h>
#include <Persistence/Exceptions/IPCError.h>
#include <Exceptions/Rollback.h>
#include <Exceptions/Terminate.h>
#include <Exceptions/Retry.h>

namespace Meow
{
	namespace Persistence
	{
		class Where;
		typedef Reference<Where> Where_ptr;

		class Query;
		typedef Reference<Query> Query_ptr;

		class SQLResult;
		typedef Reference<SQLResult> SQLResult_ptr;

		class DatabaseDriver;
		typedef Reference<DatabaseDriver> DatabaseDriver_ptr;

		class Database;
		typedef Reference<Database> Database_ptr;

		class DatabaseDriver : public Object
		{
		public:
			DatabaseDriver();
			~DatabaseDriver();

			void SetCaller( Database_ptr caller );

			/* Lifecycle operations */
			virtual void    Initialise()=0;
			virtual void    Close()=0;

			/* ACID support */
			virtual void    BeginTransaction()=0;
			virtual void    CommitTransaction()=0;
			virtual void    RollbackTransaction()=0;

			/* Making sure we generate accurate usable SQL */
			virtual std::string     Sanitize( std::string data, bool quoteit=true )=0;
			virtual std::string     SanitizeColumnName( std::string data, bool notas=true )=0;
			virtual std::string     SanitizeTableName( std::string data )=0;
			virtual std::string		CastToString( std::string data )=0;
			virtual std::string		CastToInteger( std::string data )=0;

			/* Handling the actual query */
			virtual SQLResult_ptr   ObjectQuery( Query_ptr query )=0;

			/* Remote access support for sqlresult unwind */
			virtual Core::Types::Array_ptr     FetchRowFromResult( void * descriptor, int guideRow=0 );
			virtual void FreeResult( void * descriptor );

			/* Conversion */
			virtual std::string     ConvertTypeForComparison( std::string typedefof )=0;
			virtual std::string     ConvertClass( std::string classname )=0;
			virtual std::string     ConvertAttribute( std::string classname )=0;

			virtual void Create(  std::string classname, Core::Types::Array_ptr operationList );
			virtual void Action(  std::string classname,  SchemaOperation_ptr operation );
		protected:
			Database_ptr iCaller;
			Core::Types::Array_ptr ColumnCache;
			bool isunitemporal;
		};
	}
}


#endif

