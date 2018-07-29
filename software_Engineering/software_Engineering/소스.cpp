
// ��� ����

#include <iostream>
#include <cstdio>
#include <cstring>


// ��� ����

#define INPUT_FILE_NAME "input.txt"
#define OUTPUT_FILE_NAME "output.txt"
#define MAX_STRING 32            // ���� �ִ� �Է� ����
#define MAX_MEMBER_NUM 512         // ȸ�� ������ ������ Member �迭�� �ִ� ũ��
#define MAX_ACC_NUM 512            //���� ������ ������ Accomodation �迭�� �ִ� ũ��
#define MAX_RESERVATION_NUM 512      // ���� ������ ������ ReservationInfo �迭�� �ִ� ũ��
#define HOST "host"         

FILE* in_fp; // input.txt ������
FILE* out_fp; // output.txt ������


			  // �Լ� ����

void doTask();
void changeSession();      //�Է��� id�� ���� ���� �Լ�
void changeGuest();        //�Խ�Ʈ �������� ���� �Լ�
void program_exit();      // ���α׷� ���� �Լ�


						  // ���� ���� ����

char currentTime[MAX_STRING]; //����ð�


							  // Ŭ���� ����

							  // ȸ�� Ŭ����
							  // ȸ�������� ȸ������ ������ �����Ѵ�
							  // Created : 2018/5/20
							  // Author : ������

							  // Revsions :
							  // 1. When & Who : 2018/05/26 21:31 pm ������
							  // ���� : char*������ MAX_STRING �迭�� �ٲ�,��� �����ڹ� set�Լ� �� ���� =���� �޾ƿ°� strcpy�ιٲ�
							  // ���� �� set�Լ���, ������, private attribute
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
	bool isValidSignUp(char*, char*);   // ȸ�� ���� ���� ���� �Լ� - ID �ߺ�, �ֹι�ȣ ���̸� üũ
	void createMember(char*, char*,
		char*, char*, char*, char*);	// ȸ�� ���� ���� �� ����� ������ Member �迭�� �����ϴ� �Լ�
	void deleteMember(); //��� ����
	bool isValidLogin(char*, char*);	// �α��� ��ȿ���� Ȯ�� �Լ� - ID , ��й�ȣ�� ���Ŭ������ �����ϴ��� Ȯ��
};
// �⺻ ������ - ��� ���� NULL�� �ʱ�ȭ
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
// ��� ��� ���� ������ getter, setter
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

Member memberArray[MAX_MEMBER_NUM] = {}; // ȸ�� ���� ������ ���� Member(ȸ��) �迭
int memberNumber = -1;   // ���� ȸ�� �� session ������ ���� ȸ����ȣ �ĺ���
						 // �ʱ� session�� -1, ����ڰ� ��� ���� ���� �ش� ������� ȸ�� ��ȣ�� ����
int memberCount = 0;//������ �� ȸ�� ��

char* tempAcc[MAX_STRING]; //���� �����Ҷ� ����� �迭


						   // 1.1 ȸ������ ��� �˻� �Լ�
						   // ȸ������ �� �ۼ��� ������ üũ�Ѵ� ID�� �ߺ��ǰų� �ֹι�ȣ ���̰� ���� ���� ��� false�� ��ȯ�Ѵ�
						   // �Է°� : ���̵�, �ֹι�ȣ
						   // ��°� : ����� �˻��ѵ� �̻��� ������ true, ������ false
						   // Created : 2018/5/22
						   // Author : ������

						   // Revsions :
						   // 1. When & Who : 2018/05/26 21:24 pm ������
						   // ���� : ȸ������ ������ �α��α��� �Ǵ� memberNumber �� �����Ű�°� ���� i������ ��ü
						   //ȸ������ ������ memberNumber�� i�� �ٲٰ� memberArray[memberNumber] = 
						   //Member(userType, name, ssn, address, id, password);�� �ߴµ� 
						   //�װͿ��� memberNumber�� �α������� �ѱ�� i������ �ٲ�
						   // ���� �� �� ���ǹ� memberCount �߰�, memberNumber->i, == -> strcmp �� �ٲ�
bool Member::isValidSignUp(char* id, char* ssn) {
	for (int i = 0; i < memberCount; i++) {
		if (strcmp(memberArray[i].getID(), id) == 0)
			return false;
		if (strlen(ssn) != 6)
			return false;
	}
	return true;
}

// 1.1 ȸ������ ������ ���� ����
// ȸ������ ���� �� ȸ�� ������ Member �迭�� ����
// �Է°� : ��������, �̸�, �ֹι�ȣ, �ּ�, ���̵�, ��й�ȣ �� 6������ ���ڿ�
// Created : 2018/5/22
// Author : ������

