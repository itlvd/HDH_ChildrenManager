#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <queue>
#include <sstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <windows.h>
#include <thread>
#include <conio.h>
#include <winuser.h>
#pragma warning(disable : 4996)


using namespace std;

const string TREEM = "treem";
const string BAME = "bame";

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "advapi32.lib")

BOOL MySystemShutdown()
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;

	// Get a token for this process. 

	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return(FALSE);

	// Get the LUID for the shutdown privilege. 

	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,
		&tkp.Privileges[0].Luid);

	tkp.PrivilegeCount = 1;  // one privilege to set    
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// Get the shutdown privilege for this process. 

	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
		(PTOKEN_PRIVILEGES)NULL, 0);

	if (GetLastError() != ERROR_SUCCESS)
		return FALSE;

	// Shut down the system and force all applications to close. 

	if (!ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0))
		return FALSE;

	//shutdown was successful
	return TRUE;
}


struct Time
{
	int hour = 0;
	int minute = 0;
	int second = 0;

	friend bool operator==(const Time& t1, const Time& t2) {					// operator to show show sort function how to build minHeap priority queu
		return (t1.hour == t2.hour && t1.minute == t2.minute);
	}

	friend bool operator!=(const Time& t1, const Time& t2) {					// operator to show show sort function how to build minHeap priority queu
		return (t1.hour != t2.hour || t1.minute != t2.minute);
	}

	friend bool operator<(const Time& t1, const Time& t2) {					// operator to show show sort function how to build minHeap priority queu
		if (t1.hour < t2.hour) return true;
		else if (t1.hour == t2.hour) {
			return t1.minute < t2.minute;
		}
		return false;
	}

	friend bool operator>(const Time& t1, const Time& t2) {					// operator to show show sort function how to build minHeap priority queu
		if (t1.hour > t2.hour) return true;
		else if (t1.hour == t2.hour) {
			return t1.minute > t2.minute;
		}
		return false;
	}

	friend bool operator<=(const Time& t1, const Time& t2) {					// operator to show show sort function how to build minHeap priority queu
		return (t1 < t2 || t1 == t2);
	}

	friend bool operator>=(const Time& t1, const Time& t2) {					// operator to show show sort function how to build minHeap priority queu
		return (t1 > t2 || t1 == t2);
	}

	friend int operator-(const Time& t1, const Time& t2) {					// operator to show show sort function how to build minHeap priority queu
		return (t1.hour * 3600 + t1.minute * 60 + t1.second) - (t2.hour * 3600 + t2.minute*60 + t2.second);
	}

	friend Time operator+(Time t1, int k) {					// operator to show show sort function how to build minHeap priority queu
		Time t;
		if (t1.hour + k >= 60) {
			int r = (t1.minute + k) / 60;
			t.minute = (t1.minute + k) % 60;
			t.hour = (t1.hour + r) % 24;
		}
		else {
			t.hour = t1.hour;
			t.minute = t1.minute + k;
		}
		return t;
	}
};


struct RegulationTime
{
	Time from;
	Time to;
	int duration = 0;
	int interrupt = 0;
	int sumTime = 0;	

	friend bool operator==(const RegulationTime& t1, const RegulationTime& t2) {					// operator to show show sort function how to build minHeap priority queu
		return (t1.from == t2.from && t1.to == t2.to && t1.duration == t2.duration && t1.interrupt==t2.interrupt && t1.sumTime == t2.sumTime);
	}

	friend bool operator!=(const RegulationTime& t1, const RegulationTime& t2) {					// operator to show show sort function how to build minHeap priority queu
		return !(t1 == t2);
	}

};


Time parstToTime(string token) {
	int posColon = token.find(":");
	string h = token.substr(1, posColon);
	string m = token.substr(posColon + 1);

	Time t;
	t.hour = stoi(h);
	t.minute = stoi(m);
	return t;
}

