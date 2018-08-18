#include "UserManager.h"
#include "stdbool.h"
#include <cstdbool>
#include <vector>
#include <string>
#include <algorithm>



UserManager::UserManager()
{
}

bool UserManager:: loginUser(string username, string pw) {
	
	sqlite3_stmt *pSelectStatement = NULL;
	int iResult = SQLITE_ERROR;
	bool ret = true;
	openDb();
	string sql = "SELECT * FROM USER " \
		"WHERE USER.username = '" \
		+ username
		+ "' AND USER.password = '" + pw + "';";
	iResult = sqlite3_prepare_v2(db, sql.c_str(), -1, &pSelectStatement, 0);
	if ((iResult == SQLITE_OK) && (pSelectStatement != NULL))
	{
		iResult = sqlite3_step(pSelectStatement);
		//was found?
		if (iResult == SQLITE_DONE) {
			ret = false;
			
			sqlite3_clear_bindings(pSelectStatement);
			sqlite3_reset(pSelectStatement);
		}
		else {
			//get id
			id = sqlite3_column_int(pSelectStatement, 0);
		}
		iResult = sqlite3_finalize(pSelectStatement);
	}
	return ret;
}



void UserManager::addUser(string username, string pw, string name, string dob, int sex, int address) {
	openDb();
	string sql = "INSERT INTO USER (username,password,name,DOB,sex,address) " \
		"VALUES('" + username + "','" + pw + "','" + name + "','" + dob + "','" + to_string(sex) + "','" + to_string(address) + "')";
	cout << "sql : " << sql << endl;
	int rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		fprintf(stdout, "add user success\n");
	}
	sqlite3_close(db);
}

void UserManager::sendMess(string sender, string receiver) {
	
	if (isExisted(receiver)) {
		if (!isBlocked(sender, receiver)) {
			char* content;
			content = new char;
			cout << "Content: ";
			cin >> content;

			//take real time
			time_t hientai = time(0);
			char* dt = ctime(&hientai);
			tm *gmtm = gmtime(&hientai);
			dt = asctime(gmtm);

			addMessToDB(sender, receiver, content, dt);
		}
		else cout << "Cannot send : user '" << receiver << "' is in your blocked list or has blocked you" << endl;

	}
	else cout << "Error : user '" << receiver << "' is not existed" << endl;
	fflush(stdin);
	
}

void UserManager::showMess() {
	openDb();
	string sql = "SELECT MESSAGE.* FROM MESSAGE,USER WHERE USER.username = '" + username + "';";

	cout << "sql : " << sql << endl;
	int rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
	}
	sqlite3_close(db);
}

Records UserManager::getfriendList() {
	return friendList;
}

void UserManager::showInbox(string user1,string user2 ) {
	openDb();
	Records records;

	string sql = "SELECT b.sender,b.receiver,b.content FROM message as b " \
		" WHERE(b.sender = '" + user1 + "' AND b.receiver = '" + user2 + "')" \
		" OR (b.receiver = '" + user1 + "' AND b.sender = '" + user2 + "')" \
		" ORDER BY b.time;";
	

	//cout << "sql: " << sql << endl;
	//int rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
	int rc = sqlite3_exec(db, sql.c_str(), callback_ib, &records, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		for (int i = 0; i < records.size(); i++) {
			if (records[i][0].compare(username)== 0)
				cout << "\t\t\tme:" << records[i][2] << endl;
			else if (records[i][0].compare(user2) == 0)
				cout << user2 << ": " << records[i][2] << endl;
			//fprintf(stdout," %d. %s\n",i+1,records[i][0]);
		}
		cout << "\n\n";
	}
	sqlite3_close(db);
}

void UserManager::addFriend() {
	openDb();
	string frien;

	cout << "Enter username: ";
	cin >> frien;
	bool isEx = isExisted(frien);
	bool isBl = isBlocked(username, frien);
	bool isFr = isFriend(username, frien);

	if (!isEx) cout << "ERROR : user does not exist" << endl;
	else if (isBl) cout << "ERROR : block cmnr" << endl;
	else if (isFr) cout << "You are already friend" << endl;
	if (isEx && !isBl && !isFr) {
		string sql = "INSERT INTO FRIEND (user1,user2) " \
			"VALUES('" + username + "','" + frien + "')";
		cout << "sql : " << sql << endl;
		int rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
		if (rc != SQLITE_OK) {
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}
		else {
			fprintf(stdout, "add friend success\n");
		}
		cout << "Add friend success" << endl;
	}

	sqlite3_close(db);
}

