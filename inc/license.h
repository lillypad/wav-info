#include <iostream>
#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

//function prototype, main() needs this otherwise it won't know what encryptstr function is
//youre saying that there's an encryptstr function in the program that you will define later on
//and you will be passing a character array into it

void encryptstr(char []);
void aesencryptstr(char []);
/*
int main()
{
	char mystring [] = "Hello World!";
	cout << "Original string: " << mystring << endl;

	//encrypt	
	encryptstr(mystring);	

	cout << "Encrypted string: " << mystring << endl;

	//decrypt using the SAME FUNCTION
	encryptstr(mystring);

	cout << "Decrypted string: " << mystring << endl;
    system("pause");
	return 0;
}
*/

string computerID(){
       HW_PROFILE_INFO hwProfileInfo;
       if (GetCurrentHwProfileA(&hwProfileInfo) != 0){
          string computerID = hwProfileInfo.szHwProfileGuid;
          computerID.erase(0, 1);
          computerID = computerID.substr(0, computerID.size()-1);
          return computerID;
          }else{
          return "FAILED";
          }
}

std::string GetRegistry(const char* StringName)
{
DWORD dwType = REG_SZ;
HKEY hKey = 0;

char value[1024];
DWORD value_length = 1024;
const char* subkey = "Software\\WAV_INFO\\";
RegOpenKey(HKEY_CURRENT_USER,subkey,&hKey);
RegQueryValueEx(hKey, StringName, NULL, &dwType, (LPBYTE)&value, &value_length);
return  std::string(value);
}


//encrypt string
void encryptstr(char str[]){

for( int i=0; str[i] != '\0'; i++ ){
	if(str[i] >= 'a' && str[i] <= 'm'){
		str[i] += 13;
	}

	else if(str[i] > 'm' && str[i] <= 'z'){
		str[i] -= 13;
	}

	else if (str[i] >= 'A' && str[i] <= 'M'){
		str[i] += 13;
	}

	else if(str[i] > 'M' && str[i] <= 'Z'){
		str[i] -= 13;
	}
	else if (str[i] >= '0' && str[i] <= '5'){
         str[i] += 5;
    }
    else if (str[i] <= '9' && str[i] > '5'){
         str[i] -= 5;
    }

}
}

//abcdefghijklmnopqrstuvwxyz
//bslfchygjtkzuxopnqemviawrd

//ABCDEFGHIJKLMNOPQRSTUVWXYZ
//XUBFLIDYKJOWENRAZMPSGCTQVH
//Wjzzrzjzzrlgbzbvfjo
void aesencryptstr(char str[]){
//Lower Case
for( int i=0; str[i] != '\0'; i++ ){
	if(str[i] == 'a'){str[i] = 'b';}
	else if(str[i] == 'b'){str[i] = 's';}
	else if(str[i] == 'c'){str[i] = 'l';}
	else if(str[i] == 'd'){str[i] = 'f';}
	else if(str[i] == 'e'){str[i] = 'c';}
	else if(str[i] == 'f'){str[i] = 'h';}
	else if(str[i] == 'g'){str[i] = 'y';}
	else if(str[i] == 'h'){str[i] = 'g';}
	else if(str[i] == 'i'){str[i] = 'j';}
	else if(str[i] == 'j'){str[i] = 't';}
	else if(str[i] == 'k'){str[i] = 'k';}
	else if(str[i] == 'l'){str[i] = 'z';}
	else if(str[i] == 'm'){str[i] = 'u';}
	else if(str[i] == 'n'){str[i] = 'x';}
	else if(str[i] == 'o'){str[i] = 'o';}
	else if(str[i] == 'p'){str[i] = 'p';}
	else if(str[i] == 'q'){str[i] = 'n';}
	else if(str[i] == 'r'){str[i] = 'q';}
	else if(str[i] == 's'){str[i] = 'e';}
	else if(str[i] == 't'){str[i] = 'm';}
	else if(str[i] == 'u'){str[i] = 'v';}
	else if(str[i] == 'v'){str[i] = 'i';}
	else if(str[i] == 'w'){str[i] = 'a';}
	else if(str[i] == 'x'){str[i] = 'w';}
	else if(str[i] == 'y'){str[i] = 'r';}
	else if(str[i] == 'z'){str[i] = 'd';}
	
	//Space
	else if(str[i] == ' '){str[i] = '-';}
	
	//Upper Case
	else if(str[i] == 'A'){str[i] = 'X';}
	else if(str[i] == 'B'){str[i] = 'U';}
	else if(str[i] == 'C'){str[i] = 'B';}
	else if(str[i] == 'D'){str[i] = 'F';}
	else if(str[i] == 'E'){str[i] = 'L';}
	else if(str[i] == 'F'){str[i] = 'I';}
	else if(str[i] == 'G'){str[i] = 'D';}
	else if(str[i] == 'H'){str[i] = 'Y';}
	else if(str[i] == 'I'){str[i] = 'K';}
	else if(str[i] == 'J'){str[i] = 'J';}
	else if(str[i] == 'K'){str[i] = 'O';}
	else if(str[i] == 'L'){str[i] = 'W';}
	else if(str[i] == 'M'){str[i] = 'E';}
	else if(str[i] == 'N'){str[i] = 'N';}
	else if(str[i] == 'O'){str[i] = 'R';}
	else if(str[i] == 'P'){str[i] = 'A';}
	else if(str[i] == 'Q'){str[i] = 'Z';}
	else if(str[i] == 'R'){str[i] = 'M';}
	else if(str[i] == 'S'){str[i] = 'P';}
	else if(str[i] == 'T'){str[i] = 'S';}
	else if(str[i] == 'U'){str[i] = 'G';}
	else if(str[i] == 'V'){str[i] = 'C';}
	else if(str[i] == 'W'){str[i] = 'T';}
	else if(str[i] == 'X'){str[i] = 'Q';}
	else if(str[i] == 'Y'){str[i] = 'V';}
	else if(str[i] == 'Z'){str[i] = 'H';}
	
	//Digits
	else if(str[i] == '0'){str[i] = '4';}
	else if(str[i] == '1'){str[i] = '5';}
	else if(str[i] == '2'){str[i] = '9';}
	else if(str[i] == '3'){str[i] = '1';}
	else if(str[i] == '4'){str[i] = '0';}
	else if(str[i] == '5'){str[i] = '6';}
	else if(str[i] == '6'){str[i] = '8';}
	else if(str[i] == '7'){str[i] = '2';}
	else if(str[i] == '8'){str[i] = '7';}
	else if(str[i] == '9'){str[i] = '3';}
	/*
	//Code to remove spaces
	for (int i = 0; i < array_size; i++) // Loop to find the item to delete.
    {
         if (array[i] == item_to_delete) // If we find the item to delete...
         {
              for (int j = i; j < array_size - 1; j++) // Iterate through the remaining elements, stopping one before the end.
                   {
                         array[j] = array[j + 1]; // Overwrite the current element with the next. This effectively deletes the item to delete, and moves everything else down one.
                   }
                   array[array_size - 1] = NULL; // Set the last item in the array to null, (or some other appropriate null value). This may not necessarily be needed, but is good practice.
                   array_size--; // Reduce the array size by one.
                   break; // Exit out of the 'find item to delete' loop.    
         }
    }
*/
}

}