Time getTime() {
	Time t;
	time_t now = time(0);
	tm* ltm = localtime(&now);
	//t.hour = 1 + ltm->tm_hour;
	//t.minute = 1 + ltm->tm_min;
	t.hour = ltm->tm_hour;
	t.minute = ltm->tm_min;
	t.second = ltm->tm_sec;
	return t;
}

int minTime(int& a, int& b, int& c) {
	int m = a;
	if (m > b) m = b;
	if (m > c) m = c;
	return m;
}

RegulationTime tokenize(string s) {
	RegulationTime t;
	string delimiter = " ";

	size_t pos = 0;
	string token;
	while ((pos = s.find(delimiter)) != string::npos) {
		token = s.substr(0, pos);
		if (token[0] == 'F') {
			t.from = parstToTime(token);
		}
		else if (token[0] == 'T') {
			t.to = parstToTime(token);
		}
		else if (token[0] == 'D') {
			token = token.erase(0, 1);
			t.duration = stoi(token);
		}
		else if (token[0] == 'I') {
			token = token.erase(0, 1);
			t.interrupt = stoi(token);
		}
		else if (token[0] == 'S') {
			token = token.erase(0, 1);
			t.sumTime = stoi(token);
		}

		// std::cout << token << std::endl;
		s.erase(0, pos + delimiter.length());
	}
	// std::cout << s << std::endl;
	if (s[0] == 'F') {
		t.from = parstToTime(s);
	}
	else if (s[0] == 'T') {
		t.to = parstToTime(s);
	}
	else if (s[0] == 'D') {
		s = s.erase(0, 1);
		t.duration = stoi(s);
	}
	else if (s[0] == 'I') {
		s = s.erase(0, 1);
		t.interrupt = stoi(s);
	}
	else if (s[0] == 'S') {
		s = s.erase(0, 1);
		t.sumTime = stoi(s);
	}


	return t;
}


class Regulation
{
private:
	vector<RegulationTime> times;
	string fileData;
public:
	// constructor:
	Regulation(string filename) {
		fstream fi;
		string s;
		fi.open(filename);

		if (fi.is_open()) {
			while (!fi.eof()) {
				getline(fi, s);
				times.push_back(tokenize(s));
			}
		}
		fi.close();
		fileData = filename;
	}

public: 
	// method:

	bool hasUpdate() {
		//times.clear();
		vector<RegulationTime> tempTimes;
		fstream fi;
		string s;
		fi.open(this->fileData);

		if (fi.is_open()) {
			while (!fi.eof()) {
				getline(fi, s);				
				tempTimes.push_back(tokenize(s));
			}
		}
		fi.close();

		if (tempTimes.size() != times.size()) {
			times = tempTimes;
			return true;
		}

		bool updated = false;
		for (int i = 0; i < times.size(); i++) {
			if (times[i] != tempTimes[i]) {
				times[i] = tempTimes[i];
				updated = true;
			}
		}
		return updated;
	}


	friend bool isInUseTime(Time curTime, Time savedTime, Regulation &regu, Time &nextTime, int &index) {
		bool canUse = false;
		index = -1;
		if (curTime < savedTime) {
			nextTime = savedTime;
			return canUse;
		}
				
		for (int i = 0; i < regu.times.size(); i++) {
			if (curTime >= regu.times[i].from && curTime <= regu.times[i].to) {
				canUse = true;
				if (i < regu.times.size() - 1) {
					nextTime = regu.times[i + 1].from;
				}
				index = i;
				return canUse;
			}
		}

		for (int i = 0; i < regu.times.size(); i++) {
			if (curTime < regu.times[i].from) {
				nextTime = regu.times[i].from;
				index = i;
				break;
			}
		}
		
		return canUse;
	}

	RegulationTime getFromToTime(int i) {
		return times[i];
	}