void UserManager::showFriendList() {
	openDb();
	Records records;
	
	string sql = "SELECT a.username  FROM USER as a, FRIEND as b" \
		" WHERE(b.user1 ='" + username + \
		"' AND a.username = b.user2 )" \
		" OR(b.user2 = '" + username + \
		"' AND a.username = b.user1 )" \
		" ORDER BY a.username;";
	/*string sql = "SELECT distinct a.username  FROM USER as a, FRIEND as b, message as c" \
		" WHERE(b.user1 =" + to_string(id) + \
		" AND a.userID = b.user2 )" \
		" OR(b.user2 = " + to_string(id) + \
		" AND a.userID = b.user1 )" \
		" AND c.receiver = a.username"
		" ORDER BY a.username;";*/
	int rc = sqlite3_exec(db, sql.c_str(), callback_fr, &records, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		friendList = records;
		for (int i = 0; i < records.size(); i++) {
			cout << " " << i + 1 << ". " << records[i][0] << endl;
			//fprintf(stdout," %d. %s\n",i+1,records[i][0]);
		}
		cout << "\n\n";
	}
	sqlite3_close(db);
}


void UserManager::Block() {
	string victim;
	openDb();
	cout << "Enter name you want to block : ";
	cin >> victim;
	clear_buffer();
	bool isEx = isExisted(victim);
	bool isBl = isBlocked(username, victim);
	bool isFr = isFriend(username, victim);
	if (!isEx) cout << "ERROR : user does not exist" << endl;
	else if (isBl) cout << "ERROR : Username is already blocked" << endl;
	else if (isFr) cout << "Username is in your friendlist" << endl;
	if (isEx && !isBl) {
		cout << "Are you sure you want to block this person?\nEnter Y for Yes, N for No : ";
		char ch = getchar();
		while ((ch != 'Y') && (ch != 'N') && (ch != 'n') && (ch != 'y')) {
			cout << "Wrong syntax. Please try again" << endl;
			cout << "Enter Y for Yes, N for No : ";
			ch = getchar();
		}

		if ((ch == 'N') || (ch == 'n')) {
			cout << "Okay\n";
			return;
		}
		else {
			BlockExe(victim);
			if (isFr) Unfriend(username, victim);
		}

	}
	sqlite3_close(db);
}






int UserManager::callback(void *data, int argc, char **argv, char **azColName) {
	int i;
	
	for (i = 1; i<argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}




void UserManager::addMessToDB(string sender, string receiver, string content, char* dt) {
	openDb();
	string sql = "INSERT INTO MESSAGE (sender,receiver,content,time) " \
		"VALUES('" + sender + "','" + receiver + "','" + content + "','" + dt + "')";
	//cout << "sql : " << sql << endl;
	int rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		fprintf(stdout, "SENT\n");
	}
	sqlite3_close(db);
}


void UserManager::openDb() {
	int rc;
	rc = sqlite3_open("week4.db", &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return;
	}
	else {

	}

}

void UserManager::BlockExe(string victim) {

	string sql = "INSERT INTO BLOCK (blocker,blocked) " \
		"VALUES('" + username + "','" + victim + "')";
	//cout << "sql : " << sql << endl;
	int rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {

	}

}

void UserManager::Unfriend(string username, string victim) {

	string sql = "DELETE FROM FRIEND " \
		"WHERE ( FRIEND.user1 = '" \
		+ username + "' AND FRIEND.user2 = '" + victim \
		+ "' ) OR ( FRIEND.user1 = '" \
		+ victim + "' AND FRIEND.user2 = '" + username + "' );";

	//cout << "sql : " << sql << endl;
	int rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {

	}

}



