#include<iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;


enum enMainChoice
{
	ShowC = 1, AddC = 2, DeleteC = 3, UpdateC = 4, FindC = 5, Transactions = 6, ManageU = 7, Logout = 8
};

enum enTransactionChoice
{
	eDeposit = 1, eWithdraw = 2, eTotalBalances = 3, eMainMenue = 4
};

enum enManageUserChoice
{
	ListU = 1, AddU = 2, DeleteU = 3, UpdateU = 4, FindU = 5, MainMenue = 6
};

enum enUserPermessions
{
	AllPerm = -1, ShowCListPerm = 1, AddNewCPerm = 2, DeleteCPerm = 4, UpdateCPerm = 8, FindCPerm = 16, ShowTransPerm = 32, ManageUsersPerm = 64
};

struct stClient {
	string AccNumber;
	string PinCode;
	string Name;
	string Phone;
	int Balance = 0;
	bool MarkForDelete = false;
};

struct stUser {
	string Username;
	string Password;
	short Perm = 0;
	bool MarkForDelete = false;
};

const string UsersDataFile = "DatabaseFiles/Users.txt";

const string ClientsDataFile = "DatabaseFiles/Clients.txt";

stUser LoginUser;


//String Functions 
string EnterString(string Message) {
	string s;
	cout << Message;
	getline(cin >> ws, s);
	return s;
}

char EnterChar(string Message) {
	char c;
	cout << Message;
	cin >> c;
	return c;
}

vector <string> SplitString(string s, string delim) {

	string word;
	short pos;
	vector <string> vNames;

	while ((pos = s.find(delim)) != string::npos) {

		word = s.substr(0, pos);
		if (word != "") {
			vNames.push_back(word);
		}
		s.erase(0, pos + delim.length());

	}
	if (s != "") {
		vNames.push_back(s);
	}
	return vNames;

}

string JoinString(vector <string> vString, string delim) {

	string NewString;

	for (string& s : vString) {

		NewString += s + delim;
	}

	return NewString.substr(0, NewString.length() - delim.length());

}

//-------------------------------------


//read Number function
int ReadNumber(string Message) {
	int N;
	cout << Message;;
	cin >> N;
	return N;

}

//Function & Metodes on Files And Records

string ConvertRecordToLine(stClient Client, string Separator = "#//#") {

	string stClientRecord;

	stClientRecord += Client.AccNumber + Separator;
	stClientRecord += Client.PinCode + Separator;
	stClientRecord += Client.Name + Separator;
	stClientRecord += Client.Phone + Separator;
	stClientRecord += to_string(Client.Balance);

	return stClientRecord;
}

string ConvertUserRecordToLine(stUser User, string Separator = "#//#") {

	string stUserRecord = "";

	stUserRecord += User.Username + Separator;
	stUserRecord += User.Password + Separator;
	stUserRecord += to_string(User.Perm);
	return stUserRecord;

}

stClient ConvertLineToRecord(string Line, string  delim = "#//#") {

	stClient Client;
	vector <string> vRecord = SplitString(Line, delim);

	Client.AccNumber = vRecord[0];
	Client.PinCode = vRecord[1];
	Client.Name = vRecord[2];
	Client.Phone = vRecord[3];
	Client.Balance = stoi(vRecord[4]);

	return Client;
}

stUser ConvertUserLineToRecord(string Line, string  delim = "#//#") {

	vector <string> vUser;
	stUser UserRecord;
	vUser = SplitString(Line, delim);

	UserRecord.Username = vUser[0];
	UserRecord.Password = vUser[1];
	UserRecord.Perm = stoi(vUser[2]);

	return UserRecord;

}

void DisplayClientData(stClient C) {

	cout << "\n\n the folowing are the client details   \n";
	//cout<<"========================================"
	cout << "----------------------------------------\n";
	cout << "Account number : " << C.AccNumber << endl;
	cout << "PinCode        : " << C.PinCode << endl;
	cout << "Phone Number   : " << C.Phone << endl;
	cout << "Name           : " << C.Name << endl;
	cout << "Balance        : " << C.Balance << endl;
	cout << "----------------------------------------\n\n";

}

