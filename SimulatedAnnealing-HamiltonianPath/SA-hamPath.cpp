/* Simulated Annealing to find a hamiltonian path between two given vertices of a graph
   To compile it in Linux (terminal): g++ -o program SA-hamPath.cpp
   To run it in Linux (terminal): ./program graph1.in
*/

#include<stdlib.h>
#include<fstream>
#include<sstream>
#include<ctime>
#include<cmath>
#include<list>
#include<iostream>
using namespace std;

#define maxVert 50 // define the maximum number of vertices

struct VertexType {
    list<int>Adj; // store adjacencies
    };

// **************************** GENERAL-PURPOSE FUNCTIONS *******************************

// Convert string to integer
int GetStr2Int(string strConvert) {
    int intReturn;
    intReturn = atoi(strConvert.c_str());
    return(intReturn);
}

// Convert integer to string
string GetInt2Str(int intConvert) {
    stringstream out;
    out << intConvert;
    return(out.str());
}

// **************************** WRITING THE OUTPUT FILE *******************************

// Write output file
void writeFile(int S[], int &costS, double &T, double &Tmin, double &TRed){
    string str;
    ofstream myfile; // open input file and points to the next element to be written

    myfile.open ("result.txt", ios::out);
    myfile << "Initial temperature: " << T << "\n";
    myfile << "Final temperature: " << Tmin << "\n";
    myfile << "Factor of temperature reduction: " << TRed << "\n";
    str = "Path: ";
    for (int i=1; i<=costS; i++) str = str + GetInt2Str(S[i]) + " "; // print solution
    myfile << str << " - Cost: "<< costS << "\n";
    myfile.close();
}

// **************************** READING THE INPUT FILE *******************************

// Read the input file storing the vertices and their adjacencies in an adjacency list called Vet
// the variable n is sent by reference (not by value), because it's modified in the method
void readFile(string fileName, VertexType Vet[], int S[], int &n, int &v1, int &v2){
    string line;
    short v, u, i, temp;
    ifstream myfile; // open input file and points to the next element to be read
    myfile.open (fileName.c_str(), ios::in);

    if (myfile.is_open()){
        i = 0;
        while (myfile.good()){
            i++;
            getline(myfile, line);
            if (i==1) n = GetStr2Int(line); // number of vertices of the graph; convert string to integer
            else if (i==2) v1 = GetStr2Int(line); // first vertex of the Hamiltonian path
            else if (i==3) v2 = GetStr2Int(line); // last vertex of the Hamiltonian path
            else if (line.length() > 0){ // read vertices and edges of the graph
                temp = line.find_first_of(":");
                v = GetStr2Int(line.substr(0, temp));
                line = line.substr(temp+1);
                while (line.length() > 1){
                    temp = line.find_first_of(",");
                    u = GetStr2Int(line.substr(0, temp));
                    line = line.substr(temp+1);
                    Vet[v].Adj.push_back(u);
                    Vet[u].Adj.push_back(v);
                }
            }
        }
        myfile.close();
    }
    else cout << "Unable to open file";
}

// **************************** ADJACENCY LIST METHODS *******************************

