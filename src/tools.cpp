#include <tools.h>



String result_toASCII(byte msg[], short array_size)
{

    return result_toString(msg,pos_data,array_size - 1);
}

/**
 * byte array
 * offset start position
 * length of array
 */
String result_toString(byte msg[],short offset, short length)
{
	String response;

    for (int k = offset; k < length; k++) //ignore headers and checksum
    {
      response += String(msg[k]);
    }
    return response;
}