void DisplayClientRecord(stClient C) {


	cout << "| " << setw(15) << left << C.AccNumber;
	cout << "| " << setw(10) << left << C.PinCode;
	cout << "| " << setw(40) << left << C.Name;
	cout << "| " << setw(12) << left << C.Phone;
	cout << "| " << setw(12) << left << C.Balance;

}

void DisplayUserData(stUser User) {
	cout << "\n\n the folowing are the User details   \n";
	//cout<<"========================================"
	cout << "----------------------------------------\n";
	cout << "Username    : " << User.Username << "\n";
	cout << "Password    : " << User.Password << "\n";
	cout << "Permessions : " << User.Perm << "\n";
	cout << "----------------------------------------\n\n";
}

void DisplayeUserRecord(stUser User) {

	cout << "| " << setw(20) << left << User.Username <<
		"| " << setw(18) << left << User.Password <<
		"| " << setw(18) << left << User.Perm;
	cout << "\n------------------------------------------------------------------\n";

}

//Genaric Function. (Save) and (Get) data from file. 
void SaveDataLineToFile(string FileName, string DataLine) {
	//Genaric Function.

	fstream File;

	File.open(FileName, ios::out | ios::app);
	if (File.is_open()) {
		File << DataLine << "\n";
		File.close();
	}

}

vector <stClient> SaveClientsDataToFile(string FileName, vector <stClient> vClients) {

	fstream file;

	file.open(FileName, ios::out); //write mode and delete old file.
	if (file.is_open()) {

		for (stClient& C : vClients) {

			if (C.MarkForDelete == false) {
				SaveDataLineToFile(FileName, ConvertRecordToLine(C));
			}

		}

	}file.close();
	return vClients;

}

vector<stUser> SaveUsersDataToFile(string FileName, vector<stUser>& vUsers) {

	fstream File;
	string Line;
	File.open(FileName, ios::out);//Clear & Write mode 
	if (File.is_open()) {
		for (stUser u : vUsers) {

			if (!u.MarkForDelete) {

				Line = ConvertUserRecordToLine(u, "#//#");
				SaveDataLineToFile(FileName, Line);

			}

		}
	}
	File.close();
	return vUsers;


}

vector <stClient> GetClientsDataFromFile(string FileName) {

	fstream ClientFile;
	string Line;
	vector <stClient> vClients;


	ClientFile.open(FileName, ios::in);

	while (getline(ClientFile, Line)) {

		vClients.push_back(ConvertLineToRecord(Line));

	}
	ClientFile.close();
	return vClients;

}

vector<stUser> GetUsersDataFromFile(string FileName) {

	fstream File;
	string Line;
	vector<stUser> vUsers;

	File.open(FileName, ios::in);   //Read Mode.

	if (File.is_open()) {
		while (getline(File, Line))
		{
			if (Line == "")
				continue;
			vUsers.push_back(ConvertUserLineToRecord(Line));

		}
		File.close();
		return vUsers;
	}

}

//Its Ok.......


//Find Client Functions
bool FindClientByAccNumber(string AccNumber, vector <stClient> vClients, stClient& Client) {


	for (stClient& C : vClients)
	{
		if (C.AccNumber == AccNumber) {
			Client = C;
			return true;
		}

	}
	return false;
}

bool IsClientExistByAccNumber(string AccNumber, string FileName) {

	fstream file;
	stClient Client;
	string Line;
	file.open(FileName, ios::in);//read mode

	if (file.is_open()) {

		while (getline(file, Line)) {

			Client = ConvertLineToRecord(Line);

			if (Client.AccNumber == AccNumber) {
				file.close();
				return true;
			}
		}
		file.close();

	}
	return false;

}

//Find User Functions
bool FindUserByUsername(string UsernameToFind, vector<stUser> vUsers, stUser& UserRecord) {

	stUser UserData;

	for (stUser& u : vUsers)
	{
		if (u.Username == UsernameToFind) {
			UserRecord = u;
			return true;
		}
	}
	return false;

}

bool IsUserExistByUsernameAndPassword(string Username, string Password) {

	fstream file;
	string Line;
	stUser User;

	file.open(UsersDataFile, ios::in);//read mode
	if (file.is_open()) {

		while (getline(file, Line)) {

			User = ConvertUserLineToRecord(Line);

			if (User.Username == Username && User.Password == Password) {
				file.close();
				return true;
			}
		}
		file.close();

	}
	return false;

}

