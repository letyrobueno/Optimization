/* Recursive method to determine all partitions of an integer number
   To compile it in Linux (terminal): g++ -o program partition.cpp
   To run it in Linux (terminal): ./program
*/

#include<iostream>
#include<sstream>
#include<string>
#include<math.h>
using namespace std;

void Partition(int m, int i, int k, string str){
    string str2;
    while (i<=k){
        if ((m-i) >= i){
            cout << m-i << str << " " << i << endl;
            if (((m-i)-i) >= i){
                stringstream ss;
                ss << i;
                str2.append(str);
                str2.append(" ");
                str2 += ss.str();
                Partition(m-i, i, k, str2);     
            }
            i++;
        } else i = k+1;
    }
}

main(){
    int k, n;
    string str;
    cout << str << endl;
    cout << "Enter with an integer: ";
    cin >> n;
    k = floor(n/2);
    Partition(n, 1, k, str);
}
