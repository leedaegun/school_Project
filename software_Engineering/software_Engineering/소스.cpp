
// 헤더 선언

#include <iostream>
#include <cstdio>
#include <cstring>


// 상수 선언

#define INPUT_FILE_NAME "input.txt"
#define OUTPUT_FILE_NAME "output.txt"
#define MAX_STRING 32            // 정보 최대 입력 길이
#define MAX_MEMBER_NUM 512         // 회원 정보를 보관할 Member 배열의 최대 크기
#define MAX_ACC_NUM 512            //숙소 정보를 보관할 Accomodation 배열의 최대 크기
#define MAX_RESERVATION_NUM 512      // 예약 정보를 보관할 ReservationInfo 배열의 최대 크기
#define HOST "host"         

FILE* in_fp; // input.txt 포인터
FILE* out_fp; // output.txt 포인터


			  // 함수 선언

void doTask();
void changeSession();      //입력한 id로 세션 변경 함수
void changeGuest();        //게스트 세션으로 변경 함수
void program_exit();      // 프로그램 종료 함수


						  // 전역 변수 선언

char currentTime[MAX_STRING]; //현재시간


							  // 클래스 선언

							  // 회원 클래스
							  // 회원가입한 회원들의 정보를 보관한다
							  // Created : 2018/5/20
							  // Author : 김형주

							  // Revsions :
							  // 1. When & Who : 2018/05/26 21:31 pm 김지현
							  // 내용 : char*포인터 MAX_STRING 배열로 바꿈,멤버 생성자및 set함수 를 기존 =으로 받아온것 strcpy로바꿈
							  // 수정 된 set함수들, 생성자, private attribute
class Member {

private:
	char userType[MAX_STRING];
	char name[MAX_STRING];
	char ssn[MAX_STRING];
	char address[MAX_STRING];
	char id
		[MAX_STRING];
	char password[MAX_STRING];
	char oiDate[MAX_STRING];
	int registerAccNum = 0;

public:
	Member();
	Member(char userType[], char name[], char ssn[], char address[], char id[], char password[]);
	char* getUserType();
	void setUserType(char*);
	char* getName();
	void setName(char*);
	char* getSSN();
	void setSSN(char*);
	char* getAddress();
	void setAddress(char*);
	char* getID();
	void setID(char*);
	char* getOIDate();
	void setOIDate(char*);
	char* getPassword();
	void setPassword(char*);
	int getRegisterAccNum();
	void setRegisterAccNum();
	bool isValidSignUp(char*, char*);   // 회원 가입 정보 조사 함수 - ID 중복, 주민번호 길이를 체크
	void createMember(char*, char*,
		char*, char*, char*, char*);	// 회원 가입 성공 시 사용자 정보를 Member 배열에 저장하는 함수
	void deleteMember(); //멤버 삭제
	bool isValidLogin(char*, char*);	// 로그인 유효여부 확인 함수 - ID , 비밀번호가 멤버클래스에 존재하는지 확인
};
// 기본 생성자 - 모든 값을 NULL로 초기화
Member::Member() {

}
Member::Member(char userType[], char name[], char ssn[], char address[], char id[], char password[]) {
	strcpy(this->userType, userType);
	strcpy(this->name, name);
	strcpy(this->ssn, ssn);
	strcpy(this->address, address);
	strcpy(this->id, id);
	strcpy(this->password, password);
	strcpy(this->oiDate, "");
}
// 모든 멤버 변수 각각의 getter, setter
char* Member::getUserType() {
	return userType;
}
void Member::setUserType(char* userType) {
	strcpy(this->userType, userType);
}
char* Member::getName() {
	return this->name;
}
void Member::setName(char* name) {
	strcpy(this->name, name);
}
char* Member::getSSN() {
	return ssn;
}
void Member::setSSN(char* ssn) {
	strcpy(this->ssn, ssn);
}
char* Member::getAddress() {
	return this->address;
}
void Member::setAddress(char* address) {
	strcpy(this->address, address);
}
char* Member::getID() {
	return id;
}
void Member::setID(char* id) {
	strcpy(this->id, id);
}
char* Member::getPassword() {
	return this->password;
}
void Member::setPassword(char* password) {
	strcpy(this->password, password);
}char* Member::getOIDate() {
	return this->oiDate;
}
void Member::setOIDate(char*oiDate) {
	strcpy(this->oiDate, oiDate);
}
int Member::getRegisterAccNum() {
	return this->registerAccNum;
}
void Member::setRegisterAccNum() {
	this->registerAccNum++;
}

Member memberArray[MAX_MEMBER_NUM] = {}; // 회원 정보 관리를 위한 Member(회원) 배열
int memberNumber = -1;   // 여러 회원 중 session 관리를 위한 회원번호 식별자
						 // 초기 session은 -1, 사용자가 사용 중일 때는 해당 사용자의 회원 번호를 가짐
int memberCount = 0;//가입한 총 회원 수

char* tempAcc[MAX_STRING]; //숙소 정렬할때 사용할 배열


						   // 1.1 회원가입 양식 검사 함수
						   // 회원가입 시 작성한 정보를 체크한다 ID가 중복되거나 주민번호 길이가 맞지 않을 경우 false를 반환한다
						   // 입력값 : 아이디, 주민번호
						   // 출력값 : 양식을 검사한뒤 이상이 없으면 true, 있으면 false
						   // Created : 2018/5/22
						   // Author : 김형주

						   // Revsions :
						   // 1. When & Who : 2018/05/26 21:24 pm 김지현
						   // 내용 : 회원가입 성공시 로그인까지 되는 memberNumber 수 변경시키는거 빼고 i값으로 대체
						   //회원가입 성공시 memberNumber를 i로 바꾸고 memberArray[memberNumber] = 
						   //Member(userType, name, ssn, address, id, password);로 했는데 
						   //그것에서 memberNumber는 로그인으로 넘기고 i값으로 바꿈
						   // 수정 된 것 조건문 memberCount 추가, memberNumber->i, == -> strcmp 로 바꿈
bool Member::isValidSignUp(char* id, char* ssn) {
	for (int i = 0; i < memberCount; i++) {
		if (strcmp(memberArray[i].getID(), id) == 0)
			return false;
		if (strlen(ssn) != 6)
			return false;
	}
	return true;
}

// 1.1 회원가입 성공시 정보 저장
// 회원가입 성공 시 회원 정보를 Member 배열에 저장
// 입력값 : 유저유형, 이름, 주민번호, 주소, 아이디, 비밀번호 총 6가지의 문자열
// Created : 2018/5/22
// Author : 김형주

// Revsions :
// 1. When & Who : 2018/05/26 18:32 pm 김지현
// 내용 : 회원가입 성공시 로그인까지 되는 memberNumber 수 변경시키는거 빼고 i값으로 대체
//회원가입 성공시 memberNumber를 i로 바꾸고 memberArray[memberNumber] = Member(userType, name, ssn, address, id, password);
//로 했는데 그것에서 memberNumber는 로그인으로 넘기고 i값으로 바꿈, 회원가입한 총 회원 개수 세는 변수 memberCount 추가
//아직 들어가지 않은 멤버의 아이디는 디버깅한 결과 ""로 나와서 그것과 같으면 멤버가 없다는 것이니 거기다가 추가하는
//(strcmp(memberArray[i].getID(),"") == 0) 추가
// 수정 된 것 memberNumber -> i ,memberCount 추가, 회원가입 아이디 비교문 ==에서 strcmp로 수정
void Member::createMember(char* userType, char* name, char* ssn, char* address, char* id, char* password) {
	// Member 배열에서 비어 있는 곳을 찾아 사용자 정보를 저장한다.
	int i;
	for (i = 0; i < MAX_MEMBER_NUM; i++) {
		if (strcmp(memberArray[i].getID(), "") == 0) {
			memberCount++;
			break;
		}
	}
	memberArray[i] = Member(userType, name, ssn, address, id, password);
	return;
}