bool IsUserExistByUsername(string Username, string FileName) {

	fstream file;
	string Line;
	stUser User;

	file.open(FileName, ios::in);//read mode
	if (file.is_open()) {

		while (getline(file, Line)) {

			User = ConvertUserLineToRecord(Line);

			if (User.Username == Username) {
				file.close();
				return true;
			}
		}
		file.close();

	}
	return false;

}


//Add Clients Functions
stClient FillClientData() {

	stClient Client;
	cout << "Please Enter Client Data \n" << endl;

	string AccNumber;
	//vector <stClient> vClients = GetClientsDataFromFile(ClientsDataFile);

	AccNumber = EnterString("Enter Account number : ");

	while (IsClientExistByAccNumber(AccNumber, ClientsDataFile))//ClientExistByAccNumber()
	{
		cout << "Client with [" << AccNumber << "] is Already Exist, Enter Another Acc Number ? ";
		cin >> AccNumber;
	}

	Client.AccNumber = AccNumber;
	Client.PinCode = EnterString("Enter PinCode : ");
	Client.Name = EnterString("Enter Name : ");
	Client.Phone = EnterString("Enter Phone Number : ");
	Client.Balance = ReadNumber("Enter Your Balance : ");

	return Client;



}

void AddNewClient() {

	stClient Client;
	Client = FillClientData();
	SaveDataLineToFile(ClientsDataFile, ConvertRecordToLine(Client));

}

void AddClients() {


	char AddMore = 'N';
	do
	{
		system("CLS");

		AddNewClient();

		cout << "Client Added successfully, Do You Want to add more Clients ? ";

		cin >> AddMore;
		//cin.ignore();

	} while (toupper(AddMore) == 'Y');

}

//Add Users Functions
short SetUserPermessions(short Perm) {

	Perm = 0;
	char AddPerm = 'n';
	cout << "\n\nDo You Want to give full Permessions ? ";
	cin >> AddPerm;

	if (tolower(AddPerm) == 'y') {
		Perm = enUserPermessions::AllPerm;
		return Perm;
	}

	cout << "\n\nDo You want ot give access to : \n\n";

	AddPerm = tolower(EnterChar("Show Clients Lists? y/n? "));
	cout << "\n\n";
	if (AddPerm == 'y')
		Perm |= enUserPermessions::ShowCListPerm;
	//  or
	//  Perm += enUserPermessions::ShowCListPerm;

	AddPerm = tolower(EnterChar("Add New Client? y/n? "));
	cout << "\n\n";
	if (AddPerm == 'y')
		Perm |= enUserPermessions::AddNewCPerm;
	//  or
	//  Perm += enUserPermessions::AddNewCPerm;

	AddPerm = tolower(EnterChar("Delete Client? y/n? "));
	cout << "\n\n";
	if (AddPerm == 'y')
		Perm |= enUserPermessions::DeleteCPerm;

	AddPerm = tolower(EnterChar("Update Client? y/n? "));
	cout << "\n\n";
	if (AddPerm == 'y')
		Perm |= enUserPermessions::UpdateCPerm;

	AddPerm = tolower(EnterChar("Find Client? y/n? "));
	cout << "\n\n";
	if (AddPerm == 'y')
		Perm |= enUserPermessions::FindCPerm;

	AddPerm = tolower(EnterChar("Transactions Client? y/n? "));
	cout << "\n\n";
	if (AddPerm == 'y')
		Perm |= enUserPermessions::ShowTransPerm;

	AddPerm = tolower(EnterChar("Manage Users? y/n? "));
	cout << "\n\n";
	if (AddPerm == 'y')
		Perm |= enUserPermessions::ManageUsersPerm;

	return Perm;

}

stUser FillUserData() {


	stUser User;
	string Username;

	cout << "\nEnter Username : ";

	getline(cin >> ws, Username);

	while (IsUserExistByUsername(Username, UsersDataFile))
	{
		Username = EnterString("\n\nUsername already exist Please enter anothe username? ");
	}

	User.Username = Username;
	User.Password = EnterString("\nEnter Password : ");
	User.Perm = SetUserPermessions(User.Perm);

	return User;
}

void AddNewUser() {

	stUser User;
	cout << "\nAdd User New : \n\n";
	User = FillUserData();
	SaveDataLineToFile(UsersDataFile, ConvertUserRecordToLine(User));

}

