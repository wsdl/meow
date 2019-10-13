/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer

   Driver for SQLITE3

 */
#ifndef _PERSISTENCE_SQLITEDRIVER_H_
#define _PERSISTENCE_SQLITEDRIVER_H_
#include <Persistence/DatabaseDriver.h>

namespace Meow
{
	namespace Persistence
	{
		namespace Drivers
		{
			class SQLITEDriver : public Persistence::DatabaseDriver
			{
			public:
				SQLITEDriver();
				~SQLITEDriver();

				/* Lifecycle operations */
				virtual void    Initialise();
				virtual void    Close();

				/* ACID support */
				virtual void    BeginTransaction();
				virtual void    CommitTransaction();
				virtual void    RollbackTransaction();

				/* Making sure we generate accurate usable SQL */
				virtual std::string     Sanitize( std::string data, bool quoteit=true );
				virtual std::string     SanitizeColumnName( std::string data, bool notas );
				virtual std::string     SanitizeTableName( std::string data );
				virtual std::string		CastToString( std::string data );
				virtual std::string		CastToInteger( std::string data );

				/* Handling the actual query */
				virtual Persistence::SQLResult_ptr   ObjectQuery( Query_ptr query )
				throw(Meow::Persistence::Exceptions::SQLError,
					  Meow::Persistence::Exceptions::IPCError,
					  Meow::Exceptions::Rollback,
					  Meow::Exceptions::Terminate,
					  Meow::Exceptions::Retry);
				void HandleException( int rc ) throw(Meow::Persistence::Exceptions::SQLError,
													 Meow::Persistence::Exceptions::IPCError,
													 Meow::Exceptions::Rollback,
													 Meow::Exceptions::Terminate,
													 Meow::Exceptions::Retry);

				/* Security migration support */
				virtual void                DeleteDatabase( );

				/* Travelling schema and database auto-create support */
				virtual std::string     ConvertTypeForComparison( std::string typedefof );


				/* Remote access support for sqlresult unwind */
				virtual Core::Types::Array_ptr     FetchRowFromResult( void * descriptor, int guideRow=0 );
				virtual void FreeResult( void * descriptor );

				virtual std::string     ConvertClass( std::string classname );
				virtual std::string     ConvertAttribute( std::string classname );


				virtual void Create(  std::string classname, Core::Types::Array_ptr operationList ) 
					throw(Meow::Persistence::Exceptions::SQLError,
						  Meow::Persistence::Exceptions::IPCError,
						  Meow::Exceptions::Rollback,
						  Meow::Exceptions::Terminate);

				virtual void Action( std::string classname, SchemaOperation_ptr operation ) 
					throw(Meow::Persistence::Exceptions::SQLError,
						  Meow::Persistence::Exceptions::IPCError,
						  Meow::Exceptions::Rollback,
						  Meow::Exceptions::Terminate);
	
			private:
				void * driver;
				std::string database;
				std::string unitemporalt;
				std::string encryptionmode;
			};
		}
	}
}


#endif