// 1.2 회원 탈퇴 시 회원 정보 삭제 함수
// 회원 정보를 모두 NULL값으로 바꾸고 guest session으로 복귀한다
// 회원 수를 카운트하는 memberCount의 값은 1을 줄여준다
// Created : 2018/5/30
// Author : 김형주
void Member::deleteMember() {

	strcpy(this->userType, "");
	strcpy(this->address, "");
	strcpy(this->id, "");
	strcpy(this->ssn, "");
	strcpy(this->name, "");
	strcpy(this->password, "");
	strcpy(this->oiDate, "");
	this->registerAccNum = 0;
	memberNumber = -1;
	memberCount--;
}

// 2.1 로그인 유효여부 검사 함수
// bool isValidLogin(char* id, char* ssn)
// 로그인 시 작성한 id와 비밀번호를 보고 ID가 등록되어있지 않거나 id와 비밀번호가 일치하지 않을 경우 false를 반환한다
// 매개변수 : char * id - 아이디, char * password - 비밀번호
// 반환값 : 로그인 유효여부 검사한 뒤 이상이 없으면 true, 있으면 false
// Created : 2018/5/26
// Author : 김지현
// Revsions :
// 1. When & Who : 2018/05/27  이대근
// 내용 : 로그인이 되는 상황에서 유저타입이 호스트인경우
// 유저타입 체크를 해둠 -> 이후 숙소등록등 호스트권한 설정
// Revsions :
// 1. When & Who : 2018/05/29 20:21 pm  김지현
// 내용 : 유저타입은 필요시 갖고오면 되므로 유저타입체크 부분 삭제
bool Member::isValidLogin(char* id, char* password) {
	bool valid = false;

	for (int i = 0; i < memberCount; i++) {
		if (!strcmp(memberArray[i].getID(), id)) {
			if (!strcmp(memberArray[i].getPassword(), password)) {
				memberNumber = i;
				valid = true;
				break;
			}
		}
	}
	return valid;
}


// 클래스 선언

// 클래스 : Accomodation
// 등록한 숙소의 정보
//(숙소 이름, 숙소 주소, 숙소 가격, 이용 가능 날짜, 등록 날짜, OI가격, 예약유무,호스트ID)를 보관한다
// 숙소와 관련된 정보는 숙소 클래스에서 가져온다.
// Created : 2018/5/26
// Author : 이대근

// Revsions :
// 1. When & Who : 2018/05/29 21:47 pm 김지현
// 내용 : 등록시간과 예약가능시간을 분리
// 수정된것 : reservationPossibleDate 배열 추가
class Accomodation {

private:
	char accName[MAX_STRING];
	char accAddress[MAX_STRING];
	char accPrice[MAX_STRING];
	char registerDate[MAX_STRING];
	char reservationPossibleDate[MAX_STRING];
	char oiPrice[MAX_STRING];
	char reservation;
	char hostID[MAX_STRING];

public:
	Accomodation();
	Accomodation(char accName[], char accAddress[], char accPrice[], char registerDate[], char oiPrice[], char currentTime[]);
	char* getAccName();
	void setAccName(char*);
	char* getAccAddress();
	void setAccAddress(char*);
	char* getAccPrice();
	void setAccPrice(char*);
	char* getRegisterDate();
	void setRegisterDate(char*);
	char* getReservationPossibleDate();
	void setReservationPossibleDate(char*);
	char* getOiPrice();
	void setOiPrice(char*);
	char getReservation();
	void setReservation(char);
	char* getHostID();
	void setHostID(char*);
	void registerHostAcc(char*, char*, char*, char*, char*, char*, char*);
	void getHostAcc();
	void searchGuestAcc(char*, char*);
	void deleteAcc();
	bool isAnyAccomodation();
	bool checkOICondition(char*, char*, char*);
};

Accomodation accArray[MAX_ACC_NUM] = {};
Accomodation availableAccList[MAX_ACC_NUM] = {};

// 기본 생성자
Accomodation::Accomodation() {

}
Accomodation::Accomodation(char accName[], char accAddress[], char accPrice[], char reservationPossibleDate[], char oiPrice[], char currentTime[]) {
	strcpy(this->accAddress, accAddress);
	strcpy(this->accName, accName);
	strcpy(this->accPrice, accPrice);
	strcpy(this->reservationPossibleDate, reservationPossibleDate);
	strcpy(this->oiPrice, oiPrice);
	this->reservation = 'X';
	strcpy(this->hostID, "");
	strcpy(this->registerDate, currentTime);
}
// 모든 멤버 변수 각각의 getter, setter
char* Accomodation::getAccAddress() {
	return this->accAddress;
}
void Accomodation::setAccAddress(char* accAddress) {
	strcpy(this->accAddress, accAddress);
}

char* Accomodation::getAccName() {
	return this->accName;
}
void Accomodation::setAccName(char* accName) {
	strcpy(this->accName, accName);
}
char* Accomodation::getAccPrice() {
	return this->accPrice;
}
void Accomodation::setAccPrice(char* accPrice) {
	strcpy(this->accPrice, accPrice);
}

char* Accomodation::getRegisterDate() {
	return this->registerDate;
}
void Accomodation::setRegisterDate(char* registerDate) {
	strcpy(this->registerDate, registerDate);
}
char* Accomodation::getReservationPossibleDate() {
	return this->reservationPossibleDate;
}
void Accomodation::setReservationPossibleDate(char* reservationPossibleDate) {
	strcpy(this->reservationPossibleDate, reservationPossibleDate);
}
char* Accomodation::getOiPrice() {
	return this->oiPrice;
}
void Accomodation::setOiPrice(char* oiPrice) {
	strcpy(this->oiPrice, oiPrice);
}
char Accomodation::getReservation() {
	return this->reservation;
}
void Accomodation::setReservation(char reservation) {
	this->reservation = reservation;
}
char* Accomodation::getHostID() {
	return this->hostID;
}
void Accomodation::setHostID(char* hostID) {
	strcpy(this->hostID, hostID);
}

// 함수 : deleteAcc()
// 설명 : 등록했던 숙소를 지운다.

// 작성일 : 2018/5/29
// Author : 김지현

// Revsions :
// 1. When & Who : 2018/05/30  이대근
// 내용 : 예약여부를 NULL로 바꿈
// 수정된 것 : this->reservation ='X' =>   this->reservation = NULL;
void Accomodation::deleteAcc() {
	strcpy(this->accAddress, "");
	strcpy(this->accName, "");
	strcpy(this->accPrice, "");
	strcpy(this->reservationPossibleDate, "");
	strcpy(this->oiPrice, "");
	this->reservation = NULL;
	strcpy(this->hostID, "");
	strcpy(this->registerDate, "");
}

// 함수 : registerHostAcc(char* accName, char* accAddress, char* accPrice, char* reservationPossibleDate, char* oiPrice, char* hosdID, char*currentTime)
// 설명 : 등록시 숙소 이름,숙소 주소,숙소가격,이용날짜,OI가격을 input.txt로 입력받아
// 숙소 클래스에 저장한다
// 등록시 예약 유무는 'X'로하고 로그인 되어있는 호스트ID를 등록한다.
//
// 작성일 : 2018/5/27
// Author : 이대근
void Accomodation::registerHostAcc(char* accName, char* accAddress, char* accPrice, char* reservationPossibleDate, char* oiPrice, char* hosdID, char*currentTime) {

	setAccName(accName);
	setAccAddress(accAddress);
	setAccPrice(accPrice);
	setRegisterDate(currentTime);
	setOiPrice(oiPrice);
	setReservation('X');
	setHostID(memberArray[memberNumber].getID());
	setReservationPossibleDate(reservationPossibleDate);

	memberArray[memberNumber].setRegisterAccNum();//이후 정렬에 필요한 숙소개수를 위해 등록한 숙소 개수 계산

	fprintf(out_fp, "%s %s %s %s %s\n", accName, accAddress, accPrice, reservationPossibleDate, oiPrice);
}

