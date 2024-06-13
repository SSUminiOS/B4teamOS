# miniOS
## 수행목표
user 모드에서 명령을 하면 kernel에서 처리하는 작업을 직접 설계하는 Mini OS 제작

# 현 파일들
miniOS-project/

├── sched.c/                            # 발표 및 보고서 자료

O   ├──  4조_운영체제_최종발표자료. pptx  # 발표자료 ppt

O   ├──  4조_운영체제_최종보고서.docx     # 보고서 docx파일

O   └──  4조_운영체제_최종보고서.pdf      # 보고서 pdf파일

├── sched.c                             # weighted round-robin scheduling 구현 C파일

├── thread.h                            # threading 구현 헤더파일 

├── main.c                              # 실제 miniOS 시연 C파일

├── memory.c                            # miniOS에서 사용한 memory management C파일 

├── memory.h                            # miniOS에서 사용한 memory management 헤더파일 

├── process.c                           # miniOS에서 사용한 process C파일

├── process.h                           # miniOS에서 사용한 process 헤더파일 

├── schedule.c                          # miniOS에서 사용한 scheduling C파일

├── schedule.h                          # miniOS에서 사용한 scheduling 헤더파일 

├── stats.c                             # miniOS에서 사용한 UI C파일  

├── stats.h                             # miniOS에서 사용한 UI 헤더파일 

├── booting.c                           # booting 화면 출력 C파일

├── README.md                           # 프로젝트 설명 및 사용 방법 문서  

├── Makefile                            # 전체 프로젝트 빌드 자동화를 위한 메이크파일 

├── boot/                               # 부트로더 소스 코드  

O   └── boot.asm                        # 부트로더 어셈블리 코드  

├── kernel/                             # 커널 소스 코드  

O   ├──  kernel.c                       # 커널 메인 C 소스 파일

O   └──  system.c                       # system C 소스 파일

├── drivers/                            # 디바이스 드라이버 코드  

O   └──  basic_dirver.c                 # 화면(비디오) 드라이버  
├── lib/                                # 커널 라이브러리 및 공통 유틸리티  

O   └──  basic.c                        # 형식 파일  

├── include/                            # 헤더 파일  

O   ├── basic.h                         # 헤더 형식  

O   └── system.h                        # 드라이버 헤더 파일

└── scripts/                            # 빌드 및 유틸리티 스크립트  

O   └── build.sh                        # 빌드 스크립트  

# 구현 내용 소개

## thread
A. 	pthread라이브러리를 참고하여 자체 thread 라이브러리인 thread.h를 생성하여, thread 역할을 하는 my_thread 구조체를 구현
B.  	pthread 라이브러리의 pthread_create와 pthread 라이브러리의 pthread_exit함수 역할을 하는 thread_wrapper 함수를 구현
C. 	두 thread간의 context switching을 담당하는 schedule 함수를 구현한다.

## scheduling 
A.	Weighted round robin 구현
1)	기존 RR 알고리즘에서 서로 다른 프로세스나 작업에 서로 다른 실행 우선순위를 부여하는 방식으로 스케줄링 구현
2)	Weighted round robin을 위한 Thread_Processing, addProcessToQueue, customScheduling을 구현

A.	코드를 병합하는 과정에서 기존 스레드 자료구조를 스케줄링할 때 커스터마이징한 코드와 계속해서 호환이 되지않는 문제가 발생
1)	스레드 자료 구조에는 ucontext와 thread_function 코드를 사용했으나 자료구조 숙련도 문제로 기존 스레드 코드에서 사용한 ucontext와 thread_function은 제거하고, 프로세스 자료구조로 전환한 다음 스레드 자료 구조를 넣어 최대한 단순하게 만들어서 포함시
2)	Thread_function 코드를 전달하여 시뮬레이션하는 과정에서 오류가 많이 발생하여, UI 기반으로 스케줄링 되고 있음을 스케줄러 입장에서 구현하도록 수정

## 메모리 
1)	프로세스가 생성되고 실행되는 과정에서 디멘드 페이징이 일어나는 과정을 최대한 단순화
2)	페이지 교체 알고리즘(LRU) 또한 모사

## UI
1)	인터럽트 등의 특정 상황을 표현하기 위해 Process id에 색을 입혀 인터럽트 되어 대기 큐에 들어가 있는 상황을 표현

# miniOS 이용방법
gcc -o miniOS main.c memory.c schedule.c process.c stats.c -lm

# booting.c 이용방법
gcc -o booting booting.c -lm
