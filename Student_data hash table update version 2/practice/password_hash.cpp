#include<iostream>
#include<iomanip>
#include<openssl/evp.h>
using namespace std;

int main(){
    string password = "Saifullah123";
    string salt = "X9@Random";
    unsigned char hash[32];
    PKCS5_PBKDF2_HMAC(password.c_str(),password.length(),(unsigned char*)salt.c_str(),salt.length(),10000,EVP_sha256(),32,hash);
    for(int i = 0; i < 32; i++)
       cout<<hex<<setw(2)<<setfill('0')<<(int)hash[i];
}