int count = 0;			//임시 배열에 쓰일 인덱스 카운터
char** temp[MAX_STRING];//정렬을 위한 임시 배열

						// 함수 : getHostAcc()
						// 설명 : 숙소 조회시 등록되어 있는 숙소정보를 가져온다.
						// 정보는 이후 정렬을 위해 임시 배열에 저장한다.
						// 조회시 호스트가 자신이 등록한 숙소만 조회 할 수 있도록 한다. 
						// 작성일 : 2018/5/27
						// Author : 이대근

void Accomodation::getHostAcc() {

	if (strcmp(memberArray[memberNumber].getID(), getHostID()) == 0) {//본인의 ID와 숙소에 등록된ID가 같을때
																	  //즉 본인이 등록한 숙소만 조회
		tempAcc[count++] = getAccName();
		tempAcc[count++] = getAccAddress();
		tempAcc[count++] = getReservationPossibleDate();
		tempAcc[count++] = getAccPrice();
		if (getReservation() == 'X')		//getReservation의 리턴 값은 char형 임으로 임시 배열에 넣을 수 없음으로
			tempAcc[count++] = "X";			//조건문을 만들어 그에 맞는 예약 유무 정보를 넣어준다.
		else
			tempAcc[count++] = "O";

		tempAcc[count++] = getOiPrice();
	}
}

// 함수 : searchGuestAcc(char*accAddress, char*reservationDate)
// 설명 : 게스트가 예약 가능한 숙소를 검색한다. 
// 입력 값으로 숙소 주소와 예약 날짜를 입력한다
// 결과 값으로 예약가능한 숙소 정보 (호스트ID 숙소이름 숙소주소 가격)을 출력한다.
//
// 작성일 : 2018/5/27
// Author : 이대근
void Accomodation::searchGuestAcc(char*accAddress, char*reservationDate) {

	char *Address;
	char *reservationPossibleDate;
	char *accPrice;
	char *hostID;
	char *accName;
	int j = 0;
	char *oiPrice;

	if (getReservation() == 'X') {//예약이 되어있지않은 숙소, 예약 유무가 'X'일 경우

		Address = getAccAddress();
		reservationPossibleDate = getReservationPossibleDate();

		if ((strcmp(Address, accAddress) == 0) && (strcmp(reservationPossibleDate, reservationDate) == 0)) {//검색한 숙소주소와 예약날짜가 같은 숙소 찾기

			accName = getAccName();
			hostID = getHostID();
			accPrice = getAccPrice();
			oiPrice = getOiPrice();

			fprintf(out_fp, "%s %s %s %s\n", hostID, accName, Address, accPrice);//호스트 숙소이름 숙소주소 가격 
																				 // 숙소 검색 결과를 리스트로 저장. 숙소 예약에 사용. (숙소이름, 숙소주소, 숙소가격, 예약원하는날짜, oi가격)
			availableAccList[j].setAccName(accName);
			availableAccList[j].setAccAddress(accAddress);
			availableAccList[j].setAccPrice(accPrice);
			availableAccList[j].setRegisterDate(reservationDate);
			availableAccList[j].setOiPrice(oiPrice);
			availableAccList[j].setHostID(hostID);
			j++;
		}
	}
}

// 1.2 회원탈퇴 시 등록한 숙소가 있는지 확인하는 함수
// host 회원일 시 등록 숙소를 확인해서 숙소가 존재하면 true, 없으면 false를 반환한다
// Created : 2018/5/30
// Author : 김형주
bool Accomodation::isAnyAccomodation() {
	int count = memberArray[memberNumber].getRegisterAccNum();
	if (count == 0)
		return false;
	else
		return true;
}

// 함수 : checkOICondition(char*accAddress, char* reservationDate, char* cost)
// 설명 : 게스트가 Opaque inventory 예약할 때 숙소 객체가 게스트가 적은 조건에 적합한지 검사하여
// true나 false값을 반환한다.
// 매개변수 :char*accAddress -게스트가 쓴 주소, char* reservationDate - 게스트 희망예약날짜, char* cost -게스트의 희망가격
// 작성일 : 2018/5/31
// Author : 김지현
bool Accomodation::checkOICondition(char*accAddress, char* reservationDate, char* cost) {

	if (strcmp(getAccAddress(), accAddress) == 0 && strcmp(getReservationPossibleDate(), reservationDate) == 0 && strcmp(getOiPrice(), cost) < 0 && getReservation() == 'X'&&strcmp(getOiPrice(), "0") != 0) {
		return true;
	}
	else {
		return false;
	}
}

Accomodation accomodation;

// 클래스 선언
// 예약정보 클래스
// 등록한 숙소의 정보를 보관한다
// Created : 2018/5/27
// Author : 배철훈
class ReservationInfo {

private:
	char guestID[MAX_STRING];
	char reservationDate[MAX_STRING];
	char hostID[MAX_STRING];
	char accName[MAX_STRING];
	char accAddress[MAX_STRING];
	char accPrice[MAX_STRING];

public:
	ReservationInfo();
	ReservationInfo(char guestID[], char reservationDate[], char hostID[], char accName[], char accAddress[], char accPrice[]);
	char* getGuestID();
	void setGuestID(char*);
	char* getReservationDate();
	void setReservationDate(char*);
	char* getHostID();
	void setHostID(char*);
	char* getAccName();
	void setAccName(char*);
	char* getAccAddress();
	void setAccAddress(char*);
	char* getAccPrice();
	void setAccPrice(char*);
	void makeReservation(char*, char*);
	void showReservationList();
	bool isAnyReservation();
	void autoCreateReservation(char*, char*, char*, char*, char*, char*);
};
ReservationInfo reservationInfoArray[MAX_RESERVATION_NUM] = {};

ReservationInfo::ReservationInfo() {

}
ReservationInfo::ReservationInfo(char guestID[], char reservationDate[], char hostID[], char accName[], char accAddress[], char accPrice[]) {
	strcpy(this->guestID, guestID);
	strcpy(this->reservationDate, reservationDate);
	strcpy(this->hostID, hostID);
	strcpy(this->accName, accName);
	strcpy(this->accAddress, accAddress);
	strcpy(this->accPrice, accPrice);
}
char* ReservationInfo::getGuestID() {
	return this->guestID;
}
void ReservationInfo::setGuestID(char* guestID) {
	strcpy(this->guestID, guestID);
}
char* ReservationInfo::getReservationDate() {
	return this->reservationDate;
}
void ReservationInfo::setReservationDate(char* reservationDate) {
	strcpy(this->reservationDate, reservationDate);
}
char* ReservationInfo::getHostID() {
	return this->hostID;
}
void ReservationInfo::setHostID(char* hostID) {
	strcpy(this->hostID, hostID);
}
char* ReservationInfo::getAccName() {
	return this->accName;
}
void ReservationInfo::setAccName(char* accName) {
	strcpy(this->accName, accName);
}
char* ReservationInfo::getAccAddress() {
	return this->accAddress;
}
void ReservationInfo::setAccAddress(char* accAddress) {
	strcpy(this->accAddress, accAddress);
}
char* ReservationInfo::getAccPrice() {
	return this->accPrice;
}
void ReservationInfo::setAccPrice(char* accPrice) {
	strcpy(this->accPrice, accPrice);
}

