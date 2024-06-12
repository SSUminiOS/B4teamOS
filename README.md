# miniOS

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

# miniOS 이용방법
gcc -o miniOS main.c memory.c schedule.c process.c stats.c -lm

# booting.c 이용방법
gcc -o booting main.c booting.c -lm