void AddUsers() {

	char AddMore = 'N';

	do
	{

		AddNewUser();
		cout << "\nUser Added Successfully, do you want to add new user? y/n? ";
		cin >> AddMore;


	} while (toupper(AddMore) == 'Y');
}


//Delete Client Functions.

bool MarkClientForDelete(string AccNumber, vector <stClient>& vClients) {

	for (stClient& C : vClients)
	{

		if (C.AccNumber == AccNumber) {
			C.MarkForDelete = true;
			return true;
		}

	}
	return false;

}

bool DeleteClientWithAccNumber(string AccNumber, vector <stClient>& vClients) {

	stClient Client;

	char Answer = 'n';
	if (FindClientByAccNumber(AccNumber, vClients, Client)) {

		DisplayClientData(Client);
		cout << "Are You sure you want to delete this Client ? ";
		cin >> Answer;

		while (tolower(Answer) == 'y')
		{

			MarkClientForDelete(AccNumber, vClients);
			SaveClientsDataToFile(ClientsDataFile, vClients);
			//Refresh Clients
			vClients = GetClientsDataFromFile(ClientsDataFile);

			cout << "\nClient Deleted Successfully. \n\n";
			return true;

		}

	}
	else {
		cout << "\nClient whth Acc Number (" << AccNumber << ") Not Founded !\n";
		return false;
	}

}

//Delete User Functions.

bool MarkUserForDelete(string UsernameForDelete, vector<stUser>& vUsers) {

	for (stUser& u : vUsers) {
		if (u.Username == UsernameForDelete) {
			u.MarkForDelete = true;
			return true;
		}
	}return false;


}

bool IsAdmin(stUser User) {

	return User.Perm == AllPerm ? true : false;
}

bool DeleteUserByUsername(string UsernameForDelete, vector<stUser>vUsers) {


	stUser User;
	char ch = 'n';

	if (FindUserByUsername(UsernameForDelete, vUsers, User)) {

		DisplayUserData(User);

		ch = EnterChar("\nAre You sure you want to delete this User? ");
		if (tolower(ch) == 'y') {
			if (IsAdmin(User))
				cout << "\n\nYou Cannot Delete this user!\n";
			else {
				MarkUserForDelete(UsernameForDelete, vUsers);
				SaveUsersDataToFile(UsersDataFile, vUsers);
				cout << "\n\nUser Deleted Successfully! -: \n";
				return true;
			}
		}
		else
			return false;
	}
	else
		cout << "\nUser Not Found!";
	return false;

}


//Update Client Functions
stClient UpdateClientRecord(string AccNumber) {
	stClient Client;

	Client.AccNumber = AccNumber;
	Client.PinCode = EnterString("Enter PinCode : ");
	Client.Name = EnterString("Enter Name : ");
	Client.Phone = EnterString("Enter Phone Number : ");
	Client.Balance = ReadNumber("Enter Your Balance : ");
	return Client;

}

bool UpdateClientByAccNumber(string AccNumber, vector <stClient>& vClients) {

	char Answer = 'n';
	stClient Client;

	if (FindClientByAccNumber(AccNumber, vClients, Client)) {

		DisplayClientData(Client);
		cout << "Are you sure you want to update this client ?";
		cin >> Answer;
		if (tolower(Answer == 'y')) {
			for (stClient& C : vClients) {

				if (C.AccNumber == AccNumber) {

					C = UpdateClientRecord(AccNumber);
					break;
				}

			}

			SaveClientsDataToFile(ClientsDataFile, vClients);
			cout << "\n\nClient updated Successfully\n";
			return true;

		}
		else
			return false;

	}
	else {
		cout << "Client whth Acc Number (" << AccNumber << ") Not Founded !\n";
		return false;
	}
}

//Update User Functions
stUser UpdateUserRecord(stUser& User) {

	User.Username = EnterString("\n\nEnter Username : ");
	User.Password = EnterString("\nEnter Password : ");


	User.Perm = (enUserPermessions)SetUserPermessions(User.Perm);

	return User;

}