// 숙소 예약하기 함수
// 예약가능한 숙소 검색하기 결과리스트에서 사용자가 예약을 원하는 숙소를 선택할경우
// 해당 숙소에 대한 정보를 저장한 배열에서 값을 가져와 ReservationInfo 에 추가한다.
// Created : 2018/5/31
// Author : 배철훈
void ReservationInfo::makeReservation(char* hostID, char* accName) {
	int reservationNumber;
	char* reservationDate;
	char* accAddress;
	char* accPrice;

	for (int i = 0; i < MAX_RESERVATION_NUM; i++) {
		if (strcmp(reservationInfoArray[i].getAccName(), "") == 0) {
			reservationNumber = i;
			break;
		}
	}

	for (int j = 0; j < MAX_ACC_NUM; j++) {
		if ((strcmp(hostID, availableAccList[j].getHostID()) == 0) && strcmp(accName, availableAccList[j].getAccName()) == 0) {
			//해당하는 정보들을 가져와서 저장하는데 사용한다.
			reservationDate = availableAccList[j].getRegisterDate();
			accAddress = availableAccList[j].getAccAddress();
			accPrice = availableAccList[j].getAccPrice();
			reservationInfoArray[reservationNumber] = ReservationInfo(memberArray[memberNumber].getID(), reservationDate, hostID, accName, accAddress, accPrice);
			fprintf(out_fp, "%s %s %s %s %s\n", hostID, accName, accAddress, reservationDate, accPrice);
		}
		else {
			//조건 불만족
		}
	}
}

// 예약 정보 출력하기 함수
// 사용자가 요청하면 해당 사용자가 예약한 숙소 리스트를 출력한다.
// 해당 사용자의 ID와 예약정보에 저장된 ID 정보를 비교해 사용자를 특정한다.
// 해당사용자가 예약한 정보를 호스트 아이디, 숙소이름, 숙소주소, 숙소가격, 
// Created : 2018/5/31
// Author : 배철훈
void ReservationInfo::showReservationList() {
	char* hostID;
	char* accName;
	char* accAddress;
	char* reservationDate;
	char* accPrice;

	for (int i = 0; i < MAX_RESERVATION_NUM; i++) {
		if (strcmp(reservationInfoArray[i].getGuestID(), memberArray[memberNumber].getID()) == 0) {
			hostID = reservationInfoArray[i].getHostID();
			accName = reservationInfoArray[i].getAccName();
			accAddress = reservationInfoArray[i].getAccAddress();
			reservationDate = reservationInfoArray[i].getReservationDate();
			accPrice = reservationInfoArray[i].getAccPrice();

			fprintf(out_fp, "%s %s %s %s %s\n", hostID, accName, accAddress, reservationDate, accPrice);
		}
	}
}

// 1.2 회원탈퇴 시 예약한 정보가 있는지 확인하는 함수
// guest 회원일 시 예약 정보를 확인해서 예약이 존재하면 true, 없으면 false를 반환한다
// Created : 2018/5/30
// Author : 김형주
bool ReservationInfo::isAnyReservation() {
	for (int i = 0; i < MAX_RESERVATION_NUM; i++) {
		// 예약 정보의 guset 아이디와 현재 로그인된 사용자의 아이디를 비교
		if (strcmp(reservationInfoArray[i].getGuestID(), memberArray[memberNumber].getID()) == 0)
			return true;
		else
			return false;
	}
}

// 함수 : autoCreateReservation(char *guestID, char *reservationDate, char*hostID, char *accName, char *accAddress, char *accPrice)
// 설명 : opaque inventory예약할 때 게스트가 적은 조건과 만족하는 숙소가 있다면 예약을 성립하고 예약객체를 생성하는 함수다.
// 매개변수 :char *guestID - 게스트 id , char*accAddress -게스트가 쓴 주소, char* reservationDate - 게스트 희망예약날짜,
// char* accPrice -게스트의 희망가격, char * accName - 숙소이름
// 작성일 : 2018/5/31
// Author : 김지현
void ReservationInfo::autoCreateReservation(char *guestID, char *reservationDate, char*hostID, char *accName, char *accAddress, char *accPrice) {

	setGuestID(guestID);
	setReservationDate(reservationDate);
	setHostID(hostID);
	setAccName(accName);
	setAccAddress(accAddress);
	setAccPrice(accPrice);

}



// 클래스 선언
// class: 타이머 클래스
// 시간관련된 기능을 처리해준다.(1년지난 숙소삭제, 24시간 opaque inventory 예약 제한)
// Created : 2018/5/29
// Author : 김지현

class Timer {

private:
	int year;
	int month;
	int day;
	int hour;
	int min;

public:
	Timer();
	void setTimer();

	bool isPossibleOIReserve();
	void deleteAccOverYear();
};

Timer::Timer() {
}

// 함수 : setTimer()
// 설명 : 5.1에서 현재 시간을 입력하면 현재 시간이 바뀌고 timer의 각 year,month,day,hour,min을 변경해준다.
// 작성일 : 2018/5/28
// Author : 김지현
void Timer::setTimer() {
	char time[MAX_STRING];
	fscanf(in_fp, "%s", time);
	fprintf(out_fp, "5.1 현재시간 설정\n");
	fprintf(out_fp, "%s\n", time);
	strcpy(currentTime, time);
	year = atoi(strtok(time, ":"));
	month = atoi(strtok(NULL, ":"));
	day = atoi(strtok(NULL, ":"));
	hour = atoi(strtok(NULL, ":"));
	min = atoi(strtok(NULL, ":\t\n"));
}

// 함수 : setTimer()
// 설명 : opaque inventory예약을 시도할 때 해당 로그인된 게스트가 자신의 opaque inventory 시도시간과 현재 시간을
// 비교하여 자신이 24시간 안에 또 시도한 적이 있는 지 확인 한다. 시도했다면 flase, 시도한 적 없으면 true
// 작성일 : 2018/5/30
// Author : 김지현
bool Timer::isPossibleOIReserve() {
	char oiDate[MAX_STRING];

	int oiYear, oiMonth, oiDay, oiHour, oiMin;
	bool result = true;
	strcpy(oiDate, memberArray[memberNumber].getOIDate());

	if (strcmp(oiDate, "") == 0) { return result; }
	oiYear = atoi(strtok(oiDate, ":"));
	oiMonth = atoi(strtok(NULL, ":"));
	oiDay = atoi(strtok(NULL, ":"));
	oiHour = atoi(strtok(NULL, ":"));
	oiMin = atoi(strtok(NULL, ":\t\n"));
	if (oiYear == year) {
		if (oiMonth == month) {
			if (oiDay == day) {
				result = false;
			}
			else if (oiDay == day - 1) {
				if (oiHour > hour) {
					result = false;
				}
				else if (oiHour == hour) {
					if (oiMin > min) {
						result = false;
					}

				}
			}
		}
	}
	//시도시간은 무조건 현재시간보다 과거다.
	return result;
}

// 함수 : deleteAccOverYear()
// 설명 : 현재 시간을 설정해줄 때 같이 등록된 숙소 전부를 검사해서 숙소의 등록시간 현재 시간과
// 비교하였을 때 1년이 넘었다면 해당 숙소는 삭제한다.
// 작성일 : 2018/5/30
// Author : 김지현
void Timer::deleteAccOverYear() {
	char registerDate[MAX_STRING];
	int registerYear, registerMonth, registerDay, registerHour, registerMin;
	for (int i = 0; i < MAX_ACC_NUM; i++) {
		if (strcmp(accArray[i].getAccName(), "") == 0) {

			break;
		}
		strcpy(registerDate, accArray[i].getRegisterDate());

		registerYear = atoi(strtok(registerDate, ":"));
		registerMonth = atoi(strtok(NULL, ":"));
		registerDay = atoi(strtok(NULL, ":"));
		registerHour = atoi(strtok(NULL, ":"));
		registerMin = atoi(strtok(NULL, ":\t\n"));
		if (registerYear == year - 1) {//1년전
			if (registerMonth == month) {
				if (registerDay < day) {
					accArray[i].deleteAcc();
				}
				else if (registerDay == day)
				{
					if (registerHour == hour) {
						if (registerMin < min) {
							accArray[i].deleteAcc();
						}
					}
					else if (registerHour < hour) {
						accArray[i].deleteAcc();
					}
				}
			}
			else if (registerMonth < month) {
				accArray[i].deleteAcc();
			}
		}
		else if (registerYear < year - 1) {//1년보다 더 전에
			accArray[i].deleteAcc();
		}
	}
}