// Revsions :
// 1. When & Who : 2018/05/26 18:32 pm ������
// ���� : ȸ������ ������ �α��α��� �Ǵ� memberNumber �� �����Ű�°� ���� i������ ��ü
//ȸ������ ������ memberNumber�� i�� �ٲٰ� memberArray[memberNumber] = Member(userType, name, ssn, address, id, password);
//�� �ߴµ� �װͿ��� memberNumber�� �α������� �ѱ�� i������ �ٲ�, ȸ�������� �� ȸ�� ���� ���� ���� memberCount �߰�
//���� ���� ���� ����� ���̵�� ������� ��� ""�� ���ͼ� �װͰ� ������ ����� ���ٴ� ���̴� �ű�ٰ� �߰��ϴ�
//(strcmp(memberArray[i].getID(),"") == 0) �߰�
// ���� �� �� memberNumber -> i ,memberCount �߰�, ȸ������ ���̵� �񱳹� ==���� strcmp�� ����
void Member::createMember(char* userType, char* name, char* ssn, char* address, char* id, char* password) {
	// Member �迭���� ��� �ִ� ���� ã�� ����� ������ �����Ѵ�.
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

// 1.2 ȸ�� Ż�� �� ȸ�� ���� ���� �Լ�
// ȸ�� ������ ��� NULL������ �ٲٰ� guest session���� �����Ѵ�
// ȸ�� ���� ī��Ʈ�ϴ� memberCount�� ���� 1�� �ٿ��ش�
// Created : 2018/5/30
// Author : ������
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

// 2.1 �α��� ��ȿ���� �˻� �Լ�
// bool isValidLogin(char* id, char* ssn)
// �α��� �� �ۼ��� id�� ��й�ȣ�� ���� ID�� ��ϵǾ����� �ʰų� id�� ��й�ȣ�� ��ġ���� ���� ��� false�� ��ȯ�Ѵ�
// �Ű����� : char * id - ���̵�, char * password - ��й�ȣ
// ��ȯ�� : �α��� ��ȿ���� �˻��� �� �̻��� ������ true, ������ false
// Created : 2018/5/26
// Author : ������
// Revsions :
// 1. When & Who : 2018/05/27  �̴��
// ���� : �α����� �Ǵ� ��Ȳ���� ����Ÿ���� ȣ��Ʈ�ΰ��
// ����Ÿ�� üũ�� �ص� -> ���� ���ҵ�ϵ� ȣ��Ʈ���� ����
// Revsions :
// 1. When & Who : 2018/05/29 20:21 pm  ������
// ���� : ����Ÿ���� �ʿ�� ������� �ǹǷ� ����Ÿ��üũ �κ� ����
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


// Ŭ���� ����

// Ŭ���� : Accomodation
// ����� ������ ����
//(���� �̸�, ���� �ּ�, ���� ����, �̿� ���� ��¥, ��� ��¥, OI����, ��������,ȣ��ƮID)�� �����Ѵ�
// ���ҿ� ���õ� ������ ���� Ŭ�������� �����´�.
// Created : 2018/5/26
// Author : �̴��

// Revsions :
// 1. When & Who : 2018/05/29 21:47 pm ������
// ���� : ��Ͻð��� ���డ�ɽð��� �и�
// �����Ȱ� : reservationPossibleDate �迭 �߰�
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

// �⺻ ������
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
// ��� ��� ���� ������ getter, setter
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

// �Լ� : deleteAcc()
// ���� : ����ߴ� ���Ҹ� �����.

// �ۼ��� : 2018/5/29
// Author : ������

// Revsions :
// 1. When & Who : 2018/05/30  �̴��
// ���� : ���࿩�θ� NULL�� �ٲ�
// ������ �� : this->reservation ='X' =>   this->reservation = NULL;
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

// �Լ� : registerHostAcc(char* accName, char* accAddress, char* accPrice, char* reservationPossibleDate, char* oiPrice, char* hosdID, char*currentTime)
// ���� : ��Ͻ� ���� �̸�,���� �ּ�,���Ұ���,�̿볯¥,OI������ input.txt�� �Է¹޾�
// ���� Ŭ������ �����Ѵ�
// ��Ͻ� ���� ������ 'X'���ϰ� �α��� �Ǿ��ִ� ȣ��ƮID�� ����Ѵ�.
//
// �ۼ��� : 2018/5/27
// Author : �̴��
void Accomodation::registerHostAcc(char* accName, char* accAddress, char* accPrice, char* reservationPossibleDate, char* oiPrice, char* hosdID, char*currentTime) {

	setAccName(accName);
	setAccAddress(accAddress);
	setAccPrice(accPrice);
	setRegisterDate(currentTime);
	setOiPrice(oiPrice);
	setReservation('X');
	setHostID(memberArray[memberNumber].getID());
	setReservationPossibleDate(reservationPossibleDate);

	memberArray[memberNumber].setRegisterAccNum();//���� ���Ŀ� �ʿ��� ���Ұ����� ���� ����� ���� ���� ���

	fprintf(out_fp, "%s %s %s %s %s\n", accName, accAddress, accPrice, reservationPossibleDate, oiPrice);
}

int count = 0;			//�ӽ� �迭�� ���� �ε��� ī����
char** temp[MAX_STRING];//������ ���� �ӽ� �迭

						// �Լ� : getHostAcc()
						// ���� : ���� ��ȸ�� ��ϵǾ� �ִ� ���������� �����´�.
						// ������ ���� ������ ���� �ӽ� �迭�� �����Ѵ�.
						// ��ȸ�� ȣ��Ʈ�� �ڽ��� ����� ���Ҹ� ��ȸ �� �� �ֵ��� �Ѵ�. 
						// �ۼ��� : 2018/5/27
						// Author : �̴��

void Accomodation::getHostAcc() {

	if (strcmp(memberArray[memberNumber].getID(), getHostID()) == 0) {//������ ID�� ���ҿ� ��ϵ�ID�� ������
																	  //�� ������ ����� ���Ҹ� ��ȸ
		tempAcc[count++] = getAccName();
		tempAcc[count++] = getAccAddress();
		tempAcc[count++] = getReservationPossibleDate();
		tempAcc[count++] = getAccPrice();
		if (getReservation() == 'X')		//getReservation�� ���� ���� char�� ������ �ӽ� �迭�� ���� �� ��������
			tempAcc[count++] = "X";			//���ǹ��� ����� �׿� �´� ���� ���� ������ �־��ش�.
		else
			tempAcc[count++] = "O";

		tempAcc[count++] = getOiPrice();
	}
}

// �Լ� : searchGuestAcc(char*accAddress, char*reservationDate)
// ���� : �Խ�Ʈ�� ���� ������ ���Ҹ� �˻��Ѵ�. 
// �Է� ������ ���� �ּҿ� ���� ��¥�� �Է��Ѵ�
// ��� ������ ���డ���� ���� ���� (ȣ��ƮID �����̸� �����ּ� ����)�� ����Ѵ�.
//
// �ۼ��� : 2018/5/27
// Author : �̴��
void Accomodation::searchGuestAcc(char*accAddress, char*reservationDate) {

	char *Address;
	char *reservationPossibleDate;
	char *accPrice;
	char *hostID;
	char *accName;
	int j = 0;
	char *oiPrice;

	if (getReservation() == 'X') {//������ �Ǿ��������� ����, ���� ������ 'X'�� ���

		Address = getAccAddress();
		reservationPossibleDate = getReservationPossibleDate();

		if ((strcmp(Address, accAddress) == 0) && (strcmp(reservationPossibleDate, reservationDate) == 0)) {//�˻��� �����ּҿ� ���೯¥�� ���� ���� ã��

			accName = getAccName();
			hostID = getHostID();
			accPrice = getAccPrice();
			oiPrice = getOiPrice();

			fprintf(out_fp, "%s %s %s %s\n", hostID, accName, Address, accPrice);//ȣ��Ʈ �����̸� �����ּ� ���� 
																				 // ���� �˻� ����� ����Ʈ�� ����. ���� ���࿡ ���. (�����̸�, �����ּ�, ���Ұ���, ������ϴ³�¥, oi����)
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

// 1.2 ȸ��Ż�� �� ����� ���Ұ� �ִ��� Ȯ���ϴ� �Լ�
// host ȸ���� �� ��� ���Ҹ� Ȯ���ؼ� ���Ұ� �����ϸ� true, ������ false�� ��ȯ�Ѵ�
// Created : 2018/5/30
// Author : ������
bool Accomodation::isAnyAccomodation() {
	int count = memberArray[memberNumber].getRegisterAccNum();
	if (count == 0)
		return false;
	else
		return true;
}

// �Լ� : checkOICondition(char*accAddress, char* reservationDate, char* cost)
// ���� : �Խ�Ʈ�� Opaque inventory ������ �� ���� ��ü�� �Խ�Ʈ�� ���� ���ǿ� �������� �˻��Ͽ�
// true�� false���� ��ȯ�Ѵ�.
// �Ű����� :char*accAddress -�Խ�Ʈ�� �� �ּ�, char* reservationDate - �Խ�Ʈ ������೯¥, char* cost -�Խ�Ʈ�� �������
// �ۼ��� : 2018/5/31
// Author : ������
bool Accomodation::checkOICondition(char*accAddress, char* reservationDate, char* cost) {

	if (strcmp(getAccAddress(), accAddress) == 0 && strcmp(getReservationPossibleDate(), reservationDate) == 0 && strcmp(getOiPrice(), cost) < 0 && getReservation() == 'X'&&strcmp(getOiPrice(), "0") != 0) {
		return true;
	}
	else {
		return false;
	}
}

Accomodation accomodation;

// Ŭ���� ����
// �������� Ŭ����
// ����� ������ ������ �����Ѵ�
// Created : 2018/5/27
// Author : ��ö��
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

// ���� �����ϱ� �Լ�
// ���డ���� ���� �˻��ϱ� �������Ʈ���� ����ڰ� ������ ���ϴ� ���Ҹ� �����Ұ��
// �ش� ���ҿ� ���� ������ ������ �迭���� ���� ������ ReservationInfo �� �߰��Ѵ�.
// Created : 2018/5/31
// Author : ��ö��
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
			//�ش��ϴ� �������� �����ͼ� �����ϴµ� ����Ѵ�.
			reservationDate = availableAccList[j].getRegisterDate();
			accAddress = availableAccList[j].getAccAddress();
			accPrice = availableAccList[j].getAccPrice();
			reservationInfoArray[reservationNumber] = ReservationInfo(memberArray[memberNumber].getID(), reservationDate, hostID, accName, accAddress, accPrice);
			fprintf(out_fp, "%s %s %s %s %s\n", hostID, accName, accAddress, reservationDate, accPrice);
		}
		else {
			//���� �Ҹ���
		}
	}
}

// ���� ���� ����ϱ� �Լ�
// ����ڰ� ��û�ϸ� �ش� ����ڰ� ������ ���� ����Ʈ�� ����Ѵ�.
// �ش� ������� ID�� ���������� ����� ID ������ ���� ����ڸ� Ư���Ѵ�.
// �ش����ڰ� ������ ������ ȣ��Ʈ ���̵�, �����̸�, �����ּ�, ���Ұ���, 
// Created : 2018/5/31
// Author : ��ö��
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

// 1.2 ȸ��Ż�� �� ������ ������ �ִ��� Ȯ���ϴ� �Լ�
// guest ȸ���� �� ���� ������ Ȯ���ؼ� ������ �����ϸ� true, ������ false�� ��ȯ�Ѵ�
// Created : 2018/5/30
// Author : ������
bool ReservationInfo::isAnyReservation() {
	for (int i = 0; i < MAX_RESERVATION_NUM; i++) {
		// ���� ������ guset ���̵�� ���� �α��ε� ������� ���̵� ��
		if (strcmp(reservationInfoArray[i].getGuestID(), memberArray[memberNumber].getID()) == 0)
			return true;
		else
			return false;
	}
}

// �Լ� : autoCreateReservation(char *guestID, char *reservationDate, char*hostID, char *accName, char *accAddress, char *accPrice)
// ���� : opaque inventory������ �� �Խ�Ʈ�� ���� ���ǰ� �����ϴ� ���Ұ� �ִٸ� ������ �����ϰ� ���ఴü�� �����ϴ� �Լ���.
// �Ű����� :char *guestID - �Խ�Ʈ id , char*accAddress -�Խ�Ʈ�� �� �ּ�, char* reservationDate - �Խ�Ʈ ������೯¥,
// char* accPrice -�Խ�Ʈ�� �������, char * accName - �����̸�
// �ۼ��� : 2018/5/31
// Author : ������
void ReservationInfo::autoCreateReservation(char *guestID, char *reservationDate, char*hostID, char *accName, char *accAddress, char *accPrice) {

	setGuestID(guestID);
	setReservationDate(reservationDate);
	setHostID(hostID);
	setAccName(accName);
	setAccAddress(accAddress);
	setAccPrice(accPrice);

}



// Ŭ���� ����
// class: Ÿ�̸� Ŭ����
// �ð����õ� ����� ó�����ش�.(1������ ���һ���, 24�ð� opaque inventory ���� ����)
// Created : 2018/5/29
// Author : ������

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

// �Լ� : setTimer()
// ���� : 5.1���� ���� �ð��� �Է��ϸ� ���� �ð��� �ٲ�� timer�� �� year,month,day,hour,min�� �������ش�.
// �ۼ��� : 2018/5/28
// Author : ������
void Timer::setTimer() {
	char time[MAX_STRING];
	fscanf(in_fp, "%s", time);
	fprintf(out_fp, "5.1 ����ð� ����\n");
	fprintf(out_fp, "%s\n", time);
	strcpy(currentTime, time);
	year = atoi(strtok(time, ":"));
	month = atoi(strtok(NULL, ":"));
	day = atoi(strtok(NULL, ":"));
	hour = atoi(strtok(NULL, ":"));
	min = atoi(strtok(NULL, ":\t\n"));
}

// �Լ� : setTimer()
// ���� : opaque inventory������ �õ��� �� �ش� �α��ε� �Խ�Ʈ�� �ڽ��� opaque inventory �õ��ð��� ���� �ð���
// ���Ͽ� �ڽ��� 24�ð� �ȿ� �� �õ��� ���� �ִ� �� Ȯ�� �Ѵ�. �õ��ߴٸ� flase, �õ��� �� ������ true
// �ۼ��� : 2018/5/30
// Author : ������
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
	//�õ��ð��� ������ ����ð����� ���Ŵ�.
	return result;
}