	void printListReguTime() {
		for (int i = 0; i < times.size(); i++) {
			cout << times[i].from.hour << ":" << times[i].from.minute << "." << endl;
			cout << times[i].to.hour << ":" << times[i].to.minute << "." << endl;
			cout << times[i].duration << "." << endl;
			cout << times[i].interrupt << "." << endl;
			cout << times[i].sumTime << "." << endl;
			cout << endl;
		}
	}


};

class Saved
{
private:
	Time nextStart;
	Time saveTime;
	int duration;
	int sum;
	string saveFile;
	string remainFile;

public:
	// Constructor:
	Saved() {
		int duration = -1;
		int sum = -1;
		saveFile = "savedData.txt";
		remainFile = "saveRemain.txt";
	}

	Saved(string startFileName, string remainFileName) {
		fstream f1;
		f1.open(startFileName);
		if (f1.is_open()) {
			string s;
			getline(f1, s);
			if (s == "") {
				nextStart.hour = 0;
				nextStart.minute = 0;
			}
			else {
				int pos = s.find(" ");
				nextStart.hour = stoi(s.substr(0, pos));
				nextStart.minute = stoi(s.substr(pos + 1));;
			}
		}
		f1.close();

		fstream f2;
		f2.open(remainFileName);
		if (f2.is_open()) {
			f2 >> saveTime.hour;
			f2 >> saveTime.minute;
			f2 >> duration;
			f2 >> sum;			
		}
		f2.close();
		saveFile = startFileName;
		remainFile = remainFileName;
	}

	// Destructor:
	



public: //method
	int remainDuration() {
		return duration;
	}
	int remainSum() {
		return sum;
	}
	Time nextStartTime() {
		return nextStart;
	}

	Time savedTime() {
		return saveTime;
	}

	/*void update() {
		Saved(saveFile, remainFile);
	}*/

	void saveData(Time time, int& d, int& s) {
		nextStart = time;
		duration = d;
		sum = s;

		ofstream fo;
		fo.open(this->saveFile);
		if (fo.is_open()) {
			fo << nextStart.hour << " " << nextStart.minute;
		}
		fo.close();
		saveTime = getTime();
		fo.open(this->remainFile);
		if (fo.is_open()) {
			fo << saveTime.hour<<" ";
			fo << saveTime.minute << endl;
			fo << duration << endl;
			fo << sum;
		}
		fo.close();
	}
};


Regulation gRegu = Regulation("data.txt");
Saved gSaved = Saved("savedStart.txt", "saveRemain.txt");

Time currentTime;
//Time savedTime;
Time gNextTime;
RegulationTime gReguTimeNow;
int gIndexRegu;




class Children
{
private:
	Time startTime;
	int remainDurationTime;
	int remainSumTime;
	int interruptTime;
	Time nextTime;

public:
	// constructor:
	Children()
	{
		// sau nay phai lay getTime()
		startTime.hour = 0;
		startTime.minute = 0;
		remainDurationTime = 0;
		remainSumTime = -1;
		interruptTime = 0;
		nextTime.hour = 0;
		nextTime.minute = 0;
	}