Timer timer;

// 1.1 회원가입 control class
// Created : 2018/5/29
// Author : 김형주
class SignUp {
public:
	void inputSignUpInfo(char[], char[], char[], char[], char[], char[]);
};

// 1.1 회원가입 정보 입력 함수
// SignUpUI에서 입력받은 정보를 SignUp 클래스로 전달받는다.
// Created : 2018/5/29
// Author : 김형주
void SignUp::inputSignUpInfo(char userType[], char name[], char ssn[], char address[], char id[], char password[]) {
	Member mem;
	bool isValid = mem.isValidSignUp(id, ssn);
	if (isValid == false) {
		fprintf(out_fp, "1.1 회원가입\n");
		fprintf(out_fp, "회원가입 실패, ID 중복이거나 주민번호가 6자리가 아닙니다\n");
		return;
	}
	mem.createMember(userType, name, ssn, address, id, password);
	// output.txt에 출력
	fprintf(out_fp, "1.1 회원가입\n");
	fprintf(out_fp, "%s %s %s %s %s %s\n", userType, name, ssn, address, id, password);
	return;
}

// 1.1 회원가입 UI Boundary class
// Created : 2018/5/29
// Author : 김형주
class SignUpUI {
public:
	void inputSignUpInfo();				// 회원 가입 UI에서 정보를 입력받는 함수
};

// 1.1 회원가입 정보 입력 함수
// 정보를 입력받아 signUp 함수에 입력받은 회원가입 정보를 넘긴다
// Created : 2018/5/22
// Author : 김형주
void SignUpUI::inputSignUpInfo() {
	// input.txt에서 입력
	char userType[MAX_STRING], name[MAX_STRING], ssn[MAX_STRING], address[MAX_STRING],
		id[MAX_STRING], password[MAX_STRING];
	fscanf(in_fp, "%s %s %s %s %s %s", userType, name, ssn, address, id, password);
	SignUp su;
	su.inputSignUpInfo(userType, name, ssn, address, id, password);
	return;
}

SignUpUI signUpUI;

// 1.2 회원탈퇴 control class
// Created : 2018/5/30
// Author : 김형주
class Withdraw {
public:
	void doWithdraw();
};

// 1.2 회원탈퇴 메뉴 함수
// 회원탈퇴를 신청하면 가입되었던 정보들을 포함하는 해당 회원(Member) 클래스를 삭제한다
// 회원 유형을 검사해 host, guest일 때 다르게 동작한다
// isNotValidWithdraw 변수가 true이면 회원탈퇴가 불가능하고 false이면 가능하다
// Created : 2018/5/20
// Author : 김형주
// 1. When & Who : 2018/05/30 김형주
// 함수 완성
void Withdraw::doWithdraw() {
	char* userType = memberArray[memberNumber].getUserType();
	bool isNotValidWithdraw;

	if (memberNumber == -1) {
		fprintf(out_fp, "1.2 회원탈퇴 실패, 로그인 중이 아닙니다.\n");
		return;
	}

	if (strcmp("host", userType) == 0) {
		Accomodation acc;
		isNotValidWithdraw = acc.isAnyAccomodation();
	}
	else if (strcmp("guest", userType) == 0) {
		ReservationInfo res;
		isNotValidWithdraw = res.isAnyReservation();
	}
	if (!isNotValidWithdraw) {
		fprintf(out_fp, "1.2 회원탈퇴\n%s\n", memberArray[memberNumber].getID());
		memberArray[memberNumber].deleteMember();
	}
	else {
		fprintf(out_fp, "1.2 회원탈퇴 실패, 등록숙소나 예약정보가 남아있습니다.\n");
	}
	return;
}

// 1.2 회원탈퇴 boundary class
// Created : 2018/5/30
// Author : 김형주
class WithdrawUI {
public:
	void doWithdraw();
};

// 1.2 회원탈퇴 UI 함수
// Created : 2018/5/20
// Author : 김형주
// Revsions :
// 1. When & Who : 2018/05/30 김형주
// 함수 완성
void WithdrawUI::doWithdraw() {
	Withdraw withdraw;
	withdraw.doWithdraw();
	return;
}
WithdrawUI withdrawUI;

// 2.1 로그인 control class
// Created : 2018/5/29
// Author : 김형주
class Login {
public:
	void inputLoginInfo(char*, char*);
};

// 2.1 로그인 메뉴 함수
// 함수 : void login(char*,char*)
// 로그인시 입력받은 정보를 받고 멤버 클래스에 저장되어 있는 id,passwd와 일치하는지 보려고
// 로그인 검사를 위해 isValidLogin 함수에 id, passwd를 넘긴다.
// 매개변수 : char * id - 아이디, char * password - 비밀번호
// Created : 2018/5/26
// Author : 김지현
// Revsions :
// 1. When & Who : 2018/05/26 22:25 pm 김지현
// 내용 : 로그인이 이미 되어있을때 또 로그인 되지 않도록 추가
// 수정 된 것 if (memberNumber != -1인 IF문 추가
void Login::inputLoginInfo(char* id, char* password) {
	if (memberNumber != -1) {
		fprintf(out_fp, "2.1 로그인\n");
		fprintf(out_fp, "로그인 실패, 이미 로그인 되어 있습니다\n");
		return;
	}
	Member mem;
	bool isValid = mem.isValidLogin(id, password);
	if (isValid == false) {
		fprintf(out_fp, "2.1 로그인\n");
		fprintf(out_fp, "로그인 실패, ID가 존재하지 않거나 ID와 비밀번호가 일치하지 않습니다\n");
		return;
	}

	// output.txt에 출력
	fprintf(out_fp, "2.1 로그인\n");
	fprintf(out_fp, "%s %s\n", id, password);
	return;
}

// 2.1 로그인 boundary class
// Created : 2018/5/29
// Author : 김형주
class LoginUI {
public:
	void inputLoginInfo();
};

// 2.1 로그인 정보 받는 함수
// 함수 : void inputLoginInfo()
// 정보를 입력받아 loginUp 함수에 로그인 정보를 넘긴다 
// Created : 2018/5/26
// Author : 김지현
void LoginUI::inputLoginInfo() {
	char id[MAX_STRING], password[MAX_STRING];
	fscanf(in_fp, "%s %s", id, password);

	Login l;
	l.inputLoginInfo(id, password);
	return;
}
LoginUI loginUI;

// 2.2 로그아웃 control class
//Created : 2018/5/29
//Author : 김형주
class Logout {
public:
	void doLogout();
};

// 2.2 로그아웃 실행 함수
// 함수 : void doLogout()
// 로그아웃면 게스트세션으로 돌아가기위해 다시 memberNumber를 -1로 한다.
// Created : 2018/5/29
// Author : 김형주
// Revsions :
// 1. When & Who : 2018/05/30 김형주
// 내용 : 게스트 세션이면 로그아웃 실패 메시지 추가
// 수정 된 것 if (memberNumber == -1)
//		fprintf(out_fp, "2.2 로그아웃\n로그아웃 실패, 이미 게스트 세션입니다.\n");
void Logout::doLogout() {
	if (memberNumber == -1) {
		fprintf(out_fp, "2.2 로그아웃\n로그아웃 실패, 이미 게스트 세션입니다.\n");
		return;
	}
	fprintf(out_fp, "2.2 로그아웃\n");
	fprintf(out_fp, "%s \n", memberArray[memberNumber].getID());
	memberNumber = -1;
	return;
}