// �Լ� : deleteAccOverYear()
// ���� : ���� �ð��� �������� �� ���� ��ϵ� ���� ���θ� �˻��ؼ� ������ ��Ͻð� ���� �ð���
// ���Ͽ��� �� 1���� �Ѿ��ٸ� �ش� ���Ҵ� �����Ѵ�.
// �ۼ��� : 2018/5/30
// Author : ������
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
		if (registerYear == year - 1) {//1����
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
		else if (registerYear < year - 1) {//1�⺸�� �� ����
			accArray[i].deleteAcc();
		}
	}
}

Timer timer;

// 1.1 ȸ������ control class
// Created : 2018/5/29
// Author : ������
class SignUp {
public:
	void inputSignUpInfo(char[], char[], char[], char[], char[], char[]);
};

// 1.1 ȸ������ ���� �Է� �Լ�
// SignUpUI���� �Է¹��� ������ SignUp Ŭ������ ���޹޴´�.
// Created : 2018/5/29
// Author : ������
void SignUp::inputSignUpInfo(char userType[], char name[], char ssn[], char address[], char id[], char password[]) {
	Member mem;
	bool isValid = mem.isValidSignUp(id, ssn);
	if (isValid == false) {
		fprintf(out_fp, "1.1 ȸ������\n");
		fprintf(out_fp, "ȸ������ ����, ID �ߺ��̰ų� �ֹι�ȣ�� 6�ڸ��� �ƴմϴ�\n");
		return;
	}
	mem.createMember(userType, name, ssn, address, id, password);
	// output.txt�� ���
	fprintf(out_fp, "1.1 ȸ������\n");
	fprintf(out_fp, "%s %s %s %s %s %s\n", userType, name, ssn, address, id, password);
	return;
}

