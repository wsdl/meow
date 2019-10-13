#include <Core/Tools/Curl.h>
#include <Core/Log.h>
#include <cstring>
static int wait_on_socket(curl_socket_t sockfd, int for_recv, long timeout_ms)
{
  struct timeval tv;
  fd_set infd, outfd, errfd;
  int res;

  tv.tv_sec = timeout_ms / 1000;
  tv.tv_usec= (timeout_ms % 1000) * 1000;

  FD_ZERO(&infd);
  FD_ZERO(&outfd);
  FD_ZERO(&errfd);

  FD_SET(sockfd, &errfd); /* always check for error */

  if(for_recv)
  {
    FD_SET(sockfd, &infd);
  }
  else
  {
    FD_SET(sockfd, &outfd);
  }

  /* select() returns the number of signalled sockets or -1 */
  res = select(sockfd + 1, &infd, &outfd, &errfd, &tv);
  return res;
}

Meow::Core::Tools::CurlException::CurlException()
{
    ClassName="CurlException";
}
Meow::Core::Tools::CurlException::~CurlException()
{
}
Meow::Core::Tools::Curl::Curl( std::string url)
{
	
	SetMeowClassName("Core/Tools/Curl");
	curl=curl_easy_init();
	formpost=NULL;
    lastptr=NULL;
    headerlist=NULL;
	if ( curl )
	{
		curl_easy_setopt(curl,CURLOPT_URL,url.c_str());
		curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,1L);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
	}
}
Meow::Core::Tools::Curl::Curl()
{
	SetMeowClassName("Core/Tools/Curl");

	curl=curl_easy_init();
	formpost=NULL;
    lastptr=NULL;
    headerlist=NULL;
	if ( formpost!=NULL )
		curl_formfree(formpost);
	if ( headerlist!=NULL )
		curl_slist_free_all(headerlist);
}
Meow::Core::Tools::Curl::~Curl()
{
	if ( curl )
		curl_easy_cleanup(curl);
}
void Meow::Core::Tools::Curl::FormAdd( std::string name, std::string value )
{
	curl_formadd(&formpost,
                 &lastptr,
                 CURLFORM_COPYNAME, name.c_str(),
                 CURLFORM_FILE,value.c_str(),
                 CURLFORM_END);
}
void Meow::Core::Tools::Curl::Perform( ) throw (Meow::Core::Tools::CurlException)
{
	if ( curl )
	{
		curl_easy_setopt(curl,CURLOPT_USERAGENT, "MEOW-curl/1.1");
		
		if ( headerlist!=NULL )
			curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
		if ( formpost!=NULL )
			curl_easy_setopt(curl,CURLOPT_HTTPPOST,formpost);
		Core::Log::GetLog()->Warning("Meow::Core::Tools::Curl::Perform","s","Calling curl_easy_perform");
		CURLcode res=curl_easy_perform(curl);
		if ( res != CURLE_OK )
		{
			Core::Log::GetLog()->Warning("Meow::Core::Tools::Curl::Perform","ss","Response code received from curl_easy_perform:",curl_easy_strerror(res));
			CurlException e;
			e.SetMessage(curl_easy_strerror(res));
			throw e;
		}
	}
	else
	{
		CurlException e;
		e.SetMessage("Not connected.");
		throw e;
	}
}
std::string Meow::Core::Tools::Curl::SendAndReceive( std::string buffer ) throw (Meow::Core::Tools::CurlException)
{
	std::string response="";
	if ( curl )
	{
		size_t bytessent;

		curl_socket_t sockfd; /* socket */
	    long sockextr;
		size_t iolen;
  		curl_off_t nread;
		Core::Log::GetLog()->Warning("Meow::Core::Tools::Curl::SendAndReceive","s","Calling curl_easy_getinfo");
		curl_easy_setopt(curl,CURLOPT_USERAGENT, "MEOW-curl/1.1");
		
		CURLcode res = curl_easy_getinfo(curl, CURLINFO_LASTSOCKET, &sockextr);
	    if ( res != CURLE_OK )
		{
			Core::Log::GetLog()->Warning("Meow::Core::Tools::Curl::Send","ss","Response code received from curl_easy_getinfo:",curl_easy_strerror(res));
			CurlException e;
			e.SetMessage(curl_easy_strerror(res));
			throw e;
		}
		sockfd = sockextr;

		Core::Log::GetLog()->Warning("Meow::Core::Tools::Curl::SendAndReceive","s","Calling wait_on_socket");
		wait_on_socket(sockfd, 0, 60000L);
		Core::Log::GetLog()->Warning("Meow::Core::Tools::Curl::SendAndReceive","s","Calling curl_easy_send");
		cout << buffer << endl;
		res=curl_easy_send(curl,buffer.c_str(),buffer.length(),&bytessent);
		if ( res != CURLE_OK )
		{
			Core::Log::GetLog()->Warning("Meow::Core::Tools::Curl::Send","ss","Response code received from curl_easy_send:",curl_easy_strerror(res));
			CurlException e;
			e.SetMessage(curl_easy_strerror(res));
			throw e;
		}
		Core::Log::GetLog()->Warning("Meow::Core::Tools::Curl::SendAndReceive","s","Calling wait_on_socket");
		wait_on_socket(sockfd, 1, 60000L);
      		
		while (true)
   		{
      		char buf[1024]={0};
			CURLcode res = curl_easy_recv(curl, buf, 1024, &iolen);

      		if(CURLE_OK != res)
			{
				Core::Log::GetLog()->Warning("Meow::Core::Tools::Curl::WaitAndReceive","ss","Response code received from curl_easy_recv:",curl_easy_strerror(res));
        		break;
			}
			nread = (curl_off_t)iolen;
			
      		response.append(buf);
    	}
	}
	else
	{
		CurlException e;
		e.SetMessage("Not connected.");
		throw e;
	}
	Core::Log::GetLog()->Warning("Meow::Core::Tools::Curl::SendAndReceive","s","Done");
	return response;
}
void Meow::Core::Tools::Curl::ConnectOnly()
{
	if ( curl ) curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
}
void Meow::Core::Tools::Curl::SkipPeerVerification()
{
	if ( curl ) curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
}
void Meow::Core::Tools::Curl::SkipHostnameVerification()
{
	if ( curl ) curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
}
void Meow::Core::Tools::Curl::HeaderAdd( std::string header )
{
	if ( curl ) headerlist=curl_slist_append(headerlist,header.c_str());
}
 
