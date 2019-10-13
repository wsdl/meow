#include <Persistence/Select.h>
#include <Persistence/Database.h>
#include <Core/Types/Integer.h>

Meow::Persistence::Join::Join()
{
}

Meow::Persistence::Join::~Join()
{
}

Meow::Persistence::SelectFactory::SelectFactory()
{
    SetMeowClassName("Persistence/SelectFactory");
}
Meow::Persistence::SelectFactory::~SelectFactory()
{
}
void Meow::Persistence::SelectFactory::__Destroy( )
{
}
Meow::LifecycleObject_ptr Meow::Persistence::SelectFactory::GetInstance( std::string service ) throw(Meow::Exceptions::NotFound)
{
    return new Persistence::Select();
}
Meow::Persistence::Select::Select()
{
    SetMeowClassName("Persistence/Select");
    iColumns=new Core::Types::Array();
    iJoins=new Core::Types::Array();
    iLimited=false;
    iDistinct=false;
    iOrdered=false;
    iGroupBy=false;
    iOrderByColumn=new Core::Types::Array();
    iFrom="";
    iOffset=0;
    iLimit=-1;
}
Meow::Persistence::Select::~Select()
{

}

void  Meow::Persistence::Select::SetDistinct( )
{
    iDistinct=true;
}

void  Meow::Persistence::Select::SetGroupBy(std::string column )
{
    iGroupBy=true;
    iGroupByColumn=column;
}

void Meow::Persistence::Select::SetOrderBy( std::string column, int userdefined )
{
    iOrdered=true;
	iOrderColumnOriginal=column;
    // If the string contains a : we explode on it
	Core::Types::String_ptr stringy=new Core::Types::String(column);
	//iOrderByColumn=stringy->Explode(":");
	//Implementation changed to support computed column in order by
	Core::Types::Array_ptr orderByColumn=stringy->Explode(":");
	for (int i=0;i<orderByColumn->Count();i++)
	{
		Core::Types::String_ptr column = (Core::Types::String_ptr) orderByColumn->Get(0);
		if ( userdefined == 1 )
			iOrderByColumn->Set(column->GetValueAsString(),new Core::Types::String("true"));
		else
			iOrderByColumn->Set(column->GetValueAsString(),new Core::Types::String("false"));
	}
	iOrderDirection="ASC";
}

void Meow::Persistence::Select::SetDescending( )
{
    iOrderDirection="DESC";
}

void  Meow::Persistence::Select::SetFrom( std::string table )
{
    iFrom=table;
}

void  Meow::Persistence::Select::AddColumn( std::string column, std::string as, std::string isExpression )
{
	Core::Types::Array_ptr params=new Core::Types::Array();
	params->Set(0,new Core::Types::String(column));
	params->Set(1,new Core::Types::String(as));
	if (!isExpression.compare("true"))
		params->Set(2,new Core::Types::String("true"));
	else
		params->Set(2,new Core::Types::String("false"));
	iColumns->Set(iColumns->Count(),(Core::ptr)params);
}
	

void Meow::Persistence::Select::SetLimit( unsigned int offset, unsigned int limit )
{
    iLimited=true;
    iOffset=offset;
    iLimit=limit;
}

void Meow::Persistence::Select::AddJoin( std::string jointype, std::string tablename, std::string leftkey, std::string rightkey, std::string mytable )
{
    Join_ptr join=new Join();
    join->mode=jointype;
    join->table=tablename;
    join->leftkey=leftkey;
    join->rightkey=rightkey;
	join->mytable=mytable;
	iJoins->Set(tablename,(Core::ptr)join);
}

void Meow::Persistence::Select::SetWhere( Meow::Persistence::Where_ptr where )
{
    iWhere=where;
}