int UserManager::isExisted(string username) {
	openDb();
	string sql = "SELECT * FROM USER " \
		"WHERE username = '" \
		+ username + "';";

	sqlite3_stmt *pSelectStatement = NULL;
	int iResult = SQLITE_ERROR;
	bool ret = true;

	iResult = sqlite3_prepare_v2(db, sql.c_str(), -1, &pSelectStatement, 0);
	if ((iResult == SQLITE_OK) && (pSelectStatement != NULL))
	{
		iResult = sqlite3_step(pSelectStatement);
		//was found?
		if (iResult == SQLITE_DONE) {
			ret = false;
			sqlite3_clear_bindings(pSelectStatement);
			sqlite3_reset(pSelectStatement);
		}
		iResult = sqlite3_finalize(pSelectStatement);
	}
	return ret;
}



bool UserManager::isBlocked(string sender, string receiver) {
	openDb();
	string sql = "SELECT * FROM BLOCK " \
		"WHERE ( BLOCK.blocker = '" \
		+ receiver + "' AND BLOCK.blocked = '" + sender \
		+ "' ) OR ( BLOCK.blocker = '" \
		+ sender + "' AND BLOCK.blocked = '" + receiver + "' );";

	/* Execute SQL statement */
	sqlite3_stmt *selectstmt = NULL;
	int iResult = SQLITE_ERROR;
	bool ret = true;
	int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &selectstmt, NULL);
	if ((result == SQLITE_OK) && (selectstmt != NULL))
	{
		if (sqlite3_step(selectstmt) == SQLITE_ROW)
		{
			// record found
		}
		else
		{
			ret = false;
		}
	}
	sqlite3_close(db);
	return ret;
}

bool UserManager::isFriend(string user1, string user2) {
	openDb();
	string sql = "SELECT * FROM FRIEND " \
		"WHERE ( FRIEND.user1 = '" \
		+ user1 + "' AND FRIEND.user2 = '" + user2 \
		+ "' ) OR ( FRIEND.user1 = '" \
		+ user2 + "' AND FRIEND.user2 = '" + user1 + "' );";

	/* Execute SQL statement */
	sqlite3_stmt *selectstmt = NULL;
	int iResult = SQLITE_ERROR;
	bool ret = true;
	int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &selectstmt, NULL);
	if ((result == SQLITE_OK) && (selectstmt != NULL))
	{
		if (sqlite3_step(selectstmt) == SQLITE_ROW)
		{
			// record found
		}
		else
		{
			ret = false;
		}
	}
	sqlite3_close(db);
	return ret;
}

void UserManager::clear_buffer() {
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF);
}


void UserManager::groupByCity() {
	ProvinceModel province;
	
	openDb();
	sqlite3_stmt *pSelectStatement = NULL;
	int iResult = SQLITE_ERROR;
	Records records;
	string sql = "SELECT c.province, a.username  FROM USER as a, FRIEND as b, ADDRESS as c" \
		" WHERE(b.user1 = '" + username +\
		"' AND c.username = b.user2 AND a.username = b.user2)" \
		" OR(b.user2 = '" + username +\
		"' AND c.username = b.user1 AND a.username = b.user1)" \
		" ORDER BY c.province;";
	
	int rc = sqlite3_exec(db, sql.c_str(), callback_fr, &records, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		//friendList = records;
		int count = 1;
		string check = "" ;
		for (int i = 0; i < records.size(); i++) {
			string pr = province.convertToName(atof(records[i][0].c_str()));
			std::transform(pr.begin(), pr.end(), pr.begin(), ::toupper);
			if (check != pr) {
				check = pr;
				count = 0;
				cout << pr << endl;
			}
			cout << " " << ++count << ". " << records[i][1] << endl;
		}
		cout << "\n\n";
	}
	sqlite3_close(db);

}


int UserManager::callback_fr(void *p_data, int num_fields, char **p_fields, char **p_col_names) {
	Records* records = static_cast<Records*>(p_data);
	try {
		records->emplace_back(p_fields, p_fields + num_fields);
	}
	catch (...) {
		// abort select on failure, don't let exception propogate thru sqlite3 call-stack
		return 1;
	}
	return 0;
}




int UserManager::callback_ib(void *p_data, int num_fields, char **p_fields, char **p_col_names)
{
	Records* records = static_cast<Records*>(p_data);
	try {
		records->emplace_back(p_fields, p_fields + num_fields);
	}
	catch (...) {
		// abort select on failure, don't let exception propogate thru sqlite3 call-stack
		return 1;
	}
	return 0;
}


UserManager::~UserManager()
{
}
