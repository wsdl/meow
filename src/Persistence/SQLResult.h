/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 * Version       :  1.0
 * Last Modified :  Jan 3rd, 2008
 * Description

   The SQLResult class just contains the data in cellular format. In 1.3 this was
   tied up to the driver to limit the number of loops however here we return to
   the original design which seperates entirely a Result object from a Driver.

   A driver will instantiate one of these and return it.

 */
#ifndef _PERSISTENCE_SQLRESULT_H_
#define _PERSISTENCE_SQLRESULT_H_
#include <LifecycleObject.h>
#include <Factory.h>
#include <Persistence/Database.h>
#include <Persistence/DatabaseDriver.h>
#include <Exceptions/NotFound.h>

namespace Meow
{
	namespace Persistence
	{
		class DatabaseDriver;
		typedef Reference<DatabaseDriver> DatabaseDriver_ptr;

		class SQLResultFactory;
		typedef Reference<SQLResultFactory> SQLResultFactory_ptr;

		class SQLResult;
		typedef Reference<SQLResult> SQLResult_ptr;

		class XMLResultReconstruction;
		typedef Reference<XMLResultReconstruction> XMLResultReconstruction_ptr;

		class SQLResultFactory : public Factory
		{
		public:
			SQLResultFactory();
			~SQLResultFactory();

			virtual void __Destroy( );
			virtual LifecycleObject_ptr
			GetInstance( std::string service ) throw(Meow::Exceptions::NotFound);
			SQLResult_ptr DoFromXML( std::string xmlDoc );
		};


		class XMLResultReconstruction : public Core::Tools::XMLParser
		{
		public:
			XMLResultReconstruction();
			~XMLResultReconstruction();

			virtual void StartElement( string element, Core::Types::Array_ptr attributes );
			virtual void EndElement( string element );
			virtual void CharacterData( string data );

			SQLResult_ptr GetResult();
		private:
			SQLResult_ptr iResult;
			std::string iCurrentElement;
			std::string iCurrentElementData;
			Core::Types::Array_ptr iCurrentAttributes;
		};

		class SQLResult : public TransientObject
		{
		public:
			SQLResult();
			~SQLResult();

			std::string FetchCellData( unsigned long row, std::string index ) throw (Meow::Exceptions::NotFound);
			Core::Types::ValueType_ptr FetchCellDataAsValue( unsigned long row, std::string index )
				throw(Meow::Exceptions::NotFound);
			Core::Types::Array_ptr FetchRowAsArray( unsigned long row )
				throw(Meow::Exceptions::NotFound);
			std::string GetLastInsertID();
			std::string ToXML( );
			void Release( );
			DatabaseDriver_ptr iDriver;
			friend class XMLResultReconstruction;
			std::string iLastInsertID;
			Core::Types::Array_ptr Internal;
			void * iDriverDescriptor;
			unsigned long loadedRows;
			unsigned long recordcount;
			std::string debugquery;

		}; // End of SQLResult
	} // End of NameSpace Persistence
} // End of NameSpace Meow

#endif