// 1.1 ȸ������ UI Boundary class
// Created : 2018/5/29
// Author : ������
class SignUpUI {
public:
	void inputSignUpInfo();				// ȸ�� ���� UI���� ������ �Է¹޴� �Լ�
};

// 1.1 ȸ������ ���� �Է� �Լ�
// ������ �Է¹޾� signUp �Լ��� �Է¹��� ȸ������ ������ �ѱ��
// Created : 2018/5/22
// Author : ������
void SignUpUI::inputSignUpInfo() {
	// input.txt���� �Է�
	char userType[MAX_STRING], name[MAX_STRING], ssn[MAX_STRING], address[MAX_STRING],
		id[MAX_STRING], password[MAX_STRING];
	fscanf(in_fp, "%s %s %s %s %s %s", userType, name, ssn, address, id, password);
	SignUp su;
	su.inputSignUpInfo(userType, name, ssn, address, id, password);
	return;
}

SignUpUI signUpUI;

// 1.2 ȸ��Ż�� control class
// Created : 2018/5/30
// Author : ������
class Withdraw {
public:
	void doWithdraw();
};

// 1.2 ȸ��Ż�� �޴� �Լ�
// ȸ��Ż�� ��û�ϸ� ���ԵǾ��� �������� �����ϴ� �ش� ȸ��(Member) Ŭ������ �����Ѵ�
// ȸ�� ������ �˻��� host, guest�� �� �ٸ��� �����Ѵ�
// isNotValidWithdraw ������ true�̸� ȸ��Ż�� �Ұ����ϰ� false�̸� �����ϴ�
// Created : 2018/5/20
// Author : ������
// 1. When & Who : 2018/05/30 ������
// �Լ� �ϼ�
void Withdraw::doWithdraw() {
	char* userType = memberArray[memberNumber].getUserType();
	bool isNotValidWithdraw;

	if (memberNumber == -1) {
		fprintf(out_fp, "1.2 ȸ��Ż�� ����, �α��� ���� �ƴմϴ�.\n");
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
		fprintf(out_fp, "1.2 ȸ��Ż��\n%s\n", memberArray[memberNumber].getID());
		memberArray[memberNumber].deleteMember();
	}
	else {
		fprintf(out_fp, "1.2 ȸ��Ż�� ����, ��ϼ��ҳ� ���������� �����ֽ��ϴ�.\n");
	}
	return;
}

// 1.2 ȸ��Ż�� boundary class
// Created : 2018/5/30
// Author : ������
class WithdrawUI {
public:
	void doWithdraw();
};

// 1.2 ȸ��Ż�� UI �Լ�
// Created : 2018/5/20
// Author : ������
// Revsions :
// 1. When & Who : 2018/05/30 ������
// �Լ� �ϼ�
void WithdrawUI::doWithdraw() {
	Withdraw withdraw;
	withdraw.doWithdraw();
	return;
}
WithdrawUI withdrawUI;

// 2.1 �α��� control class
// Created : 2018/5/29
// Author : ������
class Login {
public:
	void inputLoginInfo(char*, char*);
};

// 2.1 �α��� �޴� �Լ�
// �Լ� : void login(char*,char*)
// �α��ν� �Է¹��� ������ �ް� ��� Ŭ������ ����Ǿ� �ִ� id,passwd�� ��ġ�ϴ��� ������
// �α��� �˻縦 ���� isValidLogin �Լ��� id, passwd�� �ѱ��.
// �Ű����� : char * id - ���̵�, char * password - ��й�ȣ
// Created : 2018/5/26
// Author : ������
// Revsions :
// 1. When & Who : 2018/05/26 22:25 pm ������
// ���� : �α����� �̹� �Ǿ������� �� �α��� ���� �ʵ��� �߰�
// ���� �� �� if (memberNumber != -1�� IF�� �߰�
void Login::inputLoginInfo(char* id, char* password) {
	if (memberNumber != -1) {
		fprintf(out_fp, "2.1 �α���\n");
		fprintf(out_fp, "�α��� ����, �̹� �α��� �Ǿ� �ֽ��ϴ�\n");
		return;
	}
	Member mem;
	bool isValid = mem.isValidLogin(id, password);
	if (isValid == false) {
		fprintf(out_fp, "2.1 �α���\n");
		fprintf(out_fp, "�α��� ����, ID�� �������� �ʰų� ID�� ��й�ȣ�� ��ġ���� �ʽ��ϴ�\n");
		return;
	}

	// output.txt�� ���
	fprintf(out_fp, "2.1 �α���\n");
	fprintf(out_fp, "%s %s\n", id, password);
	return;
}

