#include <Core/Peers/Base64.h>
#include <stdio.h>
#include <string.h>
Meow::Core::Peers::Base64::Base64()
{
	SetMeowClassName("Core/Peers/Base64");
}
Meow::Core::Peers::Base64::~Base64()
{
}
int Meow::Core::Peers::Base64::isBase64(char c)
{
	if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
	            (c >= '0' && c <= '9') || (c == '+')    ||
	            (c == '/')             || (c == '='))
	{
	        return true;
    }
	return false;
}
char Meow::Core::Peers::Base64::encode(unsigned char u)
{
	 if(u < 26)  return 'A'+u;
	 if(u < 52)  return 'a'+(u-26);
	 if(u < 62)  return '0'+(u-52);
	 if(u == 62) return '+';
	 return '/';
}
unsigned char Meow::Core::Peers::Base64::decode(char c)
{
    if(c >= 'A' && c <= 'Z') return(c - 'A');
    if(c >= 'a' && c <= 'z') return(c - 'a' + 26);
    if(c >= '0' && c <= '9') return(c - '0' + 52);
    if(c == '+')             return 62;

    return 63;
}
char * Meow::Core::Peers::Base64::encodeBase64(int size, unsigned char *src)
{
	 int i;
	    char *out, *p;

	    if(!src)
	        return NULL;

	    if(!size)
	        size=strlen((char *)src);

	    out= (char*)calloc(sizeof(char), size*4);

	    p= out;
		
	    for(i=0; i<size; i+=3) {

	        unsigned char b1=0, b2=0, b3=0, b4=0, b5=0, b6=0, b7=0;

	        b1 = src[i];

	        if(i+1<size)
	            b2 = src[i+1];

	        if(i+2<size)
	            b3 = src[i+2];

	        b4= b1>>2;
	        b5= ((b1&0x3)<<4)|(b2>>4);
	        b6= ((b2&0xf)<<2)|(b3>>6);
	        b7= b3&0x3f;

	        *p++= encode(b4);
	        *p++= encode(b5);

	        if(i+1<size) {
	            *p++= encode(b6);
	        } else {
	            *p++= '=';
	        }
	        if(i+2<size) {
	            *p++= encode(b7);
	        } else {
	            *p++= '=';
	        }
	    }
	    return out;
}
int Meow::Core::Peers::Base64::decodeBase64(unsigned char *dest, const char *src)
{
	 if(src && *src) {

	        unsigned char *p= dest;
	        int k, l= strlen(src)+1;
	        unsigned char *buf= (unsigned char*)calloc(sizeof(unsigned char), l);


	        /* Ignore non base64 chars as per the POSIX standard */
	        for(k=0, l=0; src[k]; k++) {

	            if(isBase64(src[k])) {

	                buf[l++]= src[k];

	            }

	        }

	        for(k=0; k<l; k+=4) {

	            char c1='A', c2='A', c3='A', c4='A';
	            unsigned char b1=0, b2=0, b3=0, b4=0;

	            c1= buf[k];

	            if(k+1<l) {

	                c2= buf[k+1];

	            }

	            if(k+2<l) {

	                c3= buf[k+2];

	            }

	            if(k+3<l) {

	                c4= buf[k+3];

	            }

	            b1= decode(c1);
	            b2= decode(c2);
	            b3= decode(c3);
	            b4= decode(c4);

	            *p++=((b1<<2)|(b2>>4) );

	            if(c3 != '=') {

	                *p++=(((b2&0xf)<<4)|(b3>>2) );

	            }

	            if(c4 != '=') {

	                *p++=(((b3&0x3)<<6)|b4 );

	            }

	        }

	        free(buf);

	        return(p-dest);

	    }

	    return false;
}
std::string Meow::Core::Peers::Base64::charToHex( char dec )
{
	char dig1 = (dec&0xF0)>>4;
	char dig2 = (dec&0x0F);
	if ( 0<= dig1 && dig1<= 9) dig1+=48;    //0,48inascii
	if (10<= dig1 && dig1<=15) dig1+=97-10; //a,97inascii
	if ( 0<= dig2 && dig2<= 9) dig2+=48;
	if (10<= dig2 && dig2<=15) dig2+=97-10;

	std::string r;
	r.append( &dig1, 1);
	r.append( &dig2, 1);
	return r;
}