bool UpdateUserByUsername(string Username, vector<stUser>& vUsers) {

	stUser User;
	if (!FindUserByUsername(Username, vUsers, User)) {
		cout << "\n\nUser Not Found! \n";
		return false;
	}

	char choice = 'n';

	DisplayUserData(User);

	choice = EnterChar("Are you sure you want to update this User? y/n? ");

	if (tolower(choice) == 'y') {

		for (stUser& u : vUsers) {
			if (u.Username == User.Username) {

				u = UpdateUserRecord(User);

				SaveUsersDataToFile(UsersDataFile, vUsers);
				cout << "\n\nClient  updated Successfully.\n";
				return true;
			}
		}

	}
	cout << "\n\nClient is not updated!\n";
	return false;



}

//Main Menue Access on Screens.
void ShowAccessDeniedScreen() {
	cout << "-----------------------------------------\n";
	cout << "Access Denied!\n";
	cout << "You dont have permession to do this!\n";
	cout << "Please Contact your admin.\n";
	cout << "-----------------------------------------\n";

}

bool IsUserHasAccess(short UserPerms, enUserPermessions CurrentPerm) {
	if ((UserPerms & CurrentPerm) == CurrentPerm)
		return true;
	return false;
}

//Show Screens
void ShowMainMenueScreen();
void ShowTransactionMenueScreen();
void ShowManageUsersScreen();

//Main Menue  Screens
void AddClientScreen(vector <stClient> vClients) {

	if (!IsUserHasAccess(LoginUser.Perm, enUserPermessions::AddNewCPerm)) {
		system("CLS");
		ShowAccessDeniedScreen();
		return;
	}

	cout << "========================================\n";
	cout << "           Add Client Screen            \n";
	cout << "========================================\n";
	AddClients();

}

void DeleteClientScreen(vector <stClient> vClients) {

	if (!IsUserHasAccess(LoginUser.Perm, enUserPermessions::DeleteCPerm)) {
		system("CLS");
		ShowAccessDeniedScreen();
		return;
	}

	string AccNum;
	cout << "========================================\n";
	cout << "          Delete Client Screen           \n";
	cout << "========================================\n";

	AccNum = EnterString("Please Enter Accout number ? ");
	DeleteClientWithAccNumber(AccNum, vClients);


}

void UpdateClientScreen(vector <stClient> vClients) {

	if (!IsUserHasAccess(LoginUser.Perm, enUserPermessions::UpdateCPerm)) {
		system("CLS");
		ShowAccessDeniedScreen();
		return;
	}

	string AccNum;
	cout << "========================================\n";
	cout << "          Update Client Screen           \n";
	cout << "========================================\n";

	AccNum = EnterString("Please Enter Accout number ? ");

	UpdateClientByAccNumber(AccNum, vClients);

}

void FindClientScreen(vector <stClient> vClients) {

	if (!IsUserHasAccess(LoginUser.Perm, enUserPermessions::FindCPerm)) {
		system("CLS");
		ShowAccessDeniedScreen();
		return;
	}
	string AccNum;
	stClient Client;
	cout << "========================================\n";
	cout << "           Find Client Screen            \n";
	cout << "========================================\n";

	AccNum = EnterString("Please Enter Accout number ? ");

	if (FindClientByAccNumber(AccNum, vClients, Client)) {

		DisplayClientData(Client);

	}
	else
		cout << "Client With Account Number (" << AccNum << ") Not Found!\n";

}

void ShowAllClients(vector <stClient> ClientsData) {

	if (!IsUserHasAccess(LoginUser.Perm, enUserPermessions::ShowCListPerm)) {
		system("CLS");
		ShowAccessDeniedScreen();
		return;
	}


	system("CLS");

	cout << "\n\t\t\t\t\tClient List (" << ClientsData.size() << ")Client(s).";

	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "Accout Number";
	cout << "| " << left << setw(10) << "Pin Code";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";
	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	if (ClientsData.size() > 0)
	{
		for (stClient& Client : ClientsData)
		{
			DisplayClientRecord(Client);
			cout << endl;
		}
	}
	else
		cout << "\t\t\t\tNo Clients Data Available!";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

}

void Login();

void BackToMainMenue() {

	cout << "\n\n\nPress Any Key to back to main menue...";
	system("pause > 0");
	ShowMainMenueScreen();

}