// 2.1 �α��� boundary class
// Created : 2018/5/29
// Author : ������
class LoginUI {
public:
	void inputLoginInfo();
};

// 2.1 �α��� ���� �޴� �Լ�
// �Լ� : void inputLoginInfo()
// ������ �Է¹޾� loginUp �Լ��� �α��� ������ �ѱ�� 
// Created : 2018/5/26
// Author : ������
void LoginUI::inputLoginInfo() {
	char id[MAX_STRING], password[MAX_STRING];
	fscanf(in_fp, "%s %s", id, password);

	Login l;
	l.inputLoginInfo(id, password);
	return;
}
LoginUI loginUI;

// 2.2 �α׾ƿ� control class
//Created : 2018/5/29
//Author : ������
class Logout {
public:
	void doLogout();
};

// 2.2 �α׾ƿ� ���� �Լ�
// �Լ� : void doLogout()
// �α׾ƿ��� �Խ�Ʈ�������� ���ư������� �ٽ� memberNumber�� -1�� �Ѵ�.
// Created : 2018/5/29
// Author : ������
// Revsions :
// 1. When & Who : 2018/05/30 ������
// ���� : �Խ�Ʈ �����̸� �α׾ƿ� ���� �޽��� �߰�
// ���� �� �� if (memberNumber == -1)
//		fprintf(out_fp, "2.2 �α׾ƿ�\n�α׾ƿ� ����, �̹� �Խ�Ʈ �����Դϴ�.\n");
void Logout::doLogout() {
	if (memberNumber == -1) {
		fprintf(out_fp, "2.2 �α׾ƿ�\n�α׾ƿ� ����, �̹� �Խ�Ʈ �����Դϴ�.\n");
		return;
	}
	fprintf(out_fp, "2.2 �α׾ƿ�\n");
	fprintf(out_fp, "%s \n", memberArray[memberNumber].getID());
	memberNumber = -1;
	return;
}

// 2.2 �α׾ƿ� boundary class
// Created : 2018/5/29
// Author : ������
class LogoutUI {
public:
	void doLogout();
};

// 2.2 �α׾ƿ� UI �Լ�
// �Լ� : void doLogout()
// �α׾ƿ��ϴ� logout�Լ� ����
// Created : 2018/5/29
// Author : ������
void LogoutUI::doLogout() {
	Logout l;
	l.doLogout();
	return;
}
LogoutUI logoutUI;


// Ŭ���� : RegisterAcc
// 3.1 ���ҵ�� ��Ʈ�� 
// UIŬ������ ���� ������ �޾� ����Ŭ������ �Լ��� �ִ´�.
// Created : 2018/5/26
// Author : �̴��
// Revsions :
// 1. When & Who : 2018/05/27 �̴��
// ���� : �Լ��� Ŭ������ ����
class RegisterAcc {
private:

public:
	void inputInfo(char*, char*, char*, char*, char*);
};
// �Լ� : inputInfo(char*accName, char* accAddress, char* accPrice, char* reservationPossibleDate, char* oiPrice)
// ���� : ���� ��Ͻ� ����� ���� ����
// ���� �̸�, ���� �ּ�, ���� ���� ��¥, ���� ����,OI������
// UI�� ���� �Է¹޾� ���� �迭�� �����Ѵ�.
// �迭�� ũ�⸸ŭ Ȯ���ϸ鼭 ����ִ� �迭�� ã�� ������ �����Ѵ�.
// Created : 2018/5/27 
// Author : �̴��
// Revsions :
// 1. When & Who : 2018/05/30 �̴��
// ���� : ���� Ÿ���� ȣ��Ʈ�� ��츸 �����ϵ��� ���� ���� ����
// 1�� �� �ڵ����� ����� �����ϱ� ����� �߰� �׶� �ʿ���
// ���Ҹ� ����� ���(ȣ��Ʈ)�� ���̵�� ���� �ð��� �����ϱ�����
// �Է°��� �߰���.
void RegisterAcc::inputInfo(char*accName, char* accAddress, char* accPrice, char* reservationPossibleDate, char* oiPrice) {



	for (int i = 0; i < MAX_ACC_NUM; i++) {
		if (strcmp(accArray[i].getAccName(), "") == 0) {
			accArray[i].registerHostAcc(accName, accAddress, accPrice, reservationPossibleDate, oiPrice, memberArray[memberNumber].getID(), currentTime);
			break;
		}
	}

}

// Ŭ���� : RegisterAccUI
// 3.1 ���ҵ�� UI
// ���������� �ִ� inputAccInfo�Լ� ����
// Created : 2018/5/26
// Author : �̴��
//input.txt���� �Է��� �޾� ���ҵ�� ��Ʈ�ѷ� ������ �Ѱ���
// Revsions :
// 1. When & Who : 2018/05/27 �̴��
// ���� : �Լ��� Ŭ������ ����,���� Ÿ���� ȣ��Ʈ�� ��츸 �����ϵ��� ���� ���� ����
class RegisterAccUI {//UI
private:
	RegisterAcc registerAcc;
public:
	void inputInfo();
};
// �Լ� : inputInfo()
// ���� : ���� ��Ͻ� ����� ���� ����
// ���� �̸�, ���� �ּ�, ���� ���� ��¥, ���� ����,OI������
// input.txt���� �Է¹޾� ��Ʈ�� Ŭ������ ������
//
// Created : 2018/5/27 
// Author : �̴��
// Revsions :
// 1. When & Who : 2018/05/28 �̴��
// ���� : ���� Ÿ���� ȣ��Ʈ�� ��츸 �����ϵ��� ���� ���� ����
void RegisterAccUI::inputInfo() {
	char accName[MAX_STRING], accAddress[MAX_STRING], reservationPossibleDate[MAX_STRING];
	char accPrice[MAX_STRING], oiPrice[MAX_STRING];


	fprintf(out_fp, "3.1 ���ҵ��\n");
	if (strcmp(memberArray[memberNumber].getUserType(), HOST) == 0) {
		fscanf(in_fp, "%s %s %s %s %s", accName, accAddress, accPrice, reservationPossibleDate, oiPrice);
		registerAcc.inputInfo(accName, accAddress, accPrice, reservationPossibleDate, oiPrice);
	}
	else {
		fprintf(out_fp, "������ �����ϴ�.\n");
	}
}

