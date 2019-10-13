/* Copyright     :  (c) 2007,2008 David Spencer All Rights Reserved.
 * Author        :  David Spencer
 * Version       :  1.0
 * Last Modified :  Jan 3rd, 2008
 * Description

   The Database class is a combination of a Factory ( which all database
   connections are loaded through by key ), configuration ( from which the
   factory works out which driver to load from a shared object ), abstract class
   ( from which all drivers get their methods ) and drivers ( which are written
   by the driver writer ).

   There are two types of query supported in Meow 2.0. The first is the
   traditional Writeable and Readable query, the second is a more detailed spec
   based query which lists the tables that will be accessed by the query.

   The second one allows Meow to work out which, if any, can work off a table
   cache and which cannot. For example if we were to run a query that wrote to
   two tables:
        table_a, table_b

   Then the subsequent query that accesses table_a better make sure it doesn't
   ready from any caches, it should read from the master copy. However, if it
   is reading from table_c, this will be OK to read from a cache copy.

   Does this make a big difference to the driver? No, because all cache
   mechanisms work inside of this class.

 */
#ifndef _PERSISTENCE_DATABASE_H_
#define _PERSISTENCE_DATABASE_H_
#include <TransientObject.h>
#include <Factory.h>
#include <Persistence/Query.h>
#include <Persistence/Exceptions/SQLError.h>
#include <Persistence/Exceptions/IPCError.h>
namespace Meow
{
	namespace Persistence
	{
		class Where;
		typedef Reference<Where> Where_ptr;

		class Query;
		typedef Reference<Query> Query_ptr;

		class DatabaseDriver;
		typedef Reference<DatabaseDriver> DatabaseDriver_ptr;

		class SQLResult;
		typedef Reference<SQLResult> SQLResult_ptr;


		class DatabaseFactory;
		typedef Reference<DatabaseFactory> DatabaseFactory_ptr;

		class Database;
		typedef Reference<Database> Database_ptr;


		class DatabaseFactory : public Factory
		{
		public:
			DatabaseFactory();
			~DatabaseFactory();
			virtual void 				RegisterEvents( );
			virtual void 				__Destroy( );
			virtual void 				Purge( );
			virtual LifecycleObject_ptr	GetInstance( std::string service );
			virtual void 				OnEvent( std::string event, Core::Types::Array_ptr parameters );
			void 						PreinstallDriver( std::string driverName, DatabaseDriver_ptr driver );
			DatabaseDriver_ptr 			LoadDriver( std::string driver );
			bool 						SecondPhasePurge();
		protected:
			virtual void 				CanRead();
		private:
			Core::Types::Array_ptr iPreinstalledDrivers;
			Core::Types::Array_ptr iLibraries;
		};


		class Database : public TransientObject
		{
		public:
			Database();
			~Database();
			void            		SetAlias( std::string value );
			std::string     		GetAlias( );
			bool            		GetIsInTransaction( );
			void            		SetIsInTransaction( bool flag );
			void            		SetIsConnected( bool flag );
			bool        			GetIsConnected( );
			virtual void    		Initialise();
			virtual void    		BeginTransaction();
			virtual void    		CommitTransaction();
			virtual void    		RollbackTransaction();
			virtual std::string     Sanitize( std::string data, bool quoteit=true );
			virtual std::string     SanitizeColumnName( std::string data, bool notas=true );
			virtual std::string     SanitizeTableName( std::string data );
			virtual std::string		CastToString( std::string data );
			virtual std::string		CastToInteger( std::string data );
			SQLResult_ptr   		ObjectQuery( Query_ptr query );
			std::string     		ConvertTypeForComparison( std::string typedefof );
			virtual std::string     ConvertClass( std::string classname );
			virtual std::string     ConvertAttribute( std::string classname );
			void 					Create(  std::string classname, Core::Types::Array_ptr operationList );
			void 					Action(  std::string classname, SchemaOperation_ptr operation );

		private:
			unsigned long alias_writes;
			DatabaseDriver_ptr iDriver;
			bool isintransaction;
			bool isconnected;
			bool isunitemporal;
			bool traceresults;
			friend class DatabaseDriver;
		}; // End of Class CoreObject
	}
}
s
#endif