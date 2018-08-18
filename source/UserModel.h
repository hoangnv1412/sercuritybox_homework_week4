#pragma once
#include <iostream>
#include <cstring>
using namespace std;

class UserModel
{
private:
	string id;
	string pw;
	string name;
	int sex;
	int address;
	string dob;
public:

	UserModel() ;
	UserModel(string Id, string Pw, string Name, string Dob, int Sex, int Address)
	{
		this->id = Id;
		this->pw = Pw;
		this->name = Name;
		this->dob = Dob;
		this->sex = Sex;
		this->address = Address;
	};
	//getter
	string getId();
	string getPw();
	string getName();
	string getdob();
	int getSex();
	int getAddress();

	//setter
	void setName(string);
	void setPw(string);
	void setDob(string);
	void setSex(int);
	void setAddress(int);
	

	
	~UserModel();
};