RegisterAccUI registerAccUI;

// Ŭ���� : ShowHostAccInfo
// 3.2 ���� ��ȸ ��Ʈ��
// Created : 2018/5/26
// Author : �̴��
// ����Ŭ������ ȣ��Ʈ�� ����� ����������
//����Ŭ�������� ������.
class ShowHostAccInfo {//Control
private:
public:
	void showHostAcc();

};
// �Լ� : showHostAcc()
// ���� : ���� ��ȸ�� ��ȸ�� ���� ����
// ���� Ŭ�������� ���� ����(���� �̸�, ���� �ּ�, ���� ��¥, ���� ����, ���� ����, OI����)��
// �����ص� �ӽ� �迭�� ����� �����Ѵ�.
// ���� ����� ���� �����̴�. strcmp�� ��¥�� ���� ���� ���� ���డ�� ��¥�� ���� ����Ѵ�.
// Created : 2018/5/27 
// Author : �̴��
// Revsions :
// 1. When & Who : 2018/05/28 �̴��
// ���� : ���� ���� ��¥�� ���� ������ ���� �� ��� ����
void ShowHostAccInfo::showHostAcc() {//Control

	char *tmp[100];//������ ���� �ӽ� �迭
	count = 0;//�������� ī��Ʈ �ʱ�ȭ

			  //���� �迭�� ��� ã�ƺ��� ������� ����
			  //�迭�� ������ �����´�.
	for (int i = 0; i < MAX_ACC_NUM; i++) {

		if (strcmp(accArray[i].getAccName(), "") != 0) {

			accArray[i].getHostAcc();

		}
	}
	//ȣ��Ʈ�� ����� ���Ұ����� ��ü �迭�� ũ���̴�.
	//����� ������ ���� �̸�, ���� �ּ�, ���� ��¥, ���� ����, ���� ����, OI��������
	//������� 0,1,2,3,4,5 �� �ɼ��� ������. �� �ɼ��� �������� ������ �Ѵ�
	for (int j = 0; j < memberArray[memberNumber].getRegisterAccNum() - 1; j++) {
		for (int i = 0; i < memberArray[memberNumber].getRegisterAccNum() - 1 - j; i++) {
			if (strcmp(tempAcc[6 * i + 2], tempAcc[6 * (i + 1) + 2]) > 0) {
				//���� �̸� ��ȯ
				tmp[6 * i] = tempAcc[6 * i];
				tempAcc[6 * i] = tempAcc[6 * (i + 1)];
				tempAcc[6 * (i + 1)] = tmp[6 * i];
				//���� �ּ� ��ȯ
				tmp[6 * i + 1] = tempAcc[6 * i + 1];
				tempAcc[6 * i + 1] = tempAcc[6 * (i + 1) + 1];
				tempAcc[6 * (i + 1) + 1] = tmp[6 * i + 1];
				//���� ��¥ ��ȯ
				tmp[6 * i + 2] = tempAcc[6 * i + 2];
				tempAcc[6 * i + 2] = tempAcc[6 * (i + 1) + 2];
				tempAcc[6 * (i + 1) + 2] = tmp[6 * i + 2];
				//���� ���� ��ȯ
				tmp[6 * i + 3] = tempAcc[6 * i + 3];
				tempAcc[6 * i + 3] = tempAcc[6 * (i + 1) + 3];
				tempAcc[6 * (i + 1) + 3] = tmp[6 * i + 3];
				//���� ���� ��ȯ
				tmp[6 * i + 4] = tempAcc[6 * i + 4];
				tempAcc[6 * i + 4] = tempAcc[6 * (i + 1) + 4];
				tempAcc[6 * (i + 1) + 4] = tmp[6 * i + 4];
				//OI ���� ��ȯ
				tmp[6 * i + 5] = tempAcc[6 * i + 5];
				tempAcc[6 * i + 5] = tempAcc[6 * (i + 1) + 5];
				tempAcc[6 * (i + 1) + 5] = tmp[6 * i + 5];
			}
		}
	}
	//���ĵǾ������� ���� �̸�, ���� �ּ�, ��� ��¥, ���� ����, ���� ����, OI���ݼ����� ����Ѵ�.
	if (strcmp(tempAcc[0], "") != 0) {
		for (int i = 0; i < memberArray[memberNumber].getRegisterAccNum(); i++) {
			for (int j = 0; j < 6; j++) {
				fprintf(out_fp, "%s ", tempAcc[6 * i + j]);
			}
			fprintf(out_fp, "\n");
		}
	}
}

// Ŭ���� : ShowHostAccInfoUI
// 3.2 ���� ��ȸ UI
// Created : 2018/5/26
// Author : �̴��
// ����Ŭ������ ȣ��Ʈ�� ����� ���������� ��û��.
// Revsions :
// 1. When & Who : 2018/05/27 �̴��
// ���� : �Լ��� Ŭ������ ����
class ShowHostAccInfoUI {//UI
private:
	ShowHostAccInfo showHostAccInfo;
public:
	void showHostAcc();
};

// �Լ� : showHostAcc()
// ���� : ���� ��ȸ�� ��ȸ�� ���� ����
// ��Ʈ�� Ŭ������ ��û�Ѵ�.
// Created : 2018/5/27 
// Author : �̴��
// Revsions :
// 1. When & Who : 2018/05/28 �̴��
// ���� : ���� Ÿ���� ȣ��Ʈ�� ��츸 �����ϵ��� ���� ���� ����
void ShowHostAccInfoUI::showHostAcc() {

	fprintf(out_fp, "3.2 ������ȸ\n");

	if (strcmp(memberArray[memberNumber].getUserType(), HOST) == 0)
		showHostAccInfo.showHostAcc();
	else
		fprintf(out_fp, "������ �����ϴ�.\n");
}

ShowHostAccInfoUI showHostAccInfoUI;