// Read the adjacency list Vet and "print" its content to the user
void writeGraphList(int n, VertexType Vet[]){
    int u;
    list<int>:: iterator it;

    cout << "\n********** Printing adjacency list **********\n";
    for(int i=1; i<=n; i++){
        cout << "The vertex " << i << " is adjacent to: ";
        for (it=Vet[i].Adj.begin(); it!=Vet[i].Adj.end(); it++){
            u = *it;
            cout << u << ", ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

// **************************** SIMULATED ANNEALING METHODS *******************************

// Print solution of Simulated Annealing
void printSolution(int S[], int cost){
    for (int i=1; i<=cost; i++) 
        cout << S[i] << " ";
    cout << " - Cost: " << cost << "\n";
}

// Verify if vertex is already in the path
bool verificaAdj(int S[], int &costS, int &u){
    short i = 1;
    bool ok = false;

    while ((i<=costS) && (!ok)){
        if (S[i]==u) ok = true; //u is already in the path S
        i++;
    }
    return ok;
}

// Choose randomly an adjacent vertex of v to add to the path; if last=true it can return last vertex
int pickVertex(VertexType Vet[], int &v, int S[], int &costS, int &v2, bool ok){
    int Aux[maxVert], u, j = 0;
    list<int>:: iterator it;

    for (it=Vet[v].Adj.begin(); it!=Vet[v].Adj.end(); it++){
        u = *it;
        if (!verificaAdj(S, costS, u)){
            if (ok){ // if we can take v2
                j++; 
                Aux[j] = u;
            } else if (u!=v2){ // if ok is false, check first if vertex is not v2
                j++; 
                Aux[j] = u;
            } 
        }
    }

    if (j>0){ // v has some adjacent vertex still not in the path
        u = rand() % j + 1; // between 1 and j choose randomly one of the adjacencies of v
        return Aux[u];
    } else return 0;
}

// Construct an initial solution
void constroiSolIn(VertexType Vet[], int S[], int &costS, int &n, int &v1, int &v2){
    int v, u;
    costS = 1;
    S[costS] = v = v1;

    u = pickVertex(Vet, v, S, costS, v2, false); // choose one of the adjacencies of v that is not in the path
    while (u != 0){ // pickVertex returns 0 if there is not more adjacencies that can be added to the path
        costS++; 
        S[costS] = u; 
        v = u;
        u = pickVertex(Vet, v, S, costS, v2, false);
    }

    u = pickVertex(Vet, S[costS], S, costS, v2, true);
    if (u==v2){
        costS++; 
        S[costS] = u; 
    }

    cout << "Initial solution: ";
    printSolution(S, costS);
}

// Improve solution
void improveSol(VertexType Vet[], int S[], int &costS, int SAux[], int &costSAux, int &n, int &v2){
    int v, u;
    
    v = rand() % (costS-1) + 1; // choose a vertex in the path from which to try another choice of vertices
    for (int j=1; j<=costS; j++){ // remove vertices from position j
        if (j<=v) SAux[j] = S[j]; // from j+1 takes away the rest of the path
        else u = S[j];
    }

    costSAux = v; // size of new path is the point of splitting of the old path
    v = SAux[costSAux]; // take last vertex of new path
    u = pickVertex(Vet, v, SAux, costSAux, v2, false); // check if v has adjacencies that can be added
    
    while (u!=0){
        costSAux++; 
        SAux[costSAux] = u; 
        v = u;
        u = pickVertex(Vet, v, SAux, costSAux, v2, false);
    }
    
    u = pickVertex(Vet, SAux[costSAux], SAux, costSAux, v2, true); // check if v2 is adjacent to the last vertex and add it
    if (u==v2){
        costSAux++; 
        SAux[costSAux] = u; 
    }
}

// Attribute the values of the array SAux to the array S
void replaceSol(int S[], int SAux[], int &costSAux, int &costS){
    for (int i=1; i<=costSAux; i++) S[i] = SAux[i];
    costS = costSAux;
}

// Run Simulated Annealing to search for hamiltonian paths between two vertices
void SA(VertexType Vet[], int S[], int &costS, double &T, double &Tmin, double &TRed, int &MaxIt, int &n, int &v1, int &v2){
    int it, costSAux, delta;
    int SAux[maxVert];
    double r;
    
    while ((T>Tmin) && (costS!=n)) {
        it = 0;
        while ((it<MaxIt) && (costS<n)) { // number of iterations
            improveSol(Vet, S, costS, SAux, costSAux, n, v2);
            delta = abs(costS-costSAux);
            r = rand() % 10+1;
            if ((costSAux>costS) || (r/10 < exp(-delta/T)) && (delta!=0)){
                replaceSol(S, SAux, costSAux, costS);
                cout << "Temp.: " << T << " - Solution: ";
                printSolution(S, costS);
            }
            it++;
        }
        T = T * TRed;
    }
}

int main (int argc, char * argv[ ]) {
    string fileName = argv[1]; // it takes the name of the file as a parameter

    VertexType Vet[maxVert];
    double T, Tmin, TRed;
    int MaxIt, costS, n, v1, v2;
    int S[maxVert];
    srand(time(NULL));

    // Set parameters of Simulated Annealing
    T = 1.2;       // initial temperature
    Tmin = 0.1;    // final temperature
    TRed = 0.9;    // factor of temperature reduction
    MaxIt = 20;

    // Calling methods
    readFile(fileName, Vet, S, n, v1, v2);
    writeGraphList(n, Vet);
    constroiSolIn(Vet, S, costS, n, v1, v2);
    SA(Vet, S, costS, T, Tmin, TRed, MaxIt, n, v1, v2);
    writeFile(S, costS, T, Tmin, TRed);
    return 0;
}