	Children(int rd, int rs, Time savedTime, RegulationTime reguTime, Time nt)
	{
		startTime = getTime();
		if (savedTime < reguTime.from || savedTime > reguTime.to) {
			rd = rs = -1;
		}

		int remainUntilEnd = (reguTime.to.hour * 60 + reguTime.to.minute) - (startTime.hour * 60 + startTime.minute);
		if (reguTime.duration == 0) {
			remainDurationTime = remainUntilEnd;
		}
		else if (rd >= 0) {
			remainDurationTime = minTime(rd, remainUntilEnd, reguTime.duration);
		}
		else {
			remainDurationTime = min(remainUntilEnd, reguTime.duration);
		}
		/*if (reguTime.duration == 0) {
			remainDurationTime = (reguTime.to.hour*60 + reguTime.to.minute) - (startTime.hour*60 + startTime.minute);
		}
		else {
			if ((reguTime.to.hour * 60 + reguTime.to.minute) - (startTime.hour * 60 + startTime.minute) < rd) {
				remainDurationTime = (reguTime.to.hour * 60 + reguTime.to.minute) - (startTime.hour * 60 + startTime.minute);
			}
			else remainDurationTime = rd;
		}*/

		if (reguTime.sumTime == 0) {
			remainSumTime = (reguTime.to.hour * 60 + reguTime.to.minute) - (reguTime.from.hour * 60 + reguTime.from.minute);
		}
		else {
			if (rs == 0) {
				remainSumTime = 0;
			}
			else if (rs < 0) {
				remainSumTime = reguTime.sumTime;
			}
			else {
				int sumT = reguTime.to.hour * 60 + reguTime.to.minute - reguTime.from.hour * 60 + reguTime.from.minute;
				remainSumTime = minTime(rs, reguTime.sumTime, sumT);
			}
		}
		interruptTime = reguTime.interrupt;
		nextTime = nt;
	}

public: // Destructor
	~Children() {
		int d, su;
		if (remainDurationTime <= 0) {
			d = -1;
		}
		else {
			d = remainDurationTime;
		}

		if (remainSumTime <= 0) {
			su = remainSumTime;
		}
		else {
			su = remainSumTime;
		}
		if (su == 0) {
			gSaved.saveData(nextTime, d, su);
		}
		else if (su==-1){
			gSaved.saveData(nextTime, d, su);
		}
		else {
			gSaved.saveData(currentTime, d, su);
		}
	}

public:
	void setDuration(int d) {
		remainDurationTime = d;
	}

	void setSum(int s) {
		remainSumTime = s;
	}

	void setNextStart(Time ns) {
		nextTime = ns;
	}


	int getRemainDuration() {
		return remainDurationTime;
	}

	int getRemainSum() {
		return remainSumTime;
	}

	int getInterrupt() {
		return interruptTime;
	}

	Time getNextTime() {
		return nextTime;
	}


	void countDown() {
		remainDurationTime--;
		remainSumTime--;
	}

	void update() {
		startTime = getTime();

		if (gReguTimeNow.duration == 0) {
			remainDurationTime = (gReguTimeNow.to - gReguTimeNow.from)/60;
		}
		else remainDurationTime = min(remainDurationTime, gReguTimeNow.duration);
		
		if (gReguTimeNow.sumTime == 0) {
			remainSumTime = (gReguTimeNow.to - gReguTimeNow.from) / 60;
		}
		else remainSumTime = gReguTimeNow.sumTime;
		interruptTime = gReguTimeNow.interrupt;
		nextTime = gNextTime;
	}


};






Children tre;

int inputPassword()
{
	currentTime = getTime();
	string pass;
	
	
	MessageBox(NULL, L"Vui long nhap password", L"Thong bao", MB_OK | MB_ICONQUESTION | MB_SYSTEMMODAL);
	cout << "pass: ";
	getline(cin, pass);
	int count = 1;
	if (pass == BAME) {
		return 1;
	}

	if (!isInUseTime(currentTime, gSaved.nextStartTime(), gRegu, gNextTime, gIndexRegu)) {
		return 0;
	}

	while (isInUseTime(currentTime, gSaved.nextStartTime(), gRegu, gNextTime, gIndexRegu) && count < 3) {
		if (pass == TREEM) break;
		else {
			count++;
			cout << "pass: ";
			getline(cin, pass);
		}
		currentTime = getTime();
	}

	if (pass == TREEM) return -1;	// dung pass tre em
	return -2; // sai pass tre em

}