std::string Meow::Persistence::Select::Generate( Meow::Persistence::Database_ptr database )
{
    std::string query="SELECT ";
    if ( iDistinct )
        query.append(" DISTINCT ");

    // DS - 8/6/2012 - Assume that if we have no columns its a count, as otherwise we end up loading everything OR if its
    // lazy loading a text column it is a problem for our runtime (found during Ofsted/MetaDataItem)
    if ( iColumns->Count() == 0 )
    {
	
        query.append(" COUNT(*) AS hits FROM ");
        query.append(database->SanitizeTableName(database->ConvertClass(iFrom)));
		if ( !iWhere.IsNull() && iWhere->IsInstanceOf("Persistence/Where") )
        {
            if ( Unitemporal )
            {
                iWhere->SetUniTemporal();
            }
            query.append(iWhere->Generate(database));
        }
        else
        {
                if ( Unitemporal )
            {
                query.append(" WHERE ( VTEnd >=" + Baseline + " AND VTBegin <=" + Baseline + " )");
            }
        }

    }
    else
    {
        int i=0;
        for ( int j=0; j<iColumns->Count(); j++ )
        {
        	Core::Types::Array_ptr params =(Core::Types::Array_ptr)iColumns->Get(j);
        	Core::Types::String_ptr column=(Core::Types::String_ptr) params->Get(0);
        	Core::Types::ValueType_ptr _as = (Core::Types::ValueType_ptr) params->Get(1);
        	Core::Types::String_ptr as = new Core::Types::String(_as->GetValueAsString());
        	Core::Types::ValueType_ptr isExpression = (Core::Types::ValueType_ptr) params->Get(2);
            if (i>0)
                query.append(",");
            if ( !column->GetValueAsString().compare("COUNT(*)") || !column->GetValueAsString().compare("*")  )
			{
                query.append(column->GetValueAsString());
				if ( ! as.IsNull() )
				{
				    if ( as->GetValue().length()>0 )
				    {
				            query.append(" AS ");
				            query.append(database->SanitizeColumnName(as->GetValue(),false));
				    }
				}
			}
            else
		    {
				if ( !isExpression->GetValueAsString().compare("false") )
				{
					Core::Types::Array_ptr colComponents=column->Explode(".");
					if ( colComponents->Count() == 1 )
					{
						query.append(database->SanitizeColumnName(database->ConvertAttribute(column->GetValueAsString())));
					}
					else
					{
						Core::Types::String_ptr classPart=(Core::Types::String_ptr)colComponents->Get(0);
						Core::Types::String_ptr columnPart=(Core::Types::String_ptr)colComponents->Get(1);
						query.append(database->SanitizeTableName(database->ConvertClass(classPart->GetValueAsString())) + "." +
						database->SanitizeColumnName(database->ConvertAttribute(columnPart->GetValueAsString())));
					}
				}
				else
					query.append(column->GetValue());
				if ( ! as.IsNull() )
				{
				    if ( as->GetValue().length()>0 )
				    {
				            query.append(" AS ");
				            query.append(database->SanitizeColumnName(as->GetValue(),false));
				    }
				}
				i++;
			 }
		}
        query.append(" FROM ");
        query.append(database->SanitizeTableName(database->ConvertClass(iFrom)));

        if ( iJoins->Count()>0 )
        {
        	Core::Types::Stack_ptr keys=iJoins->GetKeys();
        	Core::Types::Array_ptr unresolved=new Core::Types::Array();
        	Core::Types::Array_ptr resolved=new Core::Types::Array();
			// If the join->classname hasn't been joined already, 
			// join it
			bool swaps=false;
			while ( !keys->IsEmpty() )
			{
				Core::Types::ValueType_ptr joinKey=(Core::Types::ValueType_ptr)keys->Pop();
				Join_ptr join=(Join_ptr)iJoins->Get(joinKey->GetValueAsString());

				if ( join->mytable.compare(iFrom) && join->mytable.length()>0 )
				{
					// Its not "From" so its a class that we'd need to join already
					if ( resolved->IsSet(join->mytable) )
						resolved->Set(joinKey->GetValueAsString(),(Core::ptr)join);
					else
						unresolved->Set(joinKey->GetValueAsString(),(Core::ptr)join);
				} 
				else
				{
					// Its from, so its resolved
					resolved->Set(joinKey->GetValueAsString(),(Core::ptr)join);
				}
			}
			while ( unresolved->Count()>0 )
			{
				keys=unresolved->GetKeys();
				while ( !keys->IsEmpty() )
				{
					Core::Types::ValueType_ptr joinKey=(Core::Types::ValueType_ptr)keys->Pop();
					Join_ptr join=(Join_ptr)iJoins->Get(joinKey->GetValueAsString());
					// Its not "From" so its a class that we'd need to join already
					if ( resolved->IsSet(join->mytable) )
					{
						resolved->Set(joinKey->GetValueAsString(),(Core::ptr)join);
						unresolved->Unset(joinKey->GetValueAsString());
					}
				}
			}
			keys=resolved->GetKeys();
			while ( !keys->IsEmpty() )
			{
				Core::Types::ValueType_ptr joinKey=(Core::Types::ValueType_ptr)keys->Pop();
				Join_ptr join=(Join_ptr)iJoins->Get(joinKey->GetValueAsString());
				query.append(" ");
				query.append(join->mode);
				query.append(" JOIN ");
				query.append(database->SanitizeTableName(database->ConvertClass(join->table)));
				query.append(" ON ");
				if ( join->mytable.length() == 0 )
				    query.append(database->CastToString(database->SanitizeTableName(database->ConvertClass(iFrom)) + "." + database->SanitizeColumnName(database->ConvertAttribute(join->leftkey))));
				else
				    query.append(database->CastToString(database->SanitizeTableName(database->ConvertClass(join->mytable)) + "." + database->SanitizeColumnName(database->ConvertAttribute(join->leftkey))));

				query.append("=");
				query.append(database->CastToString(database->SanitizeTableName(database->ConvertClass(join->table)) + "." + database->SanitizeColumnName(database->ConvertAttribute(join->rightkey))));
			}
        }

        if ( !iWhere.IsNull() && iWhere->IsInstanceOf("Persistence/Where") )
        {
            if ( Unitemporal )
            {
                iWhere->SetUniTemporal();
            }
            query.append(iWhere->Generate(database));
        }
        else
        {
	        if ( Unitemporal )
            {
                query.append(" WHERE ( VTEnd >=" + Baseline + " AND VTBegin <=" + Baseline + " )");
            }
        }
        if ( iGroupBy )
        {
            query.append(" GROUP BY ");
            Core::Types::String_ptr groupBy=new Core::Types::String(iGroupByColumn);
		   	Core::Types::Array_ptr colComponents=groupBy->Explode(".");
		  	if ( colComponents->Count() == 1 )
			{
				 query.append(database->SanitizeColumnName(database->ConvertAttribute(iGroupByColumn)));
			}
			else
			{
				Core::Types::String_ptr classPart=(Core::Types::String_ptr)colComponents->Get(0);
				Core::Types::String_ptr columnPart=(Core::Types::String_ptr)colComponents->Get(1);
				query.append(database->SanitizeTableName(database->ConvertClass(classPart->GetValueAsString())) + "." +
				database->SanitizeColumnName(database->ConvertAttribute(columnPart->GetValueAsString())));
			}
            query.append(" ");
        }
        if ( iOrdered )
        {
            query.append(" ORDER BY ");
            int i=0;
            Core::Types::Stack_ptr keys=iOrderByColumn->GetKeys();
            while ( !keys->IsEmpty() )
            {
            	Core::Types::String_ptr curColumn=(Core::Types::String_ptr)keys->Pop();
            	Core::Types::String_ptr userdefined=(Core::Types::String_ptr)iOrderByColumn->Get(i);
            	if ( i>0 )
            		query.append(",");
            	if ( !userdefined->GetValueAsString().compare("false"))
            	{
            		Core::Types::Array_ptr colComponents=curColumn->Explode(".");
            		if ( colComponents->Count() == 1 )
            		{
            			query.append(database->SanitizeColumnName(database->ConvertAttribute(curColumn->GetValueAsString())));
            		}
            		else
            		{
            			Core::Types::String_ptr classPart=(Core::Types::String_ptr)colComponents->Get(0);
            			Core::Types::String_ptr columnPart=(Core::Types::String_ptr)colComponents->Get(1);
            			query.append(database->SanitizeTableName(database->ConvertClass(classPart->GetValueAsString())) + "." +
            					database->SanitizeColumnName(database->ConvertAttribute(columnPart->GetValueAsString())));
            		}
            	}
            	else
            		query.append(curColumn->GetValueAsString());
                i++;
            }
            query.append(" ");
            query.append(iOrderDirection);
        }
    }
    if ( iLimited )
    {
    	Core::Types::Integer_ptr offset=new Core::Types::Integer(iOffset);
    	Core::Types::Integer_ptr limit=new Core::Types::Integer(iLimit);
	
		if ( offset->GetValueAsInteger()>-1 && limit->GetValueAsInteger() >0 )
		{
			// DP changed it for broader SQL implementation compatibility
			query.append(" LIMIT ");
			query.append(limit->ToString());
			query.append(" OFFSET ");
			query.append(offset->ToString());
		}
    }

    query.append(";");
    return query;
}
Meow::Persistence::SQLResult_ptr Meow::Persistence::Select::Execute( Meow::Persistence::Database_ptr database )
{
    Query_ptr query=(Query*)this;
    return database->ObjectQuery(query);
}
std::string Meow::Persistence::Select::ToXML( )
{
    std::string retval="<Query type=\"Select\"><Table>" + iFrom + "</Table>";
    if ( iLimited )
    {
    	Core::Types::Integer_ptr offset=new Core::Types::Integer(iOffset);
    	Core::Types::Integer_ptr limit=new Core::Types::Integer(iLimit);
        retval=retval + "<Limited>true</Limited><Offset>" + offset->GetValueAsString() + "</Offset>" + "<limit>" + limit->GetValueAsString() + "</iLimit>";
    }
    if ( iDistinct )
    {
        retval=retval + "<Distinct>true</Distinct>";
    }
    if ( iOrdered )
    {
        retval=retval + "<Ordered>true</Ordered><OrderColumn>" + iOrderColumnOriginal + "</OrderColumn><OrderDirection>" + iOrderDirection + "</OrderDirection>";
    }
    if ( iGroupBy )
    {
        retval=retval + "<GroupBy>true</GroupBy><GroupColumn>" + iGroupByColumn + "</GroupColumn>";
    }
    if ( !iWhere.IsNull() )
        retval=retval + iWhere->ToXML();
    Core::Types::Stack_ptr JoinKeys=iJoins->GetKeys();
    while ( !JoinKeys->IsEmpty() )
    {
    	Core::Types::String_ptr JoinKey=(Core::Types::String_ptr)JoinKeys->Pop();
        Join_ptr Join=(Join_ptr)iJoins->Get(JoinKey->GetValueAsString());
        retval=retval + "<Join mode=\"" + Join->mode + "\" table=\"" + Join->table + "\" leftkey=\"" + Join->leftkey + "\" rightkey=\"" + Join->rightkey + "\"/>";
    }
    Core::Types::Stack_ptr ColumnKeys=iColumns->GetKeys();
    while ( !ColumnKeys->IsEmpty() )
    {
    	Core::Types::String_ptr ColumnKey=(Core::Types::String_ptr)ColumnKeys->Pop();
    	Core::Types::String_ptr AsValue=(Core::Types::String_ptr)iColumns->Get(ColumnKey->GetValueAsString());
        retval=retval + "<Column name=\"" + ColumnKey->GetValueAsString() + "\" as=\"" + AsValue->GetValueAsString() + "\"/>";
    }
    if ( Unitemporal )
    {
        retval=retval + "<Unitemporal>true</Unitemporal>";
        retval=retval + "<Baseline>" + Baseline + "</Baseline>";
    }
    retval=retval + "</Query>";
    return retval;
}