void PerformMainMenueChoice(enMainChoice WhatChoice) {

	vector <stClient> vClients = GetClientsDataFromFile(ClientsDataFile);

	switch (WhatChoice)
	{
	case enMainChoice::ShowC:
		system("cls");
		ShowAllClients(vClients);
		BackToMainMenue();
		break;
	case enMainChoice::AddC:
		system("cls");
		AddClientScreen(vClients);
		BackToMainMenue();
		break;
	case enMainChoice::DeleteC:
		system("cls");
		DeleteClientScreen(vClients);
		BackToMainMenue();
		break;
	case enMainChoice::UpdateC:
		system("cls");
		UpdateClientScreen(vClients);
		BackToMainMenue();
		break;
	case enMainChoice::FindC:
		system("cls");
		FindClientScreen(vClients);
		BackToMainMenue();
		break;
	case enMainChoice::Transactions:
		system("cls");
		ShowTransactionMenueScreen();
		BackToMainMenue();
		break;
	case enMainChoice::ManageU:
		system("cls");
		ShowManageUsersScreen();
		BackToMainMenue();
		break;
	case enMainChoice::Logout:
		system("cls");
		Login();
		break;
	default:
		ShowMainMenueScreen();
		break;
	}

}

void ShowMainMenueScreen() {

	system("cls");
	cout << "========================================\n";
	cout << "            Main Menue Screen            \n";
	cout << "========================================\n";
	cout << "       [1] Show Client List. \n";
	cout << "       [2] Add New Client. \n";
	cout << "       [3] Delete Client. \n";
	cout << "       [4] Update Client. \n";
	cout << "       [5] Find Client. \n";
	cout << "       [6] Transactions. \n";
	cout << "       [7] Manage Users. \n";
	cout << "       [8] Logout. \n";
	cout << "========================================\n";

	PerformMainMenueChoice((enMainChoice)ReadNumber("Chose what do you want to do ? [1 to 6] ? "));


}



//Tranactions Menue
bool DepositBalanceByAccNumber(string AccNumber, int Amount, vector <stClient>& vClients) {

	stClient Client;
	char Ans;

	Ans = EnterChar("Are you Sure You want to perform this Transaction ? ");
	if (tolower(Ans) == 'y') {

		for (stClient& C : vClients) {

			if (C.AccNumber == AccNumber) {

				C.Balance += Amount;
				cout << "Done Successfully, New Blance = " << C.Balance << endl;
				break;

			}

		}
		SaveClientsDataToFile(ClientsDataFile, vClients);
		return true;

	}
	else
		return false;

}

bool WithdrawBalanceByAccNumber(string AccNumber, int Amount, vector <stClient>& vClients) {

	stClient Client;
	char Ans;

	Ans = EnterChar("Are you Sure You want to perform this Transaction ? ");
	if (tolower(Ans) == 'y') {

		for (stClient& C : vClients) {

			if (C.AccNumber == AccNumber) {
				C.Balance -= Amount;
				cout << "Done Successfully, New Blance = " << C.Balance << endl;
				break;

			}

		}
		SaveClientsDataToFile(ClientsDataFile, vClients);
		return true;

	}
	else
		return false;

}

void DepositMenueScreen() {

	cout << "========================================\n";
	cout << "          Deposit Menue Screen           \n";
	cout << "========================================\n\n";

	vector <stClient> vClients = GetClientsDataFromFile(ClientsDataFile);
	stClient Client;

	string AccNumber = EnterString("Please Enter Account Number ? ");

	int Amount;
	while (!FindClientByAccNumber(AccNumber, vClients, Client))
	{
		cout << "Client (" << AccNumber << ") Not Exist! \n";
		AccNumber = EnterString("Please Enter Account Number ? ");
	}
	DisplayClientData(Client);
	Amount = ReadNumber("Please Enter Amount ? ");

	DepositBalanceByAccNumber(AccNumber, Amount, vClients);

}

