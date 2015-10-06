#include <ctype.h>
#include <stdio.h>
#include <string.h>



static int hexit( char c )
{
    if ( c >= '0' && c <= '9' )
        return c - '0';
    if ( c >= 'a' && c <= 'f' )
        return c - 'a' + 10;
    if ( c >= 'A' && c <= 'F' )
        return c - 'A' + 10;
    return 0;       /* shouldn't happen, we're guarded by isxdigit() */
}



static void strdecode( char* to, char* from )
{
    for ( ; *from != '\0'; ++to, ++from )
    {
        if ( from[0] == '%' && isxdigit( from[1] ) && isxdigit( from[2] ) )
        {
            *to = hexit( from[1] ) * 16 + hexit( from[2] );
            from += 2;
        }
        else
            *to = *from;
    }
    *to = '\0';
}


static void strencode( char* to, size_t tosize, const char* from )
{
    int tolen;

    for ( tolen = 0; *from != '\0' && tolen + 4 < tosize; ++from )
    {
        if ( isalnum(*from) || strchr( "/_.-~", *from ) != (char*) 0 )
        {
            *to = *from;
            ++to;
            ++tolen;
        }
        else
        {
            (void) sprintf( to, "%%%02x", (int) *from & 0xff );
            to += 3;
            tolen += 3;
        }
    }
    *to = '\0';
}

int main(int argc, char **argv)
{
    char encoded_url[1024] = "http%3a%2f%2f192.168.109.247%2f%e7%a7%81%e4%ba%ba%e8%ae%a2%e5%88%b6.HD1280%e8%b6%85%e6%b8%85%e5%9b%bd%e8%af%ad%e4%b8%ad%e8%8b%b1%e5%8f%8c%e5%ad%97.mkv";
    char decoded_url[1024] = {};

    strdecode(decoded_url, encoded_url);
    printf("encoded_url:%s\ndecoded_url:%s\n", encoded_url, decoded_url);
    memset(encoded_url, 0, sizeof(encoded_url));
    strencode(encoded_url, sizeof(encoded_url), decoded_url);
    printf("encoded_url:%s\n", encoded_url);



    return 0;
}
