#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <algorithm>
#include <math.h>

//class for an employee.
class Employee;

//class for a department.
class Department;

//class for when we merge a department and employee together.
//I couldn't declare this and put it below main for some reason like the others...
//It has to do with the string output function for some reason.
class Department_Employee{
private:
public:
		int did;
		std::string dname;
		double budget;
		int managerid;
		int eid;
		std::string ename;
		int age;
		double salary;
		Department_Employee(int did,std::string dname, double budget, int managerid, int eid,std::string ename, int age, double salary){
			this->did = did;
			this->dname = dname;
			this->budget = budget;
			this->managerid = managerid;
			this->eid = eid;
			this->ename = ename;
			this->age = age;
			this->salary = salary;
		}
		std::string output(){
			std::string printout = std::to_string(did) + ',' + dname + ',';
			printout += std::to_string((int)budget) + ',' + std::to_string(managerid) + ',';
			printout += std::to_string(eid) + ',' + ename + ',' + std::to_string(age) + ',';
			printout += std::to_string((int)salary) + '\n';
			return printout;
		}
};

//function to get a department attributes.
std::vector<Department> get_dept();

//function to get employee attributes.
std::vector<Employee> get_emp();

//function to sort and combine employee and department attributes by using the sort-merge-join algorithm.
std::vector<Department_Employee> sort_merge_join(std::vector<Department> dept, std::vector<Employee> emp);

int main(){
	std::vector<Employee> emp_table = get_emp();
	std::vector<Department> dept_table = get_dept();
	std::vector<Department_Employee> d_e_table = sort_merge_join(dept_table, emp_table);
	std::ofstream fout("join.csv");
	for (int i=0;i<d_e_table.size();i++){
		fout << d_e_table[i].output();
	}
	fout.close();
	return 0;
}

//class for an employee.
class Employee{
private:
public:
		int eid;
		std::string ename;
		int age;
		double salary;
		Employee(int eid,std::string ename, int age, double salary){
			this->eid = eid;
			this->ename = ename;
			this->age = age;
			this->salary = salary;
		}
		bool operator<(const Employee& emp) const
		{
			return eid < emp.eid;
		}
};

//class for a department.
class Department{
private:
public:
		int did;
		std::string dname;
		double budget;
		int managerid;
		Department(int did,std::string dname, double budget, int managerid)
		{
			this->did = did;
			this->dname = dname;
			this->budget = budget;
			this->managerid = managerid;
		}
		bool operator<(const Department& dept) const
		{
			return managerid < dept.managerid;
		}
};

//function to get a department attributes.
std::vector<Department> get_dept(){
	std::ifstream dept_data;
	std::vector<Department> dept_table;
	std::size_t size;
	dept_data.open("Dept.csv");
	std::string sub;
	std::string line;
	while (getline(dept_data, line)){
		std::vector<std::string> storage(0);
		std::stringstream ss(line);
		while(ss.good()){
      getline(ss, sub, ',');
			storage.push_back(sub);
		}
		Department dept(std::stoi(storage[0],&size),storage[1],std::stod(storage[2], &size),std::stoi(storage[3],&size));
		dept_table.push_back(dept);
	}
	dept_data.close();
	return dept_table;
}

//function to get employee attributes.
std::vector<Employee> get_emp(){
	std::ifstream emp_data;
	std::vector<Employee> emp_table;
	std::size_t size;
	emp_data.open("Emp.csv");
	std::string sub;
	std::string line;
	while (getline(emp_data, line)){
		std::vector<std::string> storage(0);
		std::stringstream ss(line);
		while(ss.good()){
      getline(ss, sub, ',');
			storage.push_back(sub);
		}
		Employee emp(std::stoi(storage[0],&size),storage[1],std::stod(storage[2], &size),std::stoi(storage[3],&size));
		emp_table.push_back(emp);
	}
	emp_data.close();
	return emp_table;
}

//function to sort and combine employee and department attributes by using the sort-merge-join algorithm.
std::vector<Department_Employee> sort_merge_join(std::vector<Department> dept, std::vector<Employee> emp){
	std::vector<Department_Employee> output;
	sort(dept.begin(), dept.end());
	sort(emp.begin(), emp.end());
	int dctr = 0, ectr = 0;
	while (dctr < dept.size() && ectr < emp.size()){
		if (dept[dctr].managerid == emp[ectr].eid){
			Department_Employee d_e_join(dept[dctr].did, dept[dctr].dname, dept[dctr].budget, dept[dctr].managerid, emp[ectr].eid, emp[ectr].ename, emp[ectr].age, emp[ectr].salary);
			output.push_back(d_e_join);
			dctr++;
		}
		else if(dept[dctr].managerid < emp[ectr].eid){
			dctr++;
		}
		else{
			ectr++;
		}
	}
	return output;
}