// Ŭ���� : SearchAcc
// 4.1 ���� ������ ���� �˻��ϱ� ��Ʈ��
// Created : 2018/05/27
// Author : ��ö��
// Revsions :
// 1. When & Who : 2018/05/27 �̴��
// ���� : �Լ��� Ŭ������ ����
class SearchAcc {
public:
	void searchAcc(char*, char*);
};

// �Լ� : searchAcc(char* accAddress, char* reservationDate)
// ���� : ���� ������ ���� �˻��� ������ ���� ����
// ���� �ּ�, ���� ���� ��¥��
// UI Ŭ�������� �޾� ���� ��ƼƼ���� 
// ������ �����´�.
// Created : 2018/5/27 
// Author : �̴��
void SearchAcc::searchAcc(char* accAddress, char* reservationDate) {

	for (int i = 0; i<MAX_ACC_NUM; i++) {
		if (strcmp(accArray[i].getAccName(), "") != 0)
			accArray[i].searchGuestAcc(accAddress, reservationDate);
	}
}

// Ŭ���� : SearchAccUI
// 4.1 ���� ������ ���� �˻��ϱ� UI
// Created : 2018/05/27
// Author : ��ö��
// Revsions :
// 1. When & Who : 2018/05/27 �̴��
// ���� : �Լ��� Ŭ������ ����
class SearchAccUI {
private:
	SearchAcc searchAcc;
public:
	void inputReservationInfo();
};

// �Լ� : inputReservationInfo()
// ���� : ���� ������ ���� �˻��� ������ ���� ����
// ���� �ּ�, ���� ���� ��¥��
// input.txt���� �Է¹޾� ��Ʈ�� Ŭ������ ������
//
// Created : 2018/5/27 
// Author : �̴��
void SearchAccUI::inputReservationInfo() {
	char accAddress[MAX_STRING], reservationDate[MAX_STRING];
	fprintf(out_fp, "4.1 ���Ұ˻�\n");
	fscanf(in_fp, "%s %s", accAddress, reservationDate);
	searchAcc.searchAcc(accAddress, reservationDate);
}

SearchAccUI searchAccUI;

// Ŭ���� ����
// class : MakeReservation (���ҿ���� �ʿ��� control object�� class�� ����)
// Created : 2018/05/30
// Author : ��ö��
class MakeReservation {
private:
	ReservationInfo mrR;
	Accomodation mrA;
public:
	MakeReservation();
	void selectReservation(char*, char*);	// ���޹��� ������ ������ ����
};
MakeReservation::MakeReservation() {

}

// �Լ� : selectReservation
// ������ �ش� ������ ȣ��Ʈ ���̵�� �����̸��� �޾Ƶ鿩 ���������� ������ �߰��Ѵ�
// ������ �Ϸ��� �� ������ reservation �׸��� O �� �����϶�� ��û�Ѵ�.
// Created : 2018/05/31
// Author : ��ö��
void MakeReservation::selectReservation(char* hostID, char* accName) {
	fprintf(out_fp, "4.2 ���ҿ���\n");
	mrR.makeReservation(hostID, accName);	//reservationInfo ��ƼƼ Ŭ������ �Լ��� ���
											//mrA.set_reservation('O');
											//������ ���������� reservation ���� 'O'�� �������ش�.
	for (int i = 0; i < MAX_ACC_NUM; i++) {
		if ((strcmp(accArray[i].getAccName(), accName) == 0) && (strcmp(accArray[i].getHostID(), hostID) == 0)) {
			accArray[i].setReservation('O');
		}
	}
}

// Ŭ���� ����
// class : MakeReservationUI (���ҿ���� �ʿ��� boundary object�� class�� ����)
// Created : 2018/05/30
// Author : ��ö��
class MakeReservationUI {
private:
	MakeReservation mrui;
public:
	MakeReservationUI();
	void selectReservationInfo();	// ����ڰ� ������ ���� �д´�.
};

MakeReservationUI::MakeReservationUI() {

}

// �Լ� : selectReservationInfo
// ����ڰ� ���డ���� ���Ҹ���Ʈ���� ������ �ϰ��� �ϴ� ���Ҹ� ������ ���
// ��û�� ���� �޾Ƶ��δ�.
// Created : 2018/05/31
// Author : ��ö��
void MakeReservationUI::selectReservationInfo() {
	char hostID[MAX_STRING], accName[MAX_STRING];
	fscanf(in_fp, "%s %s", hostID, accName);
	mrui.selectReservation(hostID, accName);	//makeReservation control Ŭ������ �Լ��� ���
}

MakeReservationUI makeReservationUI;


// Ŭ���� ����

// class: ReserveOI(opaque inventory ���� ��Ʈ�� Ŭ����)
// opaque inventory ���� ��Ʈ�� Ŭ������ oi������ ����
// Created : 2018/5/29
// Author : ������
class ReserveOI {
public:
	void inputInfo(char*, char*, char*);
};
// �Լ� : inputInfo(char*accAddress, char* reservationDate, char* cost)
// ���� : UI���� ���� �޾ƿ� Opaque inventory �õ��ð����� 24�ð� ���ΰ� Ȯ���ϰ� �Խ�Ʈ�� ��
// ���ǵ�� ���Ұ� ��ġ�ϸ� �ش� ���ҿ� ���õ� ���������� �����.
// �Ű����� : char*accAddress - �����ּ�, char* reservationDate - �̿밡�ɳ�¥, char* cost - �Խ�Ʈ�������
// Created : 2018/5/28 
// Author : ������
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



// Ŭ���� ����

// class: OIReserveUI(opaque inventory ���� �ٿ���� Ŭ����)
// opaque inventory ���� �ٿ���� Ŭ������ oi���� ������ �޴� ui����
// Created : 2018/5/29
// Author : ������
class OIReserveUI {
private:
	ReserveOI reserveOI;

public:

	void inputInfo();
};

// �Լ� : inputInfo()
// ���� : ���� �Է¹޾� opaque inventory �����ϱ� ���� reserveOI���� �Է¹��� ���� �Ѱ��ش�.
// Created : 2018/5/28 
// Author : ������
void OIReserveUI::inputInfo() {
	fprintf(out_fp, "4.3 Opaque inventory ����\n");
	char accAddress[MAX_STRING], reservationDate[MAX_STRING], cost[MAX_STRING];

	fscanf(in_fp, "%s %s %s", accAddress, reservationDate, cost);

	reserveOI.inputInfo(accAddress, reservationDate, cost);
}
OIReserveUI reserveOIUI;


