#pragma once
#include "UserModel.h"
#include "databaseManager.h"
#include <string>
#include "sqlite3.h"
#include <ctime>
#include "ProvinceModel.h"
#include <vector>

using namespace std;
using Record = std::vector<std::string>;
using Records = std::vector<Record>;
class UserManager
{
public:
	UserManager();
	
	UserManager(string userName) {
		username = userName;
	};
	
	
	//login
	bool loginUser(string, string);
	//signUp
	void addUser(string, string, string, string, int, int);
	//Mess
	void addMessToDB(string, string, string, char*);
	void sendMess(string,string);
	void showMess();
	void showInbox(string,string);
	//Friend
	void addFriend();
	void showFriendList();
	Records getfriendList();
	//Block
	void Block();
	void groupByCity();
	

	~UserManager();
private:
	Records friendList;
	static int callback(void *, int , char **, char **);
	static int callback_fr(void *, int, char **, char **);
	static int callback_ib(void *, int, char **, char **);
	bool isBlocked(string, string);
	bool isFriend(string, string);
	void clear_buffer();
	int isExisted(string);
	void openDb();
	void BlockExe(string);
	void Unfriend(string,string);
	int block_count;
	bool logined;
	string username;
	int id;

	const char* data = "Callback function called";
	sqlite3 * db;
	char *zErrMsg = 0;
};

