#include <Persistence/Where.h>
#include <Persistence/Select.h>
#include <Persistence/Database.h>
#include <iostream>
#include <Core/Types/Integer.h>
#include <Core/Types/Float.h>


Meow::Persistence::WhereFactory::WhereFactory()
{
    SetMeowClassName("Persistence/WhereFactory");
}
Meow::Persistence::WhereFactory::~WhereFactory()
{
}
void Meow::Persistence::WhereFactory::__Destroy( )
{
}

Meow::Persistence::Clause::Clause()
{
    SetMeowClassName("Persistence/Clause");
}

Meow::Persistence::Clause::~Clause( )
{
}

Meow::LifecycleObject_ptr Meow::Persistence::WhereFactory::GetInstance( std::string service ) throw(Meow::Exceptions::NotFound)
{
    try
    {
        Meow::LifecycleObject_ptr retval=(Meow::LifecycleObject_ptr)new Persistence::Where();
        return retval;
    } catch ( Exception &e )
    {
    }
}
Meow::Persistence::Where::Where()
{
    SetMeowClassName("Persistence/Where");
    iClauses=new Meow::Core::Types::Array();
	nesting=0;
}
Meow::Persistence::Where::~Where()
{
}
void Meow::Persistence::Where::Reset( )
{
    iClauses=new Meow::Core::Types::Array();
}

void Meow::Persistence::Where::Bra( )
{
	nesting++;
   Clause_ptr clause=new Clause();
    clause->type="bra";
    iClauses->Set(iClauses->Count(),(Core::ptr)clause);
}

void Meow::Persistence::Where::Ket( )
{
	if ( nesting> 0 )
	{
	    Clause_ptr clause=new Clause();
	    clause->type="ket";
	    iClauses->Set(iClauses->Count(),(Core::ptr)clause);
		nesting--;
	}
}

void Meow::Persistence::Where::LogicalAnd()
{

   Clause_ptr clause=new Clause();
    clause->type="and";
    iClauses->Set(iClauses->Count(),(Core::ptr)clause);
}

void Meow::Persistence::Where::LogicalOr()
{
   Clause_ptr clause=new Clause();
    clause->type="or";
    iClauses->Set(iClauses->Count(),(Core::ptr)clause);
}

void Meow::Persistence::Where::Expression( std::string column, std::string op, Core::ptr value, std::string valuetype, Factory::SchemaColumnType stype )
{

   if(valuetype.compare("computed"))
	{
		Clause_ptr clause=new Clause();
		clause->type="expression";
		clause->op=op;
		clause->value=value;
		clause->valuetype=valuetype;
		clause->column=column;
		clause->complex=false;
		clause->stype=stype;
		iClauses->Set(iClauses->Count(),(Core::ptr)clause);
	}
	else
	{
		Clause_ptr clause=new Clause();
		clause->type="userdefinedexpression";
		clause->op="computed";
		clause->value=value;
		clause->valuetype="computed";
		clause->column="computed";
		clause->complex=false;	
		clause->stype=stype;
		iClauses->Set(iClauses->Count(),(Core::ptr)clause);
	}
}
void Meow::Persistence::Where::UserDefinedExpression( std::string expression )
{
	Core::Types::String_ptr value_as_object = new Core::Types::String(expression);
	Expression("","",(Core::ptr)value_as_object,"computed");
}
void Meow::Persistence::Where::Expression( std::string column, std::string op, std::string value )
{
	Core::Types::String_ptr value_as_string=new Core::Types::String(value);
    Expression(column,op,(Core::ptr)value_as_string,"value");
}


void Meow::Persistence::Where::Expression( std::string column, std::string op, int value )
{
	Core::Types::Integer_ptr value_as_object=new Core::Types::Integer(value);
    Expression(column,op,(Core::ptr)value_as_object,"value",Factory::Integer);
}


void Meow::Persistence::Where::Expression( std::string column, std::string op, long value )
{
	Core::Types::Integer_ptr value_as_object=new Core::Types::Integer(value);
    Expression(column,op,(Core::ptr)value_as_object,"value",Factory::Integer);
}


void Meow::Persistence::Where::Expression( std::string column, std::string op, unsigned long value )
{
	Core::Types::Integer_ptr value_as_object=new Core::Types::Integer(value);
    Expression(column,op,(Core::ptr)value_as_object,"value",Factory::Integer);
}

void Meow::Persistence::Where::Expression( std::string column, std::string op, unsigned int value )
{
	Core::Types::Integer_ptr value_as_object=new Core::Types::Integer(value);
    Expression(column,op,(Core::ptr)value_as_object,"value",Factory::Integer);
}

void Meow::Persistence::Where::Expression( std::string column, std::string op, float value )
{
	Core::Types::Float_ptr value_as_object=new Core::Types::Float(value);
    Expression(column,op,(Core::ptr)value_as_object,"value",Factory::Float);
}


void Meow::Persistence::Where::Expression( std::string column, std::string op, double value )
{
	Core::Types::Float_ptr value_as_object=new Core::Types::Float(value);
    Expression(column,op,(Core::ptr)value_as_object,"value",Factory::Float);
}
void Meow::Persistence::Where::LinkColumn( std::string left, std::string op, std::string right_column )
{
   Clause_ptr clause=new Clause();
    clause->type="expression";
    clause->op=op;
    clause->value=new Core::Types::String(right_column);
    clause->valuetype="column";
    clause->column=left;
    clause->complex=true;
    iClauses->Set(iClauses->Count(),(Core::ptr)clause);

}

