# 크기가 큰 파일이 있어 lfs를 사용했습니다.

## 설정 방법

```sh
# 압축을 푼 디렉토리로 이동
cd %USERPROFILE%\Desktop\AiV_VisionSW-master

# 저장소를 Git 저장소로 초기화
git init
git remote add origin https://github.com/joeyjhg/AiV_VisionSW.git
git fetch origin
git reset --hard origin/master

# Git LFS 설치 확인 및 재설치
git lfs install

# Git LFS 파일 목록 확인
git lfs ls-files

# Git LFS 파일 다운로드
git lfs pull
