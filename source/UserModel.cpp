#include "UserModel.h"



UserModel::UserModel()
{
}




//getter
string UserModel::getId() {
	return id;
}
string UserModel::getPw() {
	return pw;
}
string UserModel::getName() {
	return name;
}
string UserModel::getdob() {
	return dob;
}
int UserModel::getSex() {
	return sex;
}
int UserModel::getAddress() {
	return address;
}

//setter
void UserModel::setName(string name) {
	this->name = name;
}
void UserModel::setPw(string Pw) {
	this->pw = Pw;
}
void UserModel::setDob(string Dob) {
	this->dob = Dob;
}
void UserModel::setSex(int Sex) {
	this->sex = Sex;
}
void UserModel::setAddress(int Address) {
	this->address = Address;
}



UserModel::~UserModel()
{
}