// 2.2 로그아웃 boundary class
// Created : 2018/5/29
// Author : 김형주
class LogoutUI {
public:
	void doLogout();
};

// 2.2 로그아웃 UI 함수
// 함수 : void doLogout()
// 로그아웃하는 logout함수 실행
// Created : 2018/5/29
// Author : 김형주
void LogoutUI::doLogout() {
	Logout l;
	l.doLogout();
	return;
}
LogoutUI logoutUI;


// 클래스 : RegisterAcc
// 3.1 숙소등록 컨트롤 
// UI클래스로 부터 정보를 받아 숙소클래스의 함수에 넣는다.
// Created : 2018/5/26
// Author : 이대근
// Revsions :
// 1. When & Who : 2018/05/27 이대근
// 내용 : 함수를 클래스로 변경
class RegisterAcc {
private:

public:
	void inputInfo(char*, char*, char*, char*, char*);
};
// 함수 : inputInfo(char*accName, char* accAddress, char* accPrice, char* reservationPossibleDate, char* oiPrice)
// 설명 : 숙소 등록시 등록할 숙소 정보
// 숙소 이름, 숙소 주소, 예약 가능 날짜, 숙소 가격,OI가격을
// UI로 부터 입력받아 숙소 배열에 저장한다.
// 배열의 크기만큼 확인하면서 비어있는 배열에 찾아 정보를 저장한다.
// Created : 2018/5/27 
// Author : 이대근
// Revsions :
// 1. When & Who : 2018/05/30 이대근
// 내용 : 유저 타입이 호스트일 경우만 가능하도록 접근 제어 설정
// 1년 후 자동삭제 기능을 수행하기 기능을 추가 그때 필요한
// 숙소를 등록한 멤버(호스트)의 아이디와 현재 시각을 설정하기위해
// 입력값을 추가함.
void RegisterAcc::inputInfo(char*accName, char* accAddress, char* accPrice, char* reservationPossibleDate, char* oiPrice) {



	for (int i = 0; i < MAX_ACC_NUM; i++) {
		if (strcmp(accArray[i].getAccName(), "") == 0) {
			accArray[i].registerHostAcc(accName, accAddress, accPrice, reservationPossibleDate, oiPrice, memberArray[memberNumber].getID(), currentTime);
			break;
		}
	}

}

// 클래스 : RegisterAccUI
// 3.1 숙소등록 UI
// 숙소정보를 넣는 inputAccInfo함수 실행
// Created : 2018/5/26
// Author : 이대근
//input.txt에서 입력을 받아 숙소등록 컨트롤로 정보를 넘겨줌
// Revsions :
// 1. When & Who : 2018/05/27 이대근
// 내용 : 함수를 클래스로 변경,유저 타입이 호스트일 경우만 가능하도록 접근 제어 설정
class RegisterAccUI {//UI
private:
	RegisterAcc registerAcc;
public:
	void inputInfo();
};
// 함수 : inputInfo()
// 설명 : 숙소 등록시 등록할 숙소 정보
// 숙소 이름, 숙소 주소, 예약 가능 날짜, 숙소 가격,OI가격을
// input.txt에서 입력받아 컨트롤 클래스로 보낸다
//
// Created : 2018/5/27 
// Author : 이대근
// Revsions :
// 1. When & Who : 2018/05/28 이대근
// 내용 : 유저 타입이 호스트일 경우만 가능하도록 접근 제어 설정
void RegisterAccUI::inputInfo() {
	char accName[MAX_STRING], accAddress[MAX_STRING], reservationPossibleDate[MAX_STRING];
	char accPrice[MAX_STRING], oiPrice[MAX_STRING];


	fprintf(out_fp, "3.1 숙소등록\n");
	if (strcmp(memberArray[memberNumber].getUserType(), HOST) == 0) {
		fscanf(in_fp, "%s %s %s %s %s", accName, accAddress, accPrice, reservationPossibleDate, oiPrice);
		registerAcc.inputInfo(accName, accAddress, accPrice, reservationPossibleDate, oiPrice);
	}
	else {
		fprintf(out_fp, "권한이 없습니다.\n");
	}
}

RegisterAccUI registerAccUI;

// 클래스 : ShowHostAccInfo
// 3.2 숙소 조회 컨트롤
// Created : 2018/5/26
// Author : 이대근
// 숙소클래스에 호스트가 등록한 숙소정보를
//숙소클래스에서 가져옴.
class ShowHostAccInfo {//Control
private:
public:
	void showHostAcc();

};
// 함수 : showHostAcc()
// 설명 : 숙소 조회시 조회할 숙소 정보
// 숙소 클래스에서 숙소 정보(숙소 이름, 숙소 주소, 예약 날짜, 숙소 가격, 예약 유무, OI가격)를
// 저장해둔 임시 배열을 사용해 정렬한다.
// 정렬 방식은 버블 정렬이다. strcmp로 날짜를 비교해 가장 빠른 예약가능 날짜를 먼저 출력한다.
// Created : 2018/5/27 
// Author : 이대근
// Revsions :
// 1. When & Who : 2018/05/28 이대근
// 내용 : 예약 가능 날짜가 빠른 순으로 정렬 후 출력 구현
void ShowHostAccInfo::showHostAcc() {//Control

	char *tmp[100];//정렬을 위한 임시 배열
	count = 0;//전역변수 카운트 초기화

			  //숙소 배열을 모두 찾아보되 비어있지 않은
			  //배열의 정보를 가져온다.
	for (int i = 0; i < MAX_ACC_NUM; i++) {

		if (strcmp(accArray[i].getAccName(), "") != 0) {

			accArray[i].getHostAcc();

		}
	}
	//호스트가 등록한 숙소개수가 전체 배열의 크기이다.
	//출력할 정보는 숙소 이름, 숙소 주소, 예약 날짜, 숙소 가격, 예약 유무, OI가격으로
	//순서대로 0,1,2,3,4,5 의 옵셋을 가진다. 이 옵셋을 기준으로 정렬을 한다
	for (int j = 0; j < memberArray[memberNumber].getRegisterAccNum() - 1; j++) {
		for (int i = 0; i < memberArray[memberNumber].getRegisterAccNum() - 1 - j; i++) {
			if (strcmp(tempAcc[6 * i + 2], tempAcc[6 * (i + 1) + 2]) > 0) {
				//숙소 이름 교환
				tmp[6 * i] = tempAcc[6 * i];
				tempAcc[6 * i] = tempAcc[6 * (i + 1)];
				tempAcc[6 * (i + 1)] = tmp[6 * i];
				//숙소 주소 교환
				tmp[6 * i + 1] = tempAcc[6 * i + 1];
				tempAcc[6 * i + 1] = tempAcc[6 * (i + 1) + 1];
				tempAcc[6 * (i + 1) + 1] = tmp[6 * i + 1];
				//예약 날짜 교환
				tmp[6 * i + 2] = tempAcc[6 * i + 2];
				tempAcc[6 * i + 2] = tempAcc[6 * (i + 1) + 2];
				tempAcc[6 * (i + 1) + 2] = tmp[6 * i + 2];
				//숙소 가격 교환
				tmp[6 * i + 3] = tempAcc[6 * i + 3];
				tempAcc[6 * i + 3] = tempAcc[6 * (i + 1) + 3];
				tempAcc[6 * (i + 1) + 3] = tmp[6 * i + 3];
				//예약 유무 교환
				tmp[6 * i + 4] = tempAcc[6 * i + 4];
				tempAcc[6 * i + 4] = tempAcc[6 * (i + 1) + 4];
				tempAcc[6 * (i + 1) + 4] = tmp[6 * i + 4];
				//OI 가격 교환
				tmp[6 * i + 5] = tempAcc[6 * i + 5];
				tempAcc[6 * i + 5] = tempAcc[6 * (i + 1) + 5];
				tempAcc[6 * (i + 1) + 5] = tmp[6 * i + 5];
			}
		}
	}
	//정렬되었음으로 숙소 이름, 숙소 주소, 등록 날짜, 숙소 가격, 예약 유무, OI가격순으로 출력한다.
	if (strcmp(tempAcc[0], "") != 0) {
		for (int i = 0; i < memberArray[memberNumber].getRegisterAccNum(); i++) {
			for (int j = 0; j < 6; j++) {
				fprintf(out_fp, "%s ", tempAcc[6 * i + j]);
			}
			fprintf(out_fp, "\n");
		}
	}
}

