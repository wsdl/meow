/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */

#ifndef LAUNCHER_CONFIGURATION_H_
#define LAUNCHER_CONFIGURATION_H_
#include <Core/Types/Array.h>
#include <Core/Types/Stack.h>
#include <Core/Object.h>

class Configuration;
typedef Reference<Configuration> Configuration_ptr;

class Configuration : public Meow::Core::Object
{
private:
	Configuration();

public:
	static Configuration_ptr GetConfiguration( std::string linkname="meow.cnf");
	~Configuration();
	void InstallFrom( std::string filename );

	void Rewrite( Meow::Core::Types::Array_ptr pairs );
	void Passivate( );
	bool CheckMandatories();
	bool LoadExtensions();

	void (*meow_startup)(Configuration_ptr);
	void (*meow_security)(Configuration_ptr);
	void (*meow_end)(Configuration_ptr);

	bool   IsSet( string name );
	void   UnSet( string name );
	string Get( string name );
	void   Set( string name, string value );
	Meow::Core::Types::Stack_ptr GetKeys();
	Meow::Core::Types::Array_ptr GetExtensions( );
	long   LastUpdated( std::string service );
	void   MarkUpdated( std::string service );

	void   AddExtension( string extension );
private:
	Meow::Core::Types::Array_ptr iCache;
	Meow::Core::Types::Array_ptr iLibraries;
	Meow::Core::Types::Array_ptr iExtensions;
	int iCacheChanges;
	bool iLoaded;
	std::string iConnectString;
	static Configuration_ptr gConfiguration;
};



#endif /* LAUNCHER_CONFIGURATION_H_ */