/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 * Version       :  1.0
 * Last Modified :  Jan 3rd, 2008
 * Description
 */
#ifndef _PERSISTENCE_DELETE_H_
#define _PERSISTENCE_DELETE_H_
#include <LifecycleObject.h>
#include <Persistence/Query.h>
#include <Persistence/Where.h>
#include <Exceptions/NotFound.h>
#include <UncollectedFactory.h>s
namespace Meow
{
    namespace Persistence
    {
        class Database;
        typedef Reference<Database> Database_ptr;

        class DeleteFactory;
        typedef Reference<DeleteFactory> DeleteFactory_ptr;
        class DeleteFactory : public UncollectedFactory
        {
        public:
            DeleteFactory();
            ~DeleteFactory();

            virtual void __Destroy( );
            virtual LifecycleObject_ptr  GetInstance( std::string service ) throw (Meow::Exceptions::NotFound);
        };


        class Delete;
        typedef Reference<Delete> Delete_ptr;
s
        class Delete : public Query
        {
        public:
        Delete();
        ~Delete();

        void         SetFrom( std::string table );
        void         SetWhere( Where_ptr where );

        virtual std::string Generate( Database_ptr database );
        virtual SQLResult_ptr Execute( Database_ptr database );
        virtual std::string ToXML( );

        // Inherited methods
        virtual void    __Destroy( );
        virtual bool    IsPurgeable( );
        virtual void    Restore( )
        throw(Meow::Exceptions::NotFound,Meow::Exceptions::Rollback,Meow::Exceptions::Terminate);
        virtual void    Passivate( )
        throw(Meow::Exceptions::NotFound,Meow::Exceptions::Rollback,Meow::Exceptions::Terminate);
        virtual void    _Delete( )
        throw(Meow::Exceptions::NotFound,Meow::Exceptions::Rollback,Meow::Exceptions::Terminate);
        virtual void    Create( )
        throw(Meow::Exceptions::Duplicate,Meow::Exceptions::Rollback,Meow::Exceptions::Terminate);

        private:
        std::string iFrom;
        Where_ptr iWhere;

        }; // End of Insert
    } // End of NameSpace Persistence
} // End of NameSpace Meow

#endif