// Ŭ���� ����
// class : ShowReservation (������ ���� �˻��� �ʿ��� control object�� class�� ����)
// created : 2018/05/30
// Author : ��ö��
class ShowReservation {
private:
	ReservationInfo srR;
public:
	ShowReservation();
	void showReservationInfo();
};
ShowReservation::ShowReservation() {

}

// �Լ� : showReservationInfo
// ���������� ����ϱ� ���� ReservationInfo Ŭ������ showReservationList �Լ��� ȣ���Ѵ�.
// Created : 2018/05/31
// Author : ��ö��
void ShowReservation::showReservationInfo() {
	fprintf(out_fp, "4.4 �������� ��ȸ\n");
	srR.showReservationList();
}

// Ŭ���� ����
// class : ShowReservationInfoUI (������ ���� �˻��� �ʿ��� boundary object�� class�� ����)
// created : 2018/05/30
// Author : ��ö��
class ShowReservationInfoUI {
private:
	ShowReservation sriui;
public:
	ShowReservationInfoUI();
	void requestReservationInfo();
};
ShowReservationInfoUI::ShowReservationInfoUI() {

}

// �Լ� : showReservationInfo
// ����ڰ� ������ ���� ��ȸ�� ��û�Ұ�� �ش� �Լ� ���.
// showRevervation control �� showReservationInfo�� ��û�Ѵ�.
// Created : 2018/05/31
// Author : ��ö��
void ShowReservationInfoUI::requestReservationInfo() {
	//ShowReservation (Control class)�� showReservationInfo()�� ȣ���Ѵ�.
	sriui.showReservationInfo();
}

ShowReservationInfoUI showReservationInfoUI;




// main �Լ�
// input.txt�� output.txt�� ���� �޴� ���� �Լ��� doTask()�� �����Ѵ�.
int main() {
	in_fp = fopen(INPUT_FILE_NAME, "r+");
	out_fp = fopen(OUTPUT_FILE_NAME, "w+");

	doTask();

	return 0;
}




// doTask �Լ�
// 1 1, 5 1 ������ ǥ�õ� input.txt ���� �о� �ش� �޴��� �����ϴ� �Լ��� switch�Ѵ�.
void doTask() {
	int menu_level_1 = 0, menu_level_2 = 0;
	int is_program_exit = 0;

	while (!is_program_exit)
	{
		// �Է� ���Ͽ��� �޴� ���� 2���� �б�
		fscanf(in_fp, "%d %d ", &menu_level_1, &menu_level_2);

		// �޴� ���� �� �ش� ���� ����
		switch (menu_level_1)
		{
		case 1:
			switch (menu_level_2)
			{
			case 1: // 1.1 ȸ������ �޴�

				signUpUI.inputSignUpInfo();
				break;
			case 2: // 1.2 ȸ��Ż�� �޴�

				withdrawUI.doWithdraw();
				break;
			}
			break;

		case 2:
			switch (menu_level_2)
			{
			case 1: // 2.1 �α��� �޴�

				loginUI.inputLoginInfo();
				break;
			case 2: // 2.2 �α׾ƿ� �޴�

				logoutUI.doLogout();
				break;
			}
			break;

		case 3:
			switch (menu_level_2)
			{
			case 1: // 3.1 ���ҵ�� �޴�
				registerAccUI.inputInfo();
				break;
			case 2: // 3.2 ��� ���� ��ȸ �޴�
				showHostAccInfoUI.showHostAcc();
				break;
			}
			break;

		case 4:
			switch (menu_level_2)
			{
			case 1: // 4.1 ���� �˻� �޴�
				searchAccUI.inputReservationInfo();
				break;
			case 2: // 4.2 ���� ���� �޴�
				makeReservationUI.selectReservationInfo();
				break;
			case 3: // 4.3 Opaque Inventory ���� �޴� 
				reserveOIUI.inputInfo();
				break;
			case 4: // 4.4 ���� ���� ��ȸ �޴�
				showReservationInfoUI.requestReservationInfo();
				break;
			}
			break;

		case 5:
			switch (menu_level_2)
			{
			case 1: // 5.1 ���� �ð� ���� �޴�
				timer.setTimer();
				timer.deleteAccOverYear();
				break;
			}
			break;

		case 6:
			switch (menu_level_2)
			{
			case 1: // 6.1 Session ���� �޴�
				changeSession();
				break;
			case 2: // 6.2 Guest Session���� ���� �޴�
				changeGuest();
				break;
			}
			break;

		case 7:
			switch (menu_level_2)
			{
			case 1: // 7.1 ���� �޴�
				program_exit();
				is_program_exit = 1;
				break;
			}
			break;
		}
	}
	return;
}


// 6.1 ���Ǻ����Լ�
// �Լ� : void changeSession(char*id)
// �Էµ� id�� �ش��ϴ� �������� �ٲٱ����� �ش� id�� ã�� �ش��ϴ� �ε����� ��ȯ �Լ�
// Created : 2018/5/26
// Author : ������
void changeSession() {
	int i = 0;
	char id[MAX_STRING];
	fprintf(out_fp, "6.1 Session ����\n");
	fscanf(in_fp, "%s", id);
	for (i = 0; i < memberCount; i++) {
		if (!strcmp(memberArray[i].getID(), id)) {
			memberNumber = i;
			fprintf(out_fp, "%s\n", id);
			return;
		}
	}
	if (i == memberCount) {

		fprintf(out_fp, "�Էµ� id�� �ش��ϴ� ����� �����ϴ�.\n");
	}
	return;
}


// 6.2 guest session���� ����
// �Լ� : void changeGuest()
// guestsession���� �ٲٱ� ���� memberCount�� -1�� �ٲٴ� �Լ�
// Created : 2018/5/26
// Author : ������
void changeGuest() {
	memberNumber = -1;
	fprintf(out_fp, "6.2 guest session���� ����\n");
	return;
}


// 7.1 ���α׷� ���� �޴� �Լ�
// doTask �Լ��� �����ϰ� output.txt�� ���� ���
// Created : 2018/5/20
// Author : ������
void program_exit() {
	fprintf(out_fp, "7.1 ����\n");
	return;
}