struct WriteThis {
  const char *readptr;
  long sizeleft;
  long sizeread;
};
struct MemoryStruct {
  char *memory;
  size_t size;
};
 
static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userp)
{
  struct WriteThis *pooh = (struct WriteThis *)userp;
  if(size*nmemb < 1)
    return 0;
 
  size_t copy_size=size*nmemb;
  if ( copy_size > pooh->sizeleft )
	 copy_size=pooh->sizeleft;

  if ( copy_size > 0 )
  {
  	 memcpy(ptr,&pooh->readptr[pooh->sizeread],copy_size);
	 cout << "Copying:" << copy_size << " bytes" << endl;
  }
  pooh->sizeread+=copy_size;
  pooh->sizeleft-=copy_size;
  return copy_size;                          /* no more data left to deliver */ 
}

static size_t
write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
  cout << "write_callback " << size << " bytes" << endl;
  mem->memory = (char*)realloc(mem->memory, mem->size + realsize + 1);
  if(mem->memory == NULL) {
    Meow::Core::Log::GetLog()->Warning("WriteMemoryCallback","s","Not enough memory");
    return 0;
  }
  
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}

std::string Meow::Core::Tools::Curl::ConformantPost( std::string buffer, std::string contenttype )
	throw (Meow::Core::Tools::CurlException)
{
	std::string data;		
	if ( curl )
	{
		CURLcode res;
		// curl_easy_setopt(curl,CURLOPT_POST,1L);
		curl_easy_setopt(curl,CURLOPT_UPLOAD,1L);
	//	curl_easy_setopt(curl,CURLOPT_HTTP_VERSION,CURL_HTTP_VERSION_1_0);
		curl_easy_setopt(curl,CURLOPT_READFUNCTION,read_callback);
		curl_easy_setopt(curl,CURLOPT_VERBOSE,1L);
 		curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION, write_callback);
 		// curl_easy_setopt(curl,CURLOPT_RETURN_TRANSFER,1L);

		struct WriteThis dataStructure;
		struct MemoryStruct chunk;

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk);
		curl_easy_setopt(curl, CURLOPT_READDATA, &dataStructure);
		chunk.memory = (char*)malloc(1);  
		chunk.size = 0;
		dataStructure.readptr=buffer.c_str();
		dataStructure.sizeread=0;
		dataStructure.sizeleft=buffer.length();
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE_LARGE, dataStructure.sizeleft);
		HeaderAdd("Content-Type: " + contenttype);
		// HeaderAdd("Content-Length: " + Core::Types::ValueType::IntegerToString(buffer.length()));
		if ( headerlist!=NULL )
			curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headerlist);
		curl_easy_setopt(curl,CURLOPT_USERAGENT, "MEOW-curl");
		res = curl_easy_perform(curl);
		data=chunk.memory;
		free(chunk.memory);
	}
	else
	{
		CurlException e;
		e.SetMessage("Not connected.");
		throw e;
	}
	return data;
}
