#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <conio.h>
#include <iterator>

using namespace std;

struct Statistics{
	int count;
	string word;
};

string prepare(const  string& word){
	string result;
	result = word;
	int end = result.length() - 1;
	transform(word.begin(), word.end(), result.begin(), ::tolower);
	if (result[end] == '.'){
		result.pop_back();
	}
	if (result[end] == ','){
		result.pop_back();
	}
	if (result[end] == '!'){
		result.pop_back();
	}
	if (result[end] == '?'){
		result.pop_back();
	}
	return result;
}

bool diff(const pair<string, int>  &p1, const pair<string, int> &p2){
	return p1.second < p2.second;
}

void print(const Statistics &obj)
{
	cout << obj.word << endl;
	cout << obj.count << endl;
}


int main(){
	struct Statistics str;
	string word;
	int word_count;
	vector <Statistics> vector;
	typedef map <string, int> MapType;
	MapType dict;


	ifstream file("Source.txt");
	if (file.is_open()){
		while (!file.eof()){
			file >> word;
			word = prepare(word);
			dict[word] += 1;
		}
	}

	for (auto elem : dict) {

		str.word = elem.first;
		str.count = elem.second;
		vector.push_back(str);
	}

	cout << "Repetition rate - Word;\n";
	for (int i = 0; i < vector.size(); ++i) {
		if (vector[i].count > 10) {
			cout << vector[i].count << " - ";
			cout << vector[i].word << '\n';
		}
	}
   
	system("pause");
	return 0;
}