int checkUpdate() {
	if (!gRegu.hasUpdate()) return 0;	// khong co update
	
	currentTime = getTime();
	if (!isInUseTime(currentTime, gSaved.nextStartTime(), gRegu, gNextTime, gIndexRegu)) {
		tre = Children();
		return -1;	// co update, het han su dung
	}
	gReguTimeNow = gRegu.getFromToTime(gIndexRegu);
	tre.update();
	return 1;	// co update, van dc su dung
}





void count15second(Time &lastTime, bool isOver) {
	currentTime = getTime();
	if (currentTime - lastTime >= 15) {
		isOver = true;
		return;
	}
	isOver = false;
	return;
}

void inputSOS(bool &ok) {
	string s;
	cout << "pass: ";
	getline(cin, s);
	if (s == BAME)
		ok = true;
	return;
}

void foo()
{
	// do stuff...
	currentTime = getTime();
}

string parent;
void bar()
{
	// do stuff...
	system("cls");
	cout << "pass: ";
	getline(cin, parent);
}

void turnoffsreen(int second) {
	string s = "nircmd.exe monitor off";
	string on = "nircmd.exe monitor on";
	time_t now = time(0);
	tm* ltm = localtime(&now);
	cout << second << endl;
	int start = ltm->tm_sec;
	int end = start + second;
	cout << start << " end: " << end << endl;
	Time startTime = getTime();

	while (getTime() - startTime < second) {
		system(s.c_str());
		//now = time(0);
		//ltm = localtime(&now);
		//start = ltm->tm_sec;
		//cout << start << endl;
	}

	system(on.c_str());

}

//===============================================Code chua upadte================================
/*Lấy dir để lưu, hàm trả về string bao gồm thư mục ( ngày tháng ) + tên file(giờ:phút).png*/
string getDirCapture() {
	string s;
	time_t now = time(0);
	tm* ltm = localtime(&now);

	string dir = to_string(ltm->tm_year + 1900) + "_" + to_string(ltm->tm_mon + 1) + "_" + to_string(ltm->tm_mday);
	string filename = to_string(ltm->tm_hour) + "_" + to_string(ltm->tm_min) + ".png";

	//string mkdir = "mkdir history\\" + dir;
	//system(mkdir.c_str());

	//s = "\".\\history\\" + dir + "\\" + filename + "\"";
	//s = "C:\\Users\\thien\\OneDrive - VNU - HCMUS\\19127294_19127363_19127100_19127083\\data\\" + filename;
	//s = "\".\\data\\" + dir + "\\" + filename + "\"";

	string mkdir = "mkdir .\\..\\..\\data\\history";
	system(mkdir.c_str());
	s = "\".\\..\\..\\data\\history\\" + filename + "\"";
	cout << s;
	return s;
}

void scaleIMG(string src, int height, int width) {
	string des = src;
	des.replace(des.end() - 4, des.end() - 1, "jpg");
	string s = "call scale.bat -source " + src + " -target " + des + " -max-height " + to_string(height) + " -max-width " + to_string(width) + " -keep-ratio no -force yes";
	system(s.c_str());
}

/*Truyền vào dir, sau đó chụp màn hình*/
void captureScreen() {
	while (true) {
		if (tre.getRemainSum() == 0) {
			break;
		}
		string dir = getDirCapture();
		string s = "nircmd.exe savescreenshot " + dir;
		system(s.c_str());
		scaleIMG(dir, 450, 900);
		this_thread::sleep_for(chrono::seconds(60));
	}
}



void checkUpdateData() {

	while (true) {
		if (tre.getRemainSum() == 0) {
			break;
		}
		int updateStatus = checkUpdate();
		
		if (updateStatus == 0) {
			cout << "\nko co update nha" << endl;
		}
		else if (updateStatus == -1) {
			cout << "vua moi update" << endl;
			cout << "da het han su dung, bye";
			// shut down
			//exit(0);
			MySystemShutdown();
		}
		else if (updateStatus == 1) {
			cout << "\nco update nha" << endl;
			cout << "\nthoi gian con lai: " << tre.getRemainDuration() << endl;
			cout << "tong thoi gian con lai: " << tre.getRemainSum() << endl;
		}
		this_thread::sleep_for(chrono::seconds(60));
	}
}

