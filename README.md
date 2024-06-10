# miniOS

miniOS-project/  
├── README.md               # 프로젝트 설명 및 사용 방법 문서  
├── Makefile                # 전체 프로젝트 빌드 자동화를 위한 메이크파일  
├── boot/                   # 부트로더 소스 코드  
O   └── boot.asm            # 부트로더 어셈블리 코드  
├── kernel/                 # 커널 소스 코드  
O   ├──  kernel.c           # 커널 메인 C 소스 파일
O   └──  system.c           # system C 소스 파일
├── drivers/                # 디바이스 드라이버 코드  
O   └──  basic_dirver.c     # 화면(비디오) 드라이버  
├── lib/                    # 커널 라이브러리 및 공통 유틸리티  
O   └──  basic.c            # 형식 파일  
├── include/                # 헤더 파일  
O   ├── basic.h             # 헤더 형식  
O   └── system.h            # 드라이버 헤더 파일
└── scripts/                # 빌드 및 유틸리티 스크립트  
O   └── build.sh            # 빌드 스크립트  


