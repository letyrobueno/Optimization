/* Recursive method to determine all permutations of the first n natural numbers
   To compile it in Linux (terminal): g++ -o program permutation.cpp
   To run it in Linux (terminal): ./program
*/

#include<iostream>
#include<sstream>
#include<string>
#include<math.h>
using namespace std;

void PrintPermutations(int A[], string str1, int n, int m, int &count){
    string str2;
    int B[50];
    stringstream ss;

    if ((n==0) && (!str1.empty())){
        cout << str1 << endl;
        count++;
    } else {
        for (int i=1; i<=n; i++){   
            ss.str(std::string());
            ss << A[i];
            str2.clear();
            str2.append(str1);
            str2.append(" ");
            str2 += ss.str();
            for (int j=1; j<=m; j++) 
                B[j] = A[j];

            if (i != n) 
                B[i] = B[n];
            PrintPermutations(B, str2, n-1, m, count);
        }
    }
}

main(){
    int m;
    int count = 0;
    int A[50];

    cout << "Enter with an integer: ";
    cin >> m;
    for (int i=1; i<=m; i++)
        A[i] = i;
    cout << "\n******** Permutations of the first " << m << " numbers ********\n";
    PrintPermutations(A, "", m, m, count);
    cout << count << " Permutations\n";
}
