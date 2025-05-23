#include "./hash-library/md5.h"
#include <iostream> 
#include <string>
#include <tuple>

// use b smooth numbers


using std::string, std::cout, std::endl, std::tuple;


string md5_reduced (MD5 md5, string input, int amount){
    string full_output = md5(input);
    return full_output.substr( full_output.length() - amount );
}



tuple <string, string> Floyd (MD5 md5, string initial, int char_amount){
    string tortoise = initial;
    string hare = initial;
    do {
        tortoise = md5_reduced(md5, tortoise, char_amount);
        hare = md5_reduced( md5, md5_reduced(md5, hare, char_amount), char_amount );
    } while (tortoise != hare);
    
    // reset tortoise, find entry point
    tortoise = initial;
    string tortoise_pre;
    string hare_pre;
    do{
        tortoise_pre = tortoise;
        hare_pre = hare;
        tortoise = md5_reduced(md5, tortoise, char_amount);
        hare = md5_reduced(md5, hare, char_amount);
    } while(tortoise != hare);
    // tortoise and hare are the entry point
    return std::make_tuple(tortoise_pre, hare_pre);
}



tuple <string, string> Brent (MD5 md5, string initial, int char_amount){
    int power = 1;
    int lambda = 1;
    string tortoise = initial;
    string hare = md5_reduced(md5, initial, char_amount);
    while (tortoise != hare){
        if (power == lambda){
            tortoise = hare;
            power *= 2;
            lambda = 0;
        }
        hare = md5_reduced(md5, hare, char_amount);
        lambda++;
    }
    tortoise = initial;
    hare = initial;
    for (int i=0; i<lambda; i++){
        hare = md5_reduced(md5, hare, char_amount);
    }
    int mu = 0;
    string tortoise_pre, hare_pre;
    while (tortoise != hare){
        tortoise_pre = tortoise;
        hare_pre = hare;
        tortoise = md5_reduced(md5, tortoise, char_amount);
        hare = md5_reduced(md5, hare, char_amount);
        mu++;
    }
    return std::make_tuple(tortoise_pre, hare_pre);
}






int main()
{
    // create a new hashing object
    MD5 md5;

    // FIX: make it byte buffer!!
    string initial = "initial";


    string x,y;
    int char_amount = 13;
    tie(x,y) = Floyd(md5, initial, char_amount);

    cout << x << endl << y << endl << "------------" << endl;
    cout << md5_reduced(md5, x, char_amount) << endl << md5_reduced(md5, y, char_amount) << endl;

    return 0;
}