void aesdecryptstr(char str[]){
//Lower Case
for( int i=0; str[i] != '\0'; i++ ){
	if(str[i] == 'b'){str[i] = 'a';}
	else if(str[i] == 's'){str[i] = 'b';}
	else if(str[i] == 'l'){str[i] = 'c';}
	else if(str[i] == 'f'){str[i] = 'd';}
	else if(str[i] == 'c'){str[i] = 'e';}
	else if(str[i] == 'h'){str[i] = 'f';}
	else if(str[i] == 'y'){str[i] = 'g';}
	else if(str[i] == 'g'){str[i] = 'h';}
	else if(str[i] == 'j'){str[i] = 'i';}
	else if(str[i] == 't'){str[i] = 'j';}
	else if(str[i] == 'k'){str[i] = 'k';}
	else if(str[i] == 'z'){str[i] = 'l';}
	else if(str[i] == 'u'){str[i] = 'm';}
	else if(str[i] == 'x'){str[i] = 'n';}
	else if(str[i] == 'o'){str[i] = 'o';}
	else if(str[i] == 'p'){str[i] = 'p';}
	else if(str[i] == 'n'){str[i] = 'q';}
	else if(str[i] == 'q'){str[i] = 'r';}
	else if(str[i] == 'e'){str[i] = 's';}
	else if(str[i] == 'm'){str[i] = 't';}
	else if(str[i] == 'v'){str[i] = 'u';}
	else if(str[i] == 'i'){str[i] = 'v';}
	else if(str[i] == 'a'){str[i] = 'w';}
	else if(str[i] == 'w'){str[i] = 'x';}
	else if(str[i] == 'r'){str[i] = 'y';}
	else if(str[i] == 'd'){str[i] = 'z';}
	
	//Space
	else if(str[i] == '-'){str[i] = ' ';}
	
	//Upper Case
	else if(str[i] == 'X'){str[i] = 'A';}
	else if(str[i] == 'U'){str[i] = 'B';}
	else if(str[i] == 'B'){str[i] = 'C';}
	else if(str[i] == 'F'){str[i] = 'D';}
	else if(str[i] == 'L'){str[i] = 'E';}
	else if(str[i] == 'I'){str[i] = 'F';}
	else if(str[i] == 'D'){str[i] = 'G';}
	else if(str[i] == 'Y'){str[i] = 'H';}
	else if(str[i] == 'K'){str[i] = 'I';}
	else if(str[i] == 'J'){str[i] = 'J';}
	else if(str[i] == 'O'){str[i] = 'K';}
	else if(str[i] == 'W'){str[i] = 'L';}
	else if(str[i] == 'E'){str[i] = 'M';}
	else if(str[i] == 'N'){str[i] = 'N';}
	else if(str[i] == 'R'){str[i] = 'O';}
	else if(str[i] == 'A'){str[i] = 'P';}
	else if(str[i] == 'Z'){str[i] = 'Q';}
	else if(str[i] == 'M'){str[i] = 'R';}
	else if(str[i] == 'P'){str[i] = 'S';}
	else if(str[i] == 'S'){str[i] = 'T';}
	else if(str[i] == 'G'){str[i] = 'U';}
	else if(str[i] == 'C'){str[i] = 'V';}
	else if(str[i] == 'T'){str[i] = 'W';}
	else if(str[i] == 'Q'){str[i] = 'X';}
	else if(str[i] == 'V'){str[i] = 'Y';}
	else if(str[i] == 'H'){str[i] = 'Z';}
	
	//Digits
	else if(str[i] == '4'){str[i] = '0';}
	else if(str[i] == '5'){str[i] = '1';}
	else if(str[i] == '9'){str[i] = '2';}
	else if(str[i] == '1'){str[i] = '3';}
	else if(str[i] == '0'){str[i] = '4';}
	else if(str[i] == '6'){str[i] = '5';}
	else if(str[i] == '8'){str[i] = '6';}
	else if(str[i] == '2'){str[i] = '7';}
	else if(str[i] == '7'){str[i] = '8';}
	else if(str[i] == '3'){str[i] = '9';}
}

}
