#include "wirelessXbee.h"

void writeXbee(std::string str, std::string& msg_out, std::string addresslow){
    //std::cout << str<<'\n';
    //std::cout << str.length()<< '\n';
    //hexstr = str + "_OUT";

    std::string checksum;
    int l;

    //0 byte
    msg_out = msg_out + '7'; // Start Delimeter
    msg_out = msg_out + 'E'; // Start Delimeter
    //1 byte
    msg_out = msg_out + '0'; // Plceholder length
    msg_out = msg_out + '0'; // Plceholder length
    //2 byte placeholder - at the end will be calculated the lenght in hex
    msg_out = msg_out + '0'; // Plceholder length
    msg_out = msg_out + '0'; /// Plceholder length
    //3 byte
    msg_out = msg_out + '1'; // Frame Type
    msg_out = msg_out + '0'; // Frame Type
    //4 byte
    msg_out = msg_out + '0'; //Frame ID
    msg_out = msg_out + '1'; //Frame ID
    //5 byte
    msg_out = msg_out + '0'; //Address HIGH
    msg_out = msg_out + '0'; //Address HIGH
    //6 byte
    msg_out = msg_out + '1'; //Address HIGH
    msg_out = msg_out + '3'; //Address HIGH
    //7 byte
    msg_out = msg_out + 'A'; //Address HIGH
    msg_out = msg_out + '2'; //Address HIGH
    //8 byte
    msg_out = msg_out + '0'; //Address HIGH
    msg_out = msg_out + '0'; //Address HIGH
    
    //from 9 to 12 byte - Address LOW
    for(int i = 0; i<addresslow.length(); i++){
        msg_out = msg_out + addresslow[i];
    }

    //13 byte
    msg_out = msg_out + 'F'; //16 Bit Destination address
    msg_out = msg_out + 'F'; //16 Bit Destination address
    //14 byte
    msg_out = msg_out + 'F'; //Broadcast Radius + Options
    msg_out = msg_out + 'E'; //Broadcast Radius + Options
    //15 byte
    msg_out = msg_out + '0'; // Start Delimeter
    msg_out = msg_out + '0'; // Start Delimeter
    //16 byte
    msg_out = msg_out + '0'; // Start Delimeter
    msg_out = msg_out + '0'; // Start Delimeter
    
   //message byte
    msg_out = msg_out + str; // Start Delimeter
    
    //Checksum Calculation
    for(int i = 6; i<34 + str.length() ; i+=2 ){
        checksum = hexadd(checksum,msg_out.substr(i, 2));//checksum        
    }
    std::cout << checksum<<'\n';
    if (checksum.length()>2){
        checksum = checksum.substr(1,2); 
    }else{
        checksum = checksum.substr(0,2); 
    }
    std::cout << checksum<<'\n';
    
    checksum = hexand(checksum, "FF");
    std::cout << checksum<<'\n';
    checksum = hexmin("FF" , checksum);

    if (checksum.length()<2){
        checksum.insert(0, "0");
    }
    std::cout << checksum<<'\n';

    msg_out = msg_out + checksum; // Checksum
    
    //Length calculation
    l = (msg_out.length()-8)/2;
    //std::cout << typeid(l).name()<<'\n';
    std::stringstream stream;
    stream << std::uppercase << std::hex << l;
    std::string result( stream.str() );
    //std::cout << result <<'\n';

    msg_out[4] = result[0]; // Change with right length
    msg_out[5] = result[1]; // Change with right length
}


// Convert string of chars to its representative string of hex numbers
void stream2hex(const std::string str, std::string& hexstr, bool capital)
{
    hexstr.resize(str.size() * 2);
    const size_t a = capital ? 'A' - 1 : 'a' - 1;

    for (size_t i = 0, c = str[0] & 0xFF; i < hexstr.size(); c = str[i / 2] & 0xFF)
    {

        hexstr[i++] = c > 0x9F ? (c / 16 - 9) | a : c / 16 | '0';
        hexstr[i++] = (c & 0xF) > 9 ? (c % 16 - 9) | a : c % 16 | '0';
    }
}


// Convert string of hex numbers to its equivalent char-stream
void hex2stream(const std::string hexstr, std::string& str)
{
    str.resize((hexstr.size() + 1) / 2);

    for (size_t i = 0, j = 0; i < str.size(); i++, j++)
    {
        str[i] = (hexstr[j] & '@' ? hexstr[j] + 9 : hexstr[j]) << 4, j++;
        str[i] |= (hexstr[j] & '@' ? hexstr[j] + 9 : hexstr[j]) & 0xF;
    }
}


std::string hexadd(const std::string & hex1, const std::string & hex2)
{
	long n1, n2;
	n1 = hex2dec(hex1);
	n2 = hex2dec(hex2);
	return dec2hex(n1+n2);
}

std::string hexmin(const std::string & hex1, const std::string & hex2)
{
	long n1, n2;
	n1 = hex2dec(hex1);
	n2 = hex2dec(hex2);
	return dec2hex(n1-n2);
}

std::string hexand(const std::string & hex1, const std::string & hex2)
{
	long n1, n2;
	n1 = hex2dec(hex1);
	n2 = hex2dec(hex2);
	return dec2hex(n1&n2);
}

std::string dec2hex(long i)
{
	std::stringstream ss;
	std::string s;
	hex(ss);
	uppercase(ss);
	ss << i;
	ss >> s;
	return s;
}

long hex2dec(const std::string & hexstr)
{
	std::stringstream ss;
	long i = 0;
	hex(ss);
	ss << hexstr;
	ss.clear();
	ss >> i;
	return i;
}