void notificate() {
	while (true) {
		
		currentTime = getTime();
		if (tre.getRemainDuration() <= 0 && tre.getRemainSum() >= 1) {
			//log off	
			turnoffsreen(tre.getInterrupt() * 60);
			tre.setDuration(gReguTimeNow.duration);
			//tre.setSum(tre.getRemainSum() + gReguTimeNow.interrupt);
		}

		if (tre.getRemainSum() == 1)
		{
			cout << "\n1 phut nua tat may" << endl;
			wstring mess = L"Thong bao: 1 phut nua tat may";
			wstring tittle = L"Time su dung tiep theo tu: "
				+ to_wstring(gNextTime.hour) + L":" + to_wstring(gNextTime.minute);
			

			MessageBox(NULL, tittle.c_str(), mess.c_str(), MB_OK | MB_ICONQUESTION | MB_SYSTEMMODAL);
			
			
			// thong bao next time
			cout << "\nthoi gian duoc phep su dung bat dau tu: " << gNextTime.hour << ":" << gNextTime.minute << endl;
			cout << endl;
			
		}		
		else if (tre.getRemainSum() == 0) {
			break;
		}
		this_thread::sleep_for(chrono::seconds(60));
		tre.countDown();
	}
}

bool isLogin = false;

void countDown(int& time) {
	while (time >= 0) {
		this_thread::sleep_for(chrono::seconds(1));
		time -= 1;
		
	}
	if (!isLogin) {
		cout << "end game" << endl;

		cout << "\nPhat 10 phut" << endl;
		gNextTime = currentTime + 10;
		/*Time alarm = currentTime + 10;
		int tempD = -1, tempS = -1;
		gSaved.saveData(alarm, tempD, tempS);*/
		//exit(0);
		MySystemShutdown();
	}
}

void inputThread(string Tpass) {
	string pass;
	while (pass != Tpass) {
		cout << "Enter pass: ";
		getline(cin, pass);
		if (pass != Tpass)
			cout << "Wrong pass. Try again\n";
		else {
			isLogin = true;
		}
	}
}


//void nofitication(const string &title, const string& message) {
//	MessageBox(NULL, message.c_str(), title.c_str(), MB_OK | MB_ICONQUESTION | MB_SYSTEMMODAL);
//}



