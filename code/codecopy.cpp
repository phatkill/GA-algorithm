#include <bits/stdc++.h>
using namespace std;
int SIZE = 200, generation = 500, value = 30;
int generationNow = 0;
double mutationRate = 0.1;
double  crossover = 0.25;// rate mix
int numberNode = 4;

random_device rd;
mt19937 gen(rd());
vector<double> fitness;
vector<double> P; // probability
vector<double> C; // cumulative
vector<double> R;
double total = 0;

int random(int a, int b)
{
    uniform_int_distribution<int> dist(a, b);
    return dist(gen);
}

double random(double a, double b)
{
    uniform_real_distribution<double> dist(a, b);
    return dist(gen);
}

struct Chromosome
{
    int a, b, c, d;
};
vector<Chromosome> chromosomes;

// //--------------------------- note --------------------
// void inputR(){
//         // random
//     for (int i = 0; i < SIZE; ++i){
//         cin >> R[i];
//     }
// }

// void inputC(){
//     for (int i = 0; i < 3; ++i)
//     {
//         if (i == 2) C[i] = 1;
//     }
// }
// //---------------------------------------------------------



// step 1
void initialize()
{
    for (int i = 0; i < SIZE; ++i)
    {
        chromosomes[i].a = random(0, value);
        chromosomes[i].b = random(0, value);
        chromosomes[i].c = random(0, value);
        chromosomes[i].d = random(0, value);
    }
}

// step 2
void evaluation()
{
    for (int i = 0; i < SIZE; ++i)
    {
        fitness[i] = (double)(1) / (1 + abs(chromosomes[i].a + 2 * chromosomes[i].b + 3 * chromosomes[i].c + 4 * chromosomes[i].d - value));
    }
}

void randomAllChromosome(){
        for (int i = 0; i < SIZE; ++i){
        R[i] = random(0.0, 1.0);
    }
}

void mix(vector<Chromosome> &newChromosomes, int u, int v, double g){
    int id = (int)g;
    for (int i = id + 1; i < 4; ++i){
        if (i == 1) newChromosomes[u].b = chromosomes[v].b;
        if (i == 2) newChromosomes[u].c = chromosomes[v].c;
        if (i == 3) newChromosomes[u].d = chromosomes[v].d;
    }
}
// step 3 and 4
void selection(){
    for (int i = 0; i < SIZE; ++i){
        total += fitness[i];
    }
    for (int i = 0; i < SIZE; ++i){
        P[i] = fitness[i] / total;
        C[i] = (i == 0) ? P[i] : C[i - 1] + P[i];
    }

    // randomAllChromosome();
    vector<Chromosome> newChromosomes(SIZE);
    for (int i = 0; i < SIZE; ++i){
        int range = 0;
        for (int j = 0; j < SIZE; ++j){
            range = j;
            if (C[j] >= R[i]) {
                break;
            }
        }
        newChromosomes[i] = chromosomes[range];
    }

    chromosomes = newChromosomes;

    // inputR(); // need delete when don't use
    randomAllChromosome();
    vector<int> selectChromosomes;
    for (int i = 0; i < SIZE; ++i){
        if (R[i] < crossover) selectChromosomes.push_back(i);
    }
    long long sizeNeedMix = selectChromosomes.size();
    sizeNeedMix = sizeNeedMix * (sizeNeedMix - 1) / 2;
    //C will now only be a mix position.
    C.resize(sizeNeedMix);
    sizeNeedMix = selectChromosomes.size();

    // We will reuse newchromosome
    for (int i = 0; i < selectChromosomes.size(); ++i){
        C[i] = random(1, 3) - 1;
    }
    // Mix : example 1, 4, 5 -> 1, 4; 4, 5; 5, 1
    newChromosomes = chromosomes;
    for (int i = 0; i < (int)C.size(); ++i){
        mix(newChromosomes, selectChromosomes[i % sizeNeedMix], selectChromosomes[(i + 1) % sizeNeedMix], C[i]);
    }
    chromosomes = newChromosomes;
}

//step 5: mutation
void mutation(){
    int totalGen = numberNode * SIZE;
    int numberOfMutation = (int)(mutationRate * totalGen);
    vector<int> chromosomesHaveMutation(numberOfMutation);
    for (int i = 0; i < numberOfMutation; ++i) chromosomesHaveMutation[i] = random(1, SIZE * numberNode) - 1;
    for (int i = 0; i < (int)chromosomesHaveMutation.size(); ++i){
        int number = chromosomesHaveMutation[i];
        int pos = number / 4;
        int j = number % 4;
        if (j == 0) chromosomes[pos].a = random(0, value);
        if (j == 1) chromosomes[pos].b = random(0, value);
        if (j == 2) chromosomes[pos].c = random(0, value);
        if (j == 3) chromosomes[pos].d = random(0, value);
    }
}

void check(){
    
    for (int i = 0; i < SIZE; ++i){
        if (abs(fitness[i]) != 1) continue;
        cout << " you success with " << generationNow  << " iteration" << endl;
        cout << "this is the Chromosome satisfied: " << chromosomes[i].a << " " << chromosomes[i].b << " " << chromosomes[i].c << " " << chromosomes[i].d << endl;

        exit(0);
    }
}

// -------------------------- note --------------------
// void display(){

//     for (int i = 0; i < SIZE; ++i){
//         cout << "Chrmosome " << i + 1 << ": " << chromosomes[i].a << " " << chromosomes[i].b << " " << chromosomes[i].c << " " << chromosomes[i].d << endl;
//     }

//     for (int i = 0; i < SIZE; ++i){
//         cout <<"fitness " << i << " " << fitness[i]  << endl;
//     }

//     cout << "total: " << total << endl;

//     cout << "P: ";

//     for (int i = 0; i < SIZE; ++i){
//         cout << P[i] << " ";
//     }
//     cout << endl;

//     cout << "C: ";
//     for (int i = 0; i < SIZE; ++i) cout << C[i] << " ";
//     cout << endl;

//     cout << "R: ";
//     for (int i = 0; i < SIZE; ++i) cout << R[i] << " ";
//     cout << endl;
// }
// void input(){
//     for (int i  = 0; i < SIZE; ++i)
//     {
//         cin >> chromosomes[i].a >> chromosomes[i].b >> chromosomes[i].c >> chromosomes[i].d;
//     }
//     // random
//     for (int i = 0; i < SIZE; ++i){
//         cin >> R[i];
//     }
    
// }
//-------------------------



void handel(){
    chromosomes.resize(SIZE);
    fitness.resize(SIZE);
    C.resize(SIZE);
    R.resize(SIZE);
    P.resize(SIZE);
    generationNow++;//
    initialize();
    while (generationNow <= generation){
        evaluation();
        selection();
        mutation();
        evaluation();
        check();
        generationNow++;
    }
    cout << "sorry, i'm not found it!";
}

int main()
{
    handel();
}