void WithdrawMenueScreen() {
	cout << "========================================\n";
	cout << "         Withdraw Menue Screen           \n";
	cout << "========================================\n\n";

	vector <stClient> vClients = GetClientsDataFromFile(ClientsDataFile);
	stClient Client;

	string AccNumber = EnterString("Please Enter Account Number ? ");

	while (!FindClientByAccNumber(AccNumber, vClients, Client)) {

		cout << "Client (" << AccNumber << ") Not Exist! \n";
		AccNumber = EnterString("Please Enter Account Number ? ");
	}

	DisplayClientData(Client);
	int Amount = ReadNumber("Please Enter Amount ? ");

	while (Amount > Client.Balance) {
		cout << "\nSorry Your Balanc is not Enough !\n";
		Amount = ReadNumber("Please Enter Another Amount ? ");
	}

	WithdrawBalanceByAccNumber(AccNumber, Amount, vClients);

}

void DisplayClientsBalances(vector <stClient> vClients) {

	int TotalBalances = 0;
	for (stClient& C : vClients)
	{
		cout << "| " << left << setw(15) << C.AccNumber;
		cout << "| " << left << setw(35) << C.Name;
		cout << "| " << left << setw(12) << C.Balance;
		cout << endl;
		TotalBalances += C.Balance;
	}
	cout << "________________________________________________________________________________________\n";

	cout << "\n\t\tTotal Balances = " << TotalBalances << endl;

}

void TotalBalancesMenueScreen() {

	vector <stClient> vClients = GetClientsDataFromFile(ClientsDataFile);

	cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";

	cout << "\n_______________________________________________________";
	cout << "_________________________________\n" << endl;
	cout << "| " << left << setw(15) << "Accout Number";
	cout << "| " << left << setw(35) << "Client Name";
	cout << "| " << left << setw(12) << "Balance";
	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________\n" << endl;

	if (vClients.size() > 0) {

		DisplayClientsBalances(vClients);
	}
	else
		cout << "\t\t\t\tNo Clients Available in the System !";


}

void BackToTransactionMenue() {

	cout << "\n\n\nPress Any Key to back to Transactions menue...";
	system("pause > 0");
	ShowTransactionMenueScreen();

}

void PerformTransactionMenueChoice(enTransactionChoice WhatChoice) {

	switch (WhatChoice)
	{
	case eDeposit:
		system("cls");
		DepositMenueScreen();
		BackToTransactionMenue();
		break;
	case eWithdraw:
		system("cls");
		WithdrawMenueScreen();
		BackToTransactionMenue();
		break;
	case eTotalBalances:
		system("cls");
		TotalBalancesMenueScreen();
		BackToTransactionMenue();
		break;
	case eMainMenue:
		ShowMainMenueScreen();
		break;
	default:
		break;
	}

}

void ShowTransactionMenueScreen() {


	if (!IsUserHasAccess(LoginUser.Perm, enUserPermessions::ShowTransPerm)) {
		system("CLS");
		ShowAccessDeniedScreen();
		return;
	}
	system("cls");

	cout << "========================================\n";
	cout << "        Transaction Menue Screen         \n";
	cout << "========================================\n";
	cout << "       [1] Deposit. \n";
	cout << "       [2] Withdraw. \n";
	cout << "       [3] TotalBalances. \n";
	cout << "       [4] Main Menue. \n";
	cout << "========================================\n";
	PerformTransactionMenueChoice((enTransactionChoice)ReadNumber("Chose what do you want to do ? [1 to 4] ?"));
}



//Users Menue

void AddNewUserScreen(vector<stUser> vUsers) {

	char AddMore = 'n';
	stUser AddedUser;

	cout << "\n------------------------------------------------------------------\n";
	cout << "                       Add New User Screen                          \n";
	cout << "\n------------------------------------------------------------------\n";

	AddUsers();

}

void ListUsersScreen(vector<stUser> vUsers) {

	cout << "\n";
	printf("				User List (%zi) User(a)				\n", vUsers.size());
	cout << "\n------------------------------------------------------------------\n";
	cout << "| " << setw(20) << left << "User Name" <<
		"| " << setw(18) << left << "Password" <<
		"| " << setw(18) << left << "Perm";
	cout << "\n------------------------------------------------------------------\n";

	for (stUser u : vUsers) {
		DisplayeUserRecord(u);
	}
	cout << endl;
}

void UpdateUserScreen(vector<stUser> vUsers) {

	cout << "\n------------------------------------------------\n";
	cout << "                Update User Screen                 ";
	cout << "\n------------------------------------------------\n\n";

	string Username;
	Username = EnterString("Enter Username? ");

	UpdateUserByUsername(Username, vUsers);

}