int main()
{
	int status = 1;
	Time lastParent, lastTime;
	lastTime = lastParent = currentTime = getTime();
	
	do {
		isLogin = false;
		if ((currentTime - lastParent) % (1 * 60) == 0) {
			if ((currentTime - lastParent) == 2 * 60) {
				cout << "check again nhaa:" << endl << endl;
				lastParent = currentTime;
			}
			cout << "cur - lastparent: " << currentTime - lastParent << endl;
			status = inputPassword();


			switch (status)
			{
			case 1:
			{
				// parent
				cout << "parent" << endl;
				//lastCheck++;
				break;
			}
			case 0:
			{
				int timeCountDown = 15;
				// khong trong thoi gian su dung:
				wstring tittle1 = L"Khong duoc dung";
				wstring mess1 = L"quay lai luc" + to_wstring(gNextTime.hour) + L":" + to_wstring(gNextTime.minute);
				cout << "ko duoc dung" << endl;
				MessageBox(NULL, tittle1.c_str(), mess1.c_str(), MB_OK | MB_ICONQUESTION | MB_SYSTEMMODAL);
				thread t1(inputThread, BAME);
				thread t2(countDown, ref(timeCountDown));
				
				t1.join();
				//shutdown
				
				//t2.join();
				//if (is)
				//t2.join();
				if (isLogin == true) {
					t2.join();
					status = 1;
					lastTime = lastParent = currentTime = getTime();
				}
				/*else {
					MySystemShutdown();
				}*/
				break;
			}
			case -1:		// nhap dung pass tre em:
			{
				gReguTimeNow = gRegu.getFromToTime(gIndexRegu);
				tre = Children(gSaved.remainDuration(), gSaved.remainSum(), gSaved.savedTime(), gReguTimeNow, gNextTime);
				cout << "tre dang dung may" << endl;
				cout << "thoi gian con lai: " << tre.getRemainDuration() << endl;
				cout << "tong thoi gian con lai: " << tre.getRemainSum() << endl;
				cout << endl;

				wstring mess2 = L"tong thoi gian con lai: " + to_wstring(tre.getRemainSum());
				wstring tittle2 = L"Time su dung tiep theo tu: "
					+ to_wstring(gNextTime.hour) + L":" + to_wstring(gNextTime.minute);


				MessageBox(NULL, tittle2.c_str(), mess2.c_str(), MB_OK | MB_ICONQUESTION | MB_SYSTEMMODAL);



				//while (tre.getRemainDuration() > 0 && tre.getRemainSum() > 0) {
				//bool isOff = false;
				string se = getDirCapture();


				thread check(checkUpdateData);
				thread capture(captureScreen);

				//while (tre.getRemainSum() > 0) {
					//currentTime = getTime();


				thread notify(notificate);

				notify.join();
				capture.join();
				check.join();

				
				//shutdown
				MySystemShutdown();

				//Sleep(60000);

				//if (tre.getRemainDuration() <= 0) {
				//	//log off	
				//	turnoffsreen(tre.getInterrupt() * 60);
				//	tre.setDuration(gReguTimeNow.duration + gReguTimeNow.interrupt);
				//	//tre.setSum(tre.getRemainSum() + gReguTimeNow.interrupt);
				//}



				//if (currentTime - lastTime >= 1*60) {
				//	//tre.countDown();

				//	// chup man hinh:						
				//	
				//	//captureScreen(se);

				//	

				//	// neu co thay doi:

				//	/*cout << "tre dang dung may" << endl;
				//	cout << "thoi gian con lai: " << tre.getRemainDuration() << endl;
				//	cout << "tong thoi gian con lai: " << tre.getRemainSum() << endl;
				//	cout << endl;*/
				//	/*int updateStatus = checkUpdate();

				//	if (updateStatus == -1) {
				//		cout << "vua moi update" << endl;
				//		cout << "da het han su dung, bye";
				//		break;
				//	}
				//	else if (updateStatus == 1) {
				//		cout << "thoi gian con lai: " << tre.getRemainDuration() << endl;
				//		cout << "tong thoi gian con lai: " << tre.getRemainSum() << endl;
				//	}*/

				//	//if (tre.getRemainSum() == 1)
				//	//{
				//	//	cout << "\n1 phut nua tat may" << endl;
				//	//	// thong bao next time
				//	//	cout << "\nthoi gian duoc phep su dung bat dau tu: " << gNextTime.hour << ":" << gNextTime.minute << endl;
				//	//	cout << endl;
				//	//}
				//	//Sleep(2000);
				//	
				lastTime = currentTime;
				//}
			//}
			/*int d, su;
			if (tre.getRemainDuration() <= 0) {
				d = -1;
			}
			else {
				d = tre.getRemainDuration();
			}

			if (tre.getRemainSum() <= 0) {
				su = 0;
			}
			else {
				su = tre.getRemainSum();
			}
			if (su == 0) {
				gSaved.saveData(tre.getNextTime(), d, su);
			}
			else {
				gSaved.saveData(currentTime, d, su);
			}*/


			break;
			}
			case -2: {
				MySystemShutdown();
				break;
			}
		
			default:
				break;
			}
		}
		//Sleep(1000);
		currentTime = getTime();
		
	} while (status == 1);
	//MySystemShutdown();
	return 0;
}