void Meow::Persistence::Where::ComplexExpression( std::string left, std::string op, std::string right )
{
    Clause_ptr clause=new Clause();
    clause->type="expression";
    clause->op=op;
    clause->value=new Core::Types::String(right);
    clause->valuetype="value";
    clause->column=left;
    clause->complex=true;
    iClauses->Set(iClauses->Count(),(Core::ptr)clause);

}



std::string Meow::Persistence::Where::Generate( Meow::Persistence::Database_ptr database )
{
    std::string where=" ";
    std::string retwhere="";
    // if ( Unitemporal )
    //    where=" ( VTEnd>= " + Baseline + " AND VTBegin<=" + Baseline + " ";
    //if ( Unitemporal && iClauses->Count() > 0 )
    //    where.append(" AND ");

    for ( unsigned int i=0; i<iClauses->Count(); i++ )
    {
        Clause_ptr clause=(Clause_ptr)iClauses->Get(i);
        if ( !clause->type.compare("expression") )
        {
            where.append(" ");
            if ( !clause->complex )
			{
				// Now check to see if there is a . in it
            	Core::Types::String_ptr column=new Core::Types::String(clause->column);
            	Core::Types::Array_ptr colComponents=column->Explode(".");
				if ( colComponents->Count() == 1 )
				{
				  std::string output=database->SanitizeColumnName(database->ConvertAttribute(clause->column));
				  where.append(output);
				}
				else
				{
					Core::Types::String_ptr classPart=(Core::Types::String_ptr)colComponents->Get(0);
					Core::Types::String_ptr columnPart=(Core::Types::String_ptr)colComponents->Get(1);
					where.append(database->SanitizeTableName(database->ConvertClass(classPart->GetValueAsString())) + "." +
						  database->SanitizeColumnName(database->ConvertAttribute(columnPart->GetValueAsString())));
				}
			}
            else
			{
				where.append(clause->column);
			}
            where.append(" ");
            where.append(clause->op);
            where.append(" ");
            if ( !clause->valuetype.compare("value") )
            {
            	Core::Types::ValueType_ptr realvalue=(Core::Types::ValueType_ptr)clause->value;
                if ( !clause->complex )
                {
                	switch (clause->stype)
                	{
                	case Factory::Integer:
                	case Factory::Float:
                	case Factory::Date:
                	case Factory::Boolean:
                		where.append(realvalue->GetValueAsString());
                		break;
                	default:
                		where.append(database->Sanitize(realvalue->GetValueAsString()));
                	}
                }
                else
                    where.append(realvalue->GetValueAsString());
            }
            else if ( !clause->valuetype.compare("column") )
            {
            	Core::Types::ValueType_ptr realvalue=(Core::Types::ValueType_ptr)clause->value;
                where.append(database->SanitizeColumnName(database->ConvertAttribute(realvalue->GetValueAsString())));
            }
            else
            {
                Select_ptr realvalue=(Select_ptr)clause->value;
                where.append(" ( ");
                where.append(realvalue->Generate(database));
                where.append(" ) ");
            }
        }
		else if ( !clause->type.compare("userdefinedexpression") )
		{
			Core::Types::ValueType_ptr realvalue=(Core::Types::ValueType_ptr)clause->value;
			where.append(realvalue->GetValueAsString());
		}
        else if ( !clause->type.compare("bra") )
        {
            where.append(" ( ");
        }
        else if ( !clause->type.compare("ket") )
        {
            where.append(" ) ");
        }
        else if ( !clause->type.compare("and") )
        {
            where.append(" AND ");
        }
        else if ( !clause->type.compare("or") )
        {
            where.append(" OR ");
        }
    }
    // if ( Unitemporal )
     //   where.append(" ) ");

    if(where.compare(" "))
    {
        retwhere=std::string(" WHERE") + where;
    }
    return retwhere;
}

std::string Meow::Persistence::Where::ToXML( )
{
    std::string retval="<Where>";
    Core::Types::Stack_ptr Clauses=iClauses->GetKeys();
    while ( !Clauses->IsEmpty())
    {
    	Core::Types::String_ptr ClauseKey=(Core::Types::String_ptr)Clauses->Pop();
        Clause_ptr Clause=(Clause_ptr)iClauses->Get(ClauseKey->GetValueAsString());
        retval=retval + "<Clause type=\"" + Clause->type + "\" column=\"" + Clause->column + "\" op=\"" + Clause->op + "\" valuetype=\"" + Clause->valuetype + "\">";
        Core::Types::ValueType_ptr value=(Core::Types::ValueType_ptr)Clause->value;
        Core::Types::ValueType_ptr valueEncoded=value->Base64Encode();
        retval=retval + valueEncoded->GetValueAsString();
        retval=retval + "</Clause>";
    }
    retval=retval + "</Where>";
    return retval;
}

Meow::Core::Types::Array_ptr Meow::Persistence::Where::GetClauses()
{
    return iClauses;
}

