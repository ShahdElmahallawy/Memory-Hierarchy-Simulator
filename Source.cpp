#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<sstream>
#include<math.h>
#include<cmath>
#include<unordered_map>
#include<map>
#include<iomanip>

using namespace std;

map<int, int> memory;//address, content 

//Inputs
struct cache
{
	int s; //total cache size
	int l; //cahce line size
	int num_of_cycles; // number of cycles(1 -> 10)
    int hit;
    int miss;
    int count = 0;
    bool isHit;
    vector<int> sequence;
    vector<long>index;
    vector<bool> valid;
    vector <pair<int, int> >cacheContent; // tag, content


    //Extra attributes to support separate caches for instructions and data
    vector<char> seqCheck; 
    vector<pair<int, int>> cacheInst; 
    vector<pair<int, int>> cacheData; 
    vector<bool> validInst;
    vector<bool> validData;
    int instS;
    int dataS;

    cache() //construcutor
    {
        s = 0;
        l = 0;
        num_of_cycles = 0;
        hit = 0;
        miss = 0;
        isHit = 0; 
        instS = 0;
        dataS = 0;
    }
};

cache c;//Constructor to deal with the original cache 
cache c_bonus; //Another Constructor to Support separate caches for instructions and data

//read sequence
void read_file(int which_file, string file)
{
    string filename(file);
    ifstream input_file(filename);

    if (!input_file.is_open())
    {
        cout << "Error: Couldn't open the file: " << filename << endl;
        exit(0);
    }
    if (which_file == 0) //if whcih file = 0 , then read the memory file
    {
        string line, word;
        while (!input_file.eof())
        {
            int address, content = 0;
            int i = 0;
            getline(input_file, line);
            stringstream s(line);
            while (getline(s, word, ' '))
            {
                if (i == 0)
                {
                    address = stoi(word);
                    i = 1;
                }
                else
                {
                    content = stoi(word);
                    i = 0;
                }
            }
            memory[address] = content;
        }
    }
    else if (which_file == 1) //if whcih file = 1 , then read the Address Sequence file
    {
        string line;
        while (getline(input_file, line))
        {
            c.sequence.push_back(stoi(line));
        }
    }
    else if (which_file == 2) //if whcih file = 2 , then read the lable sequence file
    {
        string line, word;
        while (!input_file.eof())
        {
            int x1 = 0;
            getline(input_file, line);
            if (line[0] == 'I')
            {
                c_bonus.instS++;
                line.erase(line.begin());
                stringstream sI(line);
                while (getline(sI, word, ' '))
                {
                    x1 = stoi(word);
                }
                c_bonus.sequence.push_back(x1);
                c_bonus.seqCheck.push_back('I');
            }
            else if (line[0] == 'D') 
            {
                c_bonus.dataS++;
                line.erase(line.begin());
                stringstream sD(line);
                while (getline(sD, word, ' '))
                {
                    x1 = stoi(word);
                }
                c_bonus.sequence.push_back(x1);
                c_bonus.seqCheck.push_back('D');
            }
        }
    }
    input_file.close();
}
//output
void print_output(int idx, bool which_cache)
{
    if (which_cache == 0)
    {
        if (c.isHit)
        {
            cout << "This is a hit" << endl;
        }
        else
        {
            cout << "This is a miss" << endl;
        }
        cout << endl;
        //print the content of the cache
        // Valid bitsand tags of all entries
        int i = log2(c.s);
        int d = log2(c.l);
        int t = 32 - (d + i);
        cout << "Index" << '\t' << "Valid" << '\t' << "Tag" << '\t' << '\t' << '\t' << '\t' << "Content" << endl;
        for (int j = 0; j < c.s; j++)
        {
            cout << setfill('0') << setw(i) << c.index[j] << '\t' << c.valid[j] << '\t' << setfill('0') << setw(t) << c.cacheContent[j].first << '\t' << '\t' << setfill('0') << setw(d) << c.cacheContent[j].second << endl;
        }

        // Total number of accesses
        cout << "Total number of accesses: " << c.count << endl;

        //The hit and miss ratios
        double missRatio = (double)c.miss / (idx + 1.0);
        cout << "Miss ratio= " << missRatio << endl;
        cout << "Hit ratio= " << 1 - missRatio << endl;

        // The Average Memory Access Time(AMAT) of the memory hierarchy(in cycles)
        double AMAT = missRatio * 100.0 + c.num_of_cycles;
        cout << "The Average Memory Access Time(AMAT) of the memory hierarchy(in cycles)= " << AMAT << endl;
        cout << "------------------------------------------------------------------------" << endl;
    }
    else if (which_cache == 1)
    {
        if (c_bonus.isHit)
        {
            cout << "This is a hit" << endl;
        }
        else
        {
            cout << "This is a miss" << endl;
        }
        cout << endl;
        //print the content of the cache
        // Valid bitsand tags of all entries
        int i = log2(c_bonus.s);
        int d = log2(c_bonus.l);
        int t = 32 - (d + i);

        cout << "The cahce for the Instructions" << endl;
        cout << "Index" << '\t' << "Valid" << '\t' << "Tag" << '\t' << '\t' << '\t' << '\t' << '\t' << "Content" << endl;
        for (int j = 0; j < c_bonus.s; j++)
        {
            cout << setfill('0') << setw(i) << c_bonus.index[j] << '\t' << c_bonus.validInst[j] << '\t' << setfill('0') << setw(t) << c_bonus.cacheInst[j].first << '\t' << '\t' << setfill('0') << setw(d) << c_bonus.cacheInst[j].second << endl;
        }
        cout << endl;
        cout << "The cache for the the data" << endl;
        cout << "Index" << '\t' << "Valid" << '\t' << "Tag" << '\t' << '\t' << '\t' << '\t' << '\t' << "Content" << endl;
        for (int j = 0; j < c_bonus.s; j++)
        {
            cout << setfill('0') << setw(i) << c_bonus.index[j] << '\t' << c_bonus.validData[j] << '\t' << setfill('0') << setw(t) << c_bonus.cacheData[j].first << '\t' << '\t' << setfill('0') << setw(d) << c_bonus.cacheData[j].second << endl;
        }

        // Total number of accesses
        cout << "Total number of accesses: " << c_bonus.count << endl;

        //The hit and miss ratios
        double missRatio = (double)c_bonus.miss / (idx + 1.0);
        cout << "Miss ratio= " << missRatio << endl;
        cout << "Hit ratio= " << 1 - missRatio << endl;

        // The Average Memory Access Time(AMAT) of the memory hierarchy(in cycles)
        double AMAT = missRatio * 100.0 + c_bonus.num_of_cycles;
        cout << "The Average Memory Access Time(AMAT) of the memory hierarchy(in cycles)= " << AMAT << endl;
        cout << "------------------------------------------------------------------------" << endl;
    }
}
//Simulation
int decimalToBinary(int n)
{
    int remainder;
    int binary = 0, i = 1;

    while (n != 0)
    {
        remainder = n % 2;
        n = n / 2;
        binary = binary + (remainder * i);
        i = i * 10;
    }
    return binary;
}
int binaryToDecimal(int n)
{
    int num = n;
    int dec_value = 0;
    int base = 1;
    int temp = num;
    while (temp)
    {
        int last_digit = temp % 10;
        temp = temp / 10;

        dec_value += last_digit * base;

        base = base * 2;
    }
    return dec_value;
}
void split_sequence(bool which_cache)
{
    if (which_cache == 0)
    {
        c.cacheContent.resize(c.s);
        c.valid.resize(c.s);
        c.index.resize(c.s);
        for (long i = 0; i < c.s; i++)
        {
            c.index[i] = decimalToBinary(i);
        }
        for (int i = 0; i < c.sequence.size(); i++)
        {
            int d = pow(10, log2(c.s));
            int index = c.sequence[i] % d;
            int tag = c.sequence[i] / d;
            cout << "idx: " << c.sequence[i] << " " << c.s << endl;
            c.isHit = 0;
            c.count++;
            if (c.valid[binaryToDecimal(index)] && c.cacheContent[binaryToDecimal(index)].first == tag)
            {
                c.isHit = 1;
                c.hit++;
            }
            else
            {
                c.valid[binaryToDecimal(index)] = 1;
                c.cacheContent[binaryToDecimal(index)].first = tag;
                c.cacheContent[binaryToDecimal(index)].second = memory[c.sequence[i]];
                c.miss++;
            }
            print_output(i, 0);
        }
    }
    else if (which_cache == 1)
    {
        c_bonus.cacheInst.resize(c_bonus.s);
        c_bonus.cacheData.resize(c_bonus.s);
        c_bonus.validInst.resize(c_bonus.s);
        c_bonus.validData.resize(c_bonus.s);
        c_bonus.index.resize(c_bonus.s);

        for (long i = 0; i < c_bonus.s; i++)
        {
            c_bonus.index[i] = decimalToBinary(i);
        }

        for (int i = 0; i < c_bonus.sequence.size(); i++) 
        {
            //cout << c_bonus.s;
            int divide = pow(10, log2(c_bonus.s));
            int idx = c_bonus.sequence[i] % divide;
            int tagg = c_bonus.sequence[i] / divide;
            cout << "idx: " << c_bonus.sequence[i] << endl;
            c_bonus.isHit = 0;
            c_bonus.count++;
            if ((c_bonus.validInst[binaryToDecimal(idx)] && c_bonus.cacheInst[binaryToDecimal(idx)].first == tagg)
                || (c_bonus.validData[binaryToDecimal(idx)] && c_bonus.cacheData[binaryToDecimal(idx)].first == tagg))
            {
                c_bonus.isHit = 1;
                c_bonus.hit++;
            }
            else 
            {
                if (c_bonus.seqCheck[i] == 'I')
                {              c_bonus.validInst[binaryToDecimal(idx)] = 1;
                    c_bonus.cacheInst[binaryToDecimal(idx)].first = tagg;
                    c_bonus.cacheInst[binaryToDecimal(idx)].second = memory[c_bonus.sequence[i]];
                    c_bonus.miss++;
                }
                if (c_bonus.seqCheck[i] == 'D')
                {
                    c_bonus.validData[binaryToDecimal(idx)] = 1;
                    c_bonus.cacheData[binaryToDecimal(idx)].first = tagg;
                    c_bonus.cacheData[binaryToDecimal(idx)].second = memory[c_bonus.sequence[i]];
                    c_bonus.miss++;
                }
            }
            print_output(i, 1);
        }
    }
}
int main()
{
    string memoryFile;
    cout << "Please, Enter the memory file name" << endl;
    cin >> memoryFile;
    read_file(0, memoryFile);

    string sequenceFile;
    cout << "Please, enter the Sequence File Name" << endl;
    cin >> sequenceFile;

    bool lable_seq = 0;
    cout << "Please, enter 1 if the sequence file is labled, or 0 if it is not" << endl;
    cin >> lable_seq;
    if (lable_seq == 0)
    {
        read_file(1, sequenceFile);
        cout << "Please, enter the total cache size S" << endl;
        cin >> c.s;
        cout << "Please, enter the cache line size L" << endl;
        cin >> c.l;
        cout << "Please, enter the number of cycles needed to access the cache" << endl;
        cin >> c.num_of_cycles;

        split_sequence(0);
    }
    else if (lable_seq == 1)
    {
        read_file(2, sequenceFile);
        cout << "Please, enter the total cache size S" << endl;
        cin >> c_bonus.s;
        cout << "Please, enter the cache line size L" << endl;
        cin >> c_bonus.l;
        cout << "Please, enter the number of cycles needed to access the cache" << endl;
        cin >> c_bonus.num_of_cycles;

        split_sequence(1);
    }

    return 0;
}