void FindUserScreen(vector<stUser> vUsers) {

	cout << "\n------------------------------------------------\n";
	cout << "                 Find User Screen                 ";
	cout << "\n------------------------------------------------\n\n";

	string Username = EnterString("Enter Username? ");
	stUser User;

	if (FindUserByUsername(Username, vUsers, User)) {

		DisplayUserData(User);

	}
	else
		cout << "\n\nUser Not Found!\n";

}

void DeleteUserScreen(vector<stUser> vUsers) {

	cout << "\n------------------------------------------------\n";
	cout << "                Delete User Screen                  ";
	cout << "\n------------------------------------------------\n\n";

	string UsernameForDelete = EnterString("Please Enter User? ");

	DeleteUserByUsername(UsernameForDelete, vUsers);
}

void BackToManageUsersScreen() {

	cout << "\n\n\nPress any key to back to Manage Users Screen......";
	system("pause > 0");
	ShowManageUsersScreen();
}

void PreformManageUsersChoice(enManageUserChoice WhatChoice) {

	vector<stUser> vUsers;
	vUsers = GetUsersDataFromFile(UsersDataFile);

	switch (WhatChoice)
	{
	case ListU:
		system("cls");
		ListUsersScreen(vUsers);
		BackToManageUsersScreen();
		break;
	case AddU:
		system("cls");
		AddNewUserScreen(vUsers);
		BackToManageUsersScreen();
		break;
	case DeleteU:
		system("cls");
		DeleteUserScreen(vUsers);
		BackToManageUsersScreen();
		break;
	case UpdateU:
		system("cls");
		UpdateUserScreen(vUsers);
		BackToManageUsersScreen();
		break;
	case FindU:
		system("cls");
		FindUserScreen(vUsers);
		BackToManageUsersScreen();
		break;
	case MainMenue:
		system("cls");
		ShowMainMenueScreen();
		break;
	default:
		system("cls");
		ShowManageUsersScreen();
		break;
	}

}

void ShowManageUsersScreen() {

	if (!IsUserHasAccess(LoginUser.Perm, enUserPermessions::ManageUsersPerm)) {
		system("CLS");
		ShowAccessDeniedScreen();
		return;
	}

	system("cls");
	cout << "=========================================\n";
	cout << "           Manage Users Screen           \n";
	cout << "=========================================\n";
	cout << "	[1] List Users.                   \n";
	cout << "	[2] Add New User.                 \n";
	cout << "	[3] Delete User.                  \n";
	cout << "	[4] Update User.                  \n";
	cout << "	[5] Find User.                    \n";
	cout << "	[6] Main Menue.                   \n";
	cout << "=========================================\n";

	PreformManageUsersChoice((enManageUserChoice)ReadNumber("Chose What do You Want [1 to 6]? "));

}

//void ExitScreen() {
//	cout << "========================================\n";
//	cout << "             Program Ends :-)            \n";
//	cout << "========================================\n";
//	system("pause > 0");
//}

void SetLoginUserData() {

	vector<stUser> vUsers = GetUsersDataFromFile(UsersDataFile);
	for (stUser& u : vUsers) {
		if (u.Username == LoginUser.Username) {
			LoginUser = u;
			break;

		}
	}
}

stUser FillUserData(stUser& User) {
	User.Username = EnterString("Enter Username : ");
	User.Password = EnterString("Enter Password : ");
	return User;
}

void RemoveLines(short lines) {
	for (short i = 0; i < lines; i++)
	{
		cout << "\033[A\033[K";
	}
}

void Login() {

	stUser User;
	short LinesCountToRemove = 2;
	system("cls");
	cout << "-----------------------------------\n";
	cout << "            Login Screen           \n";
	cout << "-----------------------------------\n";
	User = FillUserData(User);

	while (!IsUserExistByUsernameAndPassword(User.Username, User.Password))
	{
		RemoveLines(LinesCountToRemove);
		cout << "Invalid Username/Password!\n";
		User = FillUserData(User);
		if (LinesCountToRemove < 3)
			LinesCountToRemove++;
	}
	LoginUser = User;
	SetLoginUserData();

	ShowMainMenueScreen();

}


//collaps ctrl + M + O
//Expand  ctrl + M + L

int main() {

	Login();


	system("pause>0");
	return 0;
}