// 클래스 : ShowHostAccInfoUI
// 3.2 숙소 조회 UI
// Created : 2018/5/26
// Author : 이대근
// 숙소클래스에 호스트가 등록한 숙소정보를 요청함.
// Revsions :
// 1. When & Who : 2018/05/27 이대근
// 내용 : 함수를 클래스로 변경
class ShowHostAccInfoUI {//UI
private:
	ShowHostAccInfo showHostAccInfo;
public:
	void showHostAcc();
};

// 함수 : showHostAcc()
// 설명 : 숙소 조회시 조회할 숙소 정보
// 컨트롤 클래스에 요청한다.
// Created : 2018/5/27 
// Author : 이대근
// Revsions :
// 1. When & Who : 2018/05/28 이대근
// 내용 : 유저 타입이 호스트일 경우만 가능하도록 접근 제어 설정
void ShowHostAccInfoUI::showHostAcc() {

	fprintf(out_fp, "3.2 숙소조회\n");

	if (strcmp(memberArray[memberNumber].getUserType(), HOST) == 0)
		showHostAccInfo.showHostAcc();
	else
		fprintf(out_fp, "권한이 없습니다.\n");
}

ShowHostAccInfoUI showHostAccInfoUI;

// 클래스 : SearchAcc
// 4.1 예약 가능한 숙소 검색하기 컨트롤
// Created : 2018/05/27
// Author : 배철훈
// Revsions :
// 1. When & Who : 2018/05/27 이대근
// 내용 : 함수를 클래스로 변경
class SearchAcc {
public:
	void searchAcc(char*, char*);
};

// 함수 : searchAcc(char* accAddress, char* reservationDate)
// 설명 : 예약 가능한 숙소 검색시 섬색할 숙소 정보
// 숙소 주소, 예약 가능 날짜를
// UI 클래스에서 받아 숙소 엔티티에서 
// 정보를 가져온다.
// Created : 2018/5/27 
// Author : 이대근
void SearchAcc::searchAcc(char* accAddress, char* reservationDate) {

	for (int i = 0; i<MAX_ACC_NUM; i++) {
		if (strcmp(accArray[i].getAccName(), "") != 0)
			accArray[i].searchGuestAcc(accAddress, reservationDate);
	}
}

// 클래스 : SearchAccUI
// 4.1 예약 가능한 숙소 검색하기 UI
// Created : 2018/05/27
// Author : 배철훈
// Revsions :
// 1. When & Who : 2018/05/27 이대근
// 내용 : 함수를 클래스로 변경
class SearchAccUI {
private:
	SearchAcc searchAcc;
public:
	void inputReservationInfo();
};

// 함수 : inputReservationInfo()
// 설명 : 예약 가능한 숙소 검색시 섬색할 숙소 정보
// 숙소 주소, 예약 가능 날짜를
// input.txt에서 입력받아 컨트롤 클래스로 보낸다
//
// Created : 2018/5/27 
// Author : 이대근
void SearchAccUI::inputReservationInfo() {
	char accAddress[MAX_STRING], reservationDate[MAX_STRING];
	fprintf(out_fp, "4.1 숙소검색\n");
	fscanf(in_fp, "%s %s", accAddress, reservationDate);
	searchAcc.searchAcc(accAddress, reservationDate);
}

SearchAccUI searchAccUI;

// 클래스 선언
// class : MakeReservation (숙소예약시 필요한 control object를 class로 선언)
// Created : 2018/05/30
// Author : 배철훈
class MakeReservation {
private:
	ReservationInfo mrR;
	Accomodation mrA;
public:
	MakeReservation();
	void selectReservation(char*, char*);	// 전달받은 값으로 예약을 진행
};
MakeReservation::MakeReservation() {

}

// 함수 : selectReservation
// 선택한 해당 숙소의 호스트 아이디와 숙소이름을 받아들여 예약정보에 예약을 추가한다
// 예약을 완료한 후 숙소의 reservation 항목을 O 로 수정하라고 요청한다.
// Created : 2018/05/31
// Author : 배철훈
void MakeReservation::selectReservation(char* hostID, char* accName) {
	fprintf(out_fp, "4.2 숙소예약\n");
	mrR.makeReservation(hostID, accName);	//reservationInfo 엔티티 클래스의 함수를 사용
											//mrA.set_reservation('O');
											//예약한 숙소정보에 reservation 값을 'O'로 수정해준다.
	for (int i = 0; i < MAX_ACC_NUM; i++) {
		if ((strcmp(accArray[i].getAccName(), accName) == 0) && (strcmp(accArray[i].getHostID(), hostID) == 0)) {
			accArray[i].setReservation('O');
		}
	}
}

// 클래스 선언
// class : MakeReservationUI (숙소예약시 필요한 boundary object를 class로 선언)
// Created : 2018/05/30
// Author : 배철훈
class MakeReservationUI {
private:
	MakeReservation mrui;
public:
	MakeReservationUI();
	void selectReservationInfo();	// 사용자가 선택한 값을 읽는다.
};

MakeReservationUI::MakeReservationUI() {

}

// 함수 : selectReservationInfo
// 사용자가 예약가능한 숙소리스트에서 예약을 하고자 하는 숙소를 선택한 경우
// 요청한 값을 받아들인다.
// Created : 2018/05/31
// Author : 배철훈
void MakeReservationUI::selectReservationInfo() {
	char hostID[MAX_STRING], accName[MAX_STRING];
	fscanf(in_fp, "%s %s", hostID, accName);
	mrui.selectReservation(hostID, accName);	//makeReservation control 클래스의 함수를 사용
}

MakeReservationUI makeReservationUI;


// 클래스 선언

// class: ReserveOI(opaque inventory 예약 컨트롤 클래스)
// opaque inventory 예약 컨트롤 클래스로 oi예약을 관리
// Created : 2018/5/29
// Author : 김지현
class ReserveOI {
public:
	void inputInfo(char*, char*, char*);
};
// 함수 : inputInfo(char*accAddress, char* reservationDate, char* cost)
// 설명 : UI에서 값을 받아와 Opaque inventory 시도시간에서 24시간 후인가 확인하고 게스트가 쓴
// 조건들과 숙소가 일치하면 해당 숙소에 관련된 예약정보를 만든다.
// 매개변수 : char*accAddress - 숙소주소, char* reservationDate - 이용가능날짜, char* cost - 게스트희망가격
// Created : 2018/5/28 
// Author : 김지현
void ReserveOI::inputInfo(char*accAddress, char* reservationDate, char* cost) {
	int i;
	int accNumber;

	if (!timer.isPossibleOIReserve()) {
		fprintf(out_fp, "Try again in 24 hours\n");
		return;
	}
	for (i = 0; i < MAX_ACC_NUM; i++) {
		if (accArray[i].checkOICondition(accAddress, reservationDate, cost)) {
			accNumber = i;
			for (i = 0; i < MAX_RESERVATION_NUM; i++) {
				if (strcmp(reservationInfoArray[i].getAccName(), "") == 0) {
					reservationInfoArray[i].autoCreateReservation(memberArray[memberNumber].getID(), reservationDate, accArray[accNumber].getHostID(), accArray[accNumber].getAccName(), accArray[accNumber].getAccAddress(), cost);
					accArray[accNumber].setReservation('O');
					memberArray[memberNumber].setOIDate(currentTime);
					fprintf(out_fp, "Success %s %s %s %c %s\n", accArray[accNumber].getHostID(), accArray[accNumber].getAccName(), accArray[accNumber].getAccAddress(), accArray[accNumber].getReservation(), cost);
					break;
				}
			}
			break;
		}
	}
}



