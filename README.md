# StudyFaceMaker -Team Project

## 개요

<img src = "https://user-images.githubusercontent.com/59462895/104881044-fcc14d00-59a3-11eb-8110-ecbf0625dae7.jpg"></img>

이는 공부를 하면서 시간을 재고 그 누적된 시간을 확인할 수 있게 해주게 하는 소프트웨어를 만드는 프로젝트이다. 리눅스 드라이버를 사용한 어플리케이션으로 각 센서나 장치를 제어하였다. 이 프로젝트는 명지대학교 전자공학과 임베디드 시스템에서 팀프로젝트로 진행되었다.

## 사용 키트

 - [eCube Embedded All-In-One](http://www.cndi.co.kr/cndi/board/bbs/board.php?bo_table=imbe&wr_id=1)

<img width = "600" src = "https://user-images.githubusercontent.com/59462895/104881492-bd473080-59a4-11eb-90b4-ed7a9bd2eee5.JPG"></img>

 - 세부 스펙 (소프트웨어 및 하드웨어)

<img src = "https://user-images.githubusercontent.com/59462895/104880613-39d90f80-59a3-11eb-82b4-d1de54120500.JPG"></img>

## 플로우 차트
 - 메인 메뉴

|메인 메뉴|해당 일, 주간 시간 출력|
|--|--|
|<img width = "350" src = "https://user-images.githubusercontent.com/59462895/104881937-8b829980-59a5-11eb-91d0-039603a2f507.jpg"></img>|<img width = "330" src = "https://user-images.githubusercontent.com/59462895/104881959-950c0180-59a5-11eb-8b7e-26533cfee73a.jpg"></img>|
   

 - 타이머 메뉴

<img width = "350" src = "https://user-images.githubusercontent.com/59462895/104881967-96d5c500-59a5-11eb-8190-0ec900064d73.jpg"></img>
|카운트 다운|카운트 업|
|--|--|
|<img  src = "https://user-images.githubusercontent.com/59462895/104882029-aead4900-59a5-11eb-8f5a-c0ca07760398.jpg"></img>|<img width = "" src = "https://user-images.githubusercontent.com/59462895/104882038-b10fa300-59a5-11eb-8853-38087db5208a.jpg"></img>|

## 구현 기능
 - 버튼으로 점층적인 메뉴구현

 - 시작 시 온도센서를 이용해서 공부하기 좋은 온도인지를 구별

|20도 이하|20도 미만 26도 이하|26도 초과|
|--|--|--|
|<img  src = "https://user-images.githubusercontent.com/59462895/104882859-1f089a00-59a7-11eb-8c44-b4c8c0621582.jpg"></img>|<img width = "" src = "https://user-images.githubusercontent.com/59462895/104882869-22038a80-59a7-11eb-9383-d8d642a45eae.jpg"></img>|<img width = "" src = "https://user-images.githubusercontent.com/59462895/104882879-23cd4e00-59a7-11eb-8413-5e0cf22c8195.jpg"></img>|

 - 카운트 업, 다운 시 log 파일에 공부시간 저장 (log 파일이 없을 시 자동으로 생성된다.)

<img  src = "https://user-images.githubusercontent.com/59462895/104883072-7c9ce680-59a7-11eb-9ff6-40852306db7a.jpg"></img>

 - 1주일 내로 공부시간 확인 가능

<img width = "700" src = "https://user-images.githubusercontent.com/59462895/104883192-bd94fb00-59a7-11eb-8cd9-513927bdd87c.jpg"></img>

 - 카운트 다운시에 led에 목표시간에서 달성비율만큼 led 로 알려주는 기능

 <img  width = "400" src = "https://user-images.githubusercontent.com/59462895/104883510-37c57f80-59a8-11eb-9062-757de349178c.JPG"></img>
   
   
   또한, 해당 시간을 달성할 경우 color led 가 반짝거리고 buzzer 가 울리면서 해당시간이 되었음을 알려준다.

 - 현재 시간 확인 기능
   
   현재 시간을 FND 를 통해서 확인할 수 있다.

## 실행 영상
 - 카운트 다운 기능

<img  width = "400" src = "https://user-images.githubusercontent.com/59462895/104886137-9856bb80-59ac-11eb-8359-6495a78be4e8.gif"></img>

 - 카운트 업 기능

<img  width = "400" src = "https://user-images.githubusercontent.com/59462895/104885147-e36fcf00-59aa-11eb-8759-72cb4ba82257.gif"></img>

 - 현재 시간 확인

<img  width = "400" src = "https://user-images.githubusercontent.com/59462895/104883867-c33f1080-59a8-11eb-8010-96d0c0f06ce0.jpg"></img>

 - 일일 학습량 확인

<img  width = "" src = "https://user-images.githubusercontent.com/59462895/104885364-4e210a80-59ab-11eb-8a05-19bb17aa0d72.jpg"></img>

 - 주간 학습량 확인

<img  width = "" src = "https://user-images.githubusercontent.com/59462895/104885375-524d2800-59ab-11eb-9513-ce456a19b592.jpg"></img>


## 조원  
 - 전자공학과 신인우   
 메인 어플리케이션 작성   
 버저, FND, LED, TEXTLCD, BUTTON 드라이버 및 라이브러리 및 작성   
 각 센서 테스트 파일 작성,  센서별 MAKEFILE 작성  
 
 - 전자공학과 김홍민   
 비트맵 이미지, 온도센서, COLOR LED, 가속도 센서 드라이버 및 라이브러리 작성   
 플로우차트 및 계획서 작성 및 비트맵 이미지 파일 제작
