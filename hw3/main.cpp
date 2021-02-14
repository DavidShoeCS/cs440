#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <string.h>
#include <sstream>
#include <vector>

using namespace std;


class Employee {
	public:
		int id;
		string name;
		string bio;
		int m_id;

		Employee(int i, string n, string b, int mi) {
			id = i;
			name = n;
			bio = b;
			m_id = mi;
		}

	string print() {
		return to_string(id) + "|" + name + "|" + bio + "|" + to_string(m_id);
	}
};


class Block {
	vector<Employee> records;
	Block *overflow;

	public:
	Block() {
		overflow = NULL;
		records.clear();
	}

	bool isPresent(int id) {
		Block *node = this;
		while(node) {
			for(int i = 0; i < node->records.size(); i++) {
				if(node->records[i].id == id) {
					return true;
				}
			}
			node = node->overflow;
		}
		return false;
	}

	void add(Employee emp) {
		if(records.size() < 5) {
			records.push_back(emp);
		}
		else {
			if(overflow == NULL) {
				overflow = new Block();
			}
			overflow->add(emp);
		}
	}

	void clearElements(vector<Employee> &v) {
		for(int i = 0; i < records.size(); i++) {
			v.push_back(records[i]);
		}
		records.clear();
		if(overflow) {
			overflow->clearElements(v);
			delete overflow;
			overflow = NULL;
		}
	}

	string print() {
		Block *node = this;
		string output = "";

		while(node) {
			for(unsigned int i = 0; i < node->records.size(); i++) {
				output += node->records[i].print();
				if (i != node->records.size()-1) {
					output += ",";
				}
			}

			node = node->overflow;
			if (node) {
				output += ",";
			}
		}

		return output;
	}
};


class HashTable {
	public:
		int numRecords, numBits;
		vector<Block *> buckets;

		HashTable() {
			numRecords = 0;
			numBits = 1;
			buckets.push_back(new Block());
		}

	unsigned int hash(int id) {
		unsigned int mod = (1 << numBits);
		return (unsigned int)(id % mod + mod) % mod;
	}

	int occupancy() {
		double ratio = 1.0 * numRecords / buckets.size();
		return (int)(100 * (ratio / 5));
	}

	bool isPresent(int num) {
		unsigned int k = hash(num);
		if(k >= buckets.size()) {
			k -= (1 << (numBits - 1));
		}
		if(buckets[k]->isPresent(num)) {
			return true;
		}
		return false;
	}

	void insert(Employee emp) {
		unsigned int k = hash(emp.id);
		if(k >= buckets.size()) {
			k -= (1 << (numBits - 1));
		}

		buckets[k]->add(emp);
		numRecords++;
		while(occupancy() >= 80) {
			buckets.push_back(new Block());
			numBits = ceil(log2((double)buckets.size()));
			// split old bucket and rehash
			k = buckets.size() - 1 - (1 << (numBits - 1));
			vector<Employee> v;
			buckets[k]->clearElements(v);
			for(unsigned int i = 0; i < v.size(); i++) {
				buckets[hash(v[i].id)]->add(v[i]);
			}
		}
	}

	string print() {
		string output = "";
		for(unsigned int i = 0; i < buckets.size(); i++) {
			output += to_string(i) + "," + buckets[i]->print() + '\n';
		}
		return output;
	}

};
HashTable ht;

unsigned int hashLookup(int id, int lines) {
	unsigned int mod = (1 << (int)ceil(log2(lines)));
	return (unsigned int)(id % mod + mod) % mod;
}


void Create() {
	ifstream infile("Employee.csv");
	if(!infile.is_open()){
			cerr << "Input file failed to open." << endl;
			return;
	}

	int id, manager_id;
	string name, bio, idstring, manager_idstring, line;

	while(getline(infile, line)){
			stringstream ss(line);
			getline(ss, idstring, ',');
			getline(ss, name, ',');
			getline(ss, bio, ',');
			getline(ss, manager_idstring, ',');
			id = stoi(idstring);
			manager_id = stoi(manager_idstring);

			Employee emp(id, name, bio, manager_id);
			ht.insert(emp);
	}

	ofstream outfile("EmployeeIndex.csv");
	if(!outfile.is_open()){
		cerr << "Output file failed to open." << endl;
		return;
	}

	outfile << ht.print();
	outfile.close();
	infile.close();
}


void Lookup(char* argv2) {
	string line;
	ifstream count_lines_in_file("EmployeeIndex.csv");
	int ctr = 0;
	while(getline(count_lines_in_file, line)){
		ctr++;

	}
	count_lines_in_file.close();

	ifstream infile("EmployeeIndex.csv");
	//fail to open EmployeeIndex.csv
	if(!infile.is_open()){
		cerr << "File failed to open" << endl;
		return;
	}

	int lookup_id = stoi(argv2);
	int number_of_buckets = ctr;

	int index = hashLookup(lookup_id, number_of_buckets);
	if(index >= number_of_buckets) {
			index -= (1 << ((int)(ceil(log2(number_of_buckets))) - 1));
	}

	string hashvaluestring;
	while(getline(infile, line)){

    stringstream ss(line);
		getline(ss,hashvaluestring,',');

		//Get the line associated with the hashed version of the id we give our program.
		if (stoi(hashvaluestring) == index){
			//the values after bucket/hashed id number
			string newline = line.substr(line.find(",")+1);

			//seperate the the line of employees into a line for each employee
			vector<string> v;
			stringstream s_stream(newline);
			while(s_stream.good()){
				string substr;
				getline(s_stream, substr, ',');
				v.push_back(substr);
			}

			for (size_t i=0; i<v.size(); i++){
				stringstream ss2(v[i]);
				string id_var;

				getline(ss2, id_var, '|');
				if(id_var == string(argv2)){
					//return employee
					cout << v[i] << endl;
				}
			}
		}
	}
	infile.close();
}


int main(int argc, char *argv[]) {

		if((std::string(argv[1]) != "-C" && std::string(argv[1]) != "-L")) {
			cerr << "Format: ./main.out [-C, -L] \n";
			exit(-1);
		}

		if (string(argv[1]) == "-C") {
			Create();
    } else if(string(argv[1]) == "-L"){
			if(argc == 3){
				Lookup(argv[2]);
		  } else {
				cerr << "Format: ./main.out -L [employee id]" << endl;
				exit(-1);
			}
		} else {
			cerr << "Format: ./main.out [-C, -L]" << endl;
			exit(-1);
		}
	return 0;
}