// 클래스 선언

// class: OIReserveUI(opaque inventory 예약 바운더리 클래스)
// opaque inventory 예약 바운더리 클래스로 oi예약 정보를 받는 ui역할
// Created : 2018/5/29
// Author : 김지현
class OIReserveUI {
private:
	ReserveOI reserveOI;

public:

	void inputInfo();
};

// 함수 : inputInfo()
// 설명 : 값을 입력받아 opaque inventory 예약하기 위해 reserveOI에게 입력받은 값을 넘겨준다.
// Created : 2018/5/28 
// Author : 김지현
void OIReserveUI::inputInfo() {
	fprintf(out_fp, "4.3 Opaque inventory 예약\n");
	char accAddress[MAX_STRING], reservationDate[MAX_STRING], cost[MAX_STRING];

	fscanf(in_fp, "%s %s %s", accAddress, reservationDate, cost);

	reserveOI.inputInfo(accAddress, reservationDate, cost);
}
OIReserveUI reserveOIUI;


// 클래스 선언
// class : ShowReservation (예약한 숙소 검색시 필요한 control object를 class로 선언)
// created : 2018/05/30
// Author : 배철훈
class ShowReservation {
private:
	ReservationInfo srR;
public:
	ShowReservation();
	void showReservationInfo();
};
ShowReservation::ShowReservation() {

}

// 함수 : showReservationInfo
// 예약정보를 출력하기 위해 ReservationInfo 클래스의 showReservationList 함수를 호출한다.
// Created : 2018/05/31
// Author : 배철훈
void ShowReservation::showReservationInfo() {
	fprintf(out_fp, "4.4 예약정보 조회\n");
	srR.showReservationList();
}

// 클래스 선언
// class : ShowReservationInfoUI (예약한 숙소 검색시 필요한 boundary object를 class로 선언)
// created : 2018/05/30
// Author : 배철훈
class ShowReservationInfoUI {
private:
	ShowReservation sriui;
public:
	ShowReservationInfoUI();
	void requestReservationInfo();
};
ShowReservationInfoUI::ShowReservationInfoUI() {

}

// 함수 : showReservationInfo
// 사용자가 예약한 숙소 조회를 요청할경우 해당 함수 사용.
// showRevervation control 의 showReservationInfo를 요청한다.
// Created : 2018/05/31
// Author : 배철훈
void ShowReservationInfoUI::requestReservationInfo() {
	//ShowReservation (Control class)의 showReservationInfo()를 호출한다.
	sriui.showReservationInfo();
}

ShowReservationInfoUI showReservationInfoUI;




// main 함수
// input.txt와 output.txt를 열고 메뉴 구분 함수인 doTask()를 실행한다.
int main() {
	in_fp = fopen(INPUT_FILE_NAME, "r+");
	out_fp = fopen(OUTPUT_FILE_NAME, "w+");

	doTask();

	return 0;
}




// doTask 함수
// 1 1, 5 1 등으로 표시된 input.txt 값을 읽어 해당 메뉴를 제공하는 함수로 switch한다.
void doTask() {
	int menu_level_1 = 0, menu_level_2 = 0;
	int is_program_exit = 0;

	while (!is_program_exit)
	{
		// 입력 파일에서 메뉴 숫자 2개를 읽기
		fscanf(in_fp, "%d %d ", &menu_level_1, &menu_level_2);

		// 메뉴 구분 및 해당 연산 수행
		switch (menu_level_1)
		{
		case 1:
			switch (menu_level_2)
			{
			case 1: // 1.1 회원가입 메뉴

				signUpUI.inputSignUpInfo();
				break;
			case 2: // 1.2 회원탈퇴 메뉴

				withdrawUI.doWithdraw();
				break;
			}
			break;

		case 2:
			switch (menu_level_2)
			{
			case 1: // 2.1 로그인 메뉴

				loginUI.inputLoginInfo();
				break;
			case 2: // 2.2 로그아웃 메뉴

				logoutUI.doLogout();
				break;
			}
			break;

		case 3:
			switch (menu_level_2)
			{
			case 1: // 3.1 숙소등록 메뉴
				registerAccUI.inputInfo();
				break;
			case 2: // 3.2 등록 숙소 조회 메뉴
				showHostAccInfoUI.showHostAcc();
				break;
			}
			break;

		case 4:
			switch (menu_level_2)
			{
			case 1: // 4.1 숙소 검색 메뉴
				searchAccUI.inputReservationInfo();
				break;
			case 2: // 4.2 숙소 예약 메뉴
				makeReservationUI.selectReservationInfo();
				break;
			case 3: // 4.3 Opaque Inventory 예약 메뉴 
				reserveOIUI.inputInfo();
				break;
			case 4: // 4.4 예약 정보 조회 메뉴
				showReservationInfoUI.requestReservationInfo();
				break;
			}
			break;

		case 5:
			switch (menu_level_2)
			{
			case 1: // 5.1 현재 시간 설정 메뉴
				timer.setTimer();
				timer.deleteAccOverYear();
				break;
			}
			break;

		case 6:
			switch (menu_level_2)
			{
			case 1: // 6.1 Session 변경 메뉴
				changeSession();
				break;
			case 2: // 6.2 Guest Session으로 변경 메뉴
				changeGuest();
				break;
			}
			break;

		case 7:
			switch (menu_level_2)
			{
			case 1: // 7.1 종료 메뉴
				program_exit();
				is_program_exit = 1;
				break;
			}
			break;
		}
	}
	return;
}


// 6.1 세션변경함수
// 함수 : void changeSession(char*id)
// 입력된 id에 해당하는 세션으로 바꾸기위해 해당 id를 찾아 해당하는 인덱스값 반환 함수
// Created : 2018/5/26
// Author : 김지현
void changeSession() {
	int i = 0;
	char id[MAX_STRING];
	fprintf(out_fp, "6.1 Session 변경\n");
	fscanf(in_fp, "%s", id);
	for (i = 0; i < memberCount; i++) {
		if (!strcmp(memberArray[i].getID(), id)) {
			memberNumber = i;
			fprintf(out_fp, "%s\n", id);
			return;
		}
	}
	if (i == memberCount) {

		fprintf(out_fp, "입력된 id에 해당하는 멤버가 없습니다.\n");
	}
	return;
}


// 6.2 guest session으로 변경
// 함수 : void changeGuest()
// guestsession으로 바꾸기 위해 memberCount를 -1로 바꾸는 함수
// Created : 2018/5/26
// Author : 김지현
void changeGuest() {
	memberNumber = -1;
	fprintf(out_fp, "6.2 guest session으로 변경\n");
	return;
}


// 7.1 프로그램 종료 메뉴 함수
// doTask 함수를 종료하고 output.txt에 종료 출력
// Created : 2018/5/20
// Author : 김형주
void program_exit() {
	fprintf(out_fp, "7.1 종료\n");
	return;
}