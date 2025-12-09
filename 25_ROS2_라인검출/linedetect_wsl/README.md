2101034 고찬영

Jetson키트가 없어서 WSL 환경에서 진행했습니다.
wsl에서 Publish한 메시지를 Mat으로 변환하여 영상 프레임에서 라인을 찾고 오류(error)를 계산하는 노드입니다. 

1. 전처리
   입력 영상을 받아서 압축을 풀고, Gray변환 + 밝기 보정 + 이진화 + 하단 1/4를 roi로 만들어 라인 검출    하기 좋은 형태로 전처리 하는 코드이다.

<img width="615" height="140" alt="image" src="https://github.com/user-attachments/assets/1d307dd5-1ccd-4f43-8af9-87073a0043cf" />
