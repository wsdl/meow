/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 * Version       :  1.0
 * Last Modified :  Jan 3rd, 2008
 * Description

   The query class is a super class to Insert, Update, Delete, Select

 */
#ifndef _PERSISTENCE_QUERY_H_
#define _PERSISTENCE_QUERY_H_
#include <LifecycleObject.h>
#include <Factory.h>
#include <Exceptions/NotFound.h>
#include <Persistence/SQLResult.h>
#include <Core/Tools/XMLParser.h>

namespace Meow
{
	namespace Persistence
	{
		class SQLResult;
		typedef Reference<SQLResult> SQLResult_ptr;

		class Query;
		typedef Reference<Query> Query_ptr;

		class Database;
		typedef Reference<Database> Database_ptr;

		class QueryFactory;
		typedef Reference<QueryFactory> QueryFactory_ptr;

		class XMLReconstruction;
		typedef Reference<XMLReconstruction> XMLReconstruction_ptr;

		class QueryFactory : public Factory
		{
		public:
			QueryFactory();
			~QueryFactory();

			virtual void __Destroy( );
			virtual LifecycleObject_ptr GetInstance( std::string service )
				throw(Meow::Exceptions::NotFound);
			Query_ptr RebuildFromXML( std::string xmlDoc );
		};

		class XMLReconstruction : public Core::Tools::XMLParser
		{
		public:
			XMLReconstruction();
			~XMLReconstruction();

			virtual void StartElement( string element, Core::Types::Array_ptr attributes );
			virtual void EndElement( string element );
			virtual void CharacterData( string data );

			Query_ptr GetQuery();
		private:
			Query_ptr iQuery;
			Core::ptr iWhere;
			std::string iQueryType;
			std::string iCurrentElement;
			std::string iCurrentElementData;
			Core::Types::Array_ptr iCurrentAttributes;
		};

		class Query : public TransientObject
		{
		public:
			Query();
			~Query();

			virtual std::string Generate( Database_ptr database );
			virtual Meow::Persistence::SQLResult_ptr Execute( Database_ptr database );
			virtual void SetUniTemporal();
			virtual void SetBaseline( std::string baseline );
			virtual string ToXML( );

		protected:
			friend class XMLReconstruction;
			Core::Types::Array_ptr Internal;
			bool Unitemporal;
			std::string Baseline;
		}; // End of Query
	} // End of NameSpace Persistence
} // End of NameSpace Meow

#endif
