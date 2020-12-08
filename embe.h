#ifndef _embe_drv_
#define _embe_drv_

typedef struct{
	int year;
	int month;
	int day;
	int hour;
	int min ;
	int sec ;
} hourAndMinute; //카운트 업과 카운트 다운에 필요한 시간과 날짜를 저장해주기 위해서 만든 구조체

int goalstudy(void); //목표 시간을 정하는 기능을 하는 함수
int countup(void); //1초씩 올리면서 0에서부터 시작하는 
					//종료키를 누르면 파일에 날짜에 따른 공부한 양을 초로 저장
int countdown(void); //목표 시간에서 1초씩 내리는 함수 goalstudy 는 이 함수에서 사용한다. 
					//종료시 파일에 날짜에 따른 공부량 초로 저장
int timer(void);  //카운트 다운, 카운트 업, 현재시간을 선택할 수 있는 메뉴
int daystudy(void); //오늘 공부량을 파일에서 read한 후에 날짜와 공부량 출력
int weekstudy(void); //1주일 총 공부량을 파일에서 읽어와서 1주일 합산 공부량과 각각의 날짜 공부량 출력
int setInit(void); //프로그램을 시작할때 각 기기의 초기설정을 해주고 온도값을 읽어와서 그에 따른 컨디션
					//출력
int setExit(void); //열어놨던 각종 기기들과 txt 파일을 close 해준다.


#endif
