2101034 고찬영

Jetson키트가 없어서 WSL 환경에서 진행했습니다.
wsl에서 Publish한 메시지를 Mat으로 변환하여 영상 프레임에서 라인을 찾고 오류(error)를 계산하는 노드입니다. 

1. 전처리
   입력 영상을 받아서 압축을 풀고, Gray변환 + 밝기 보정 + 이진화 + 하단 1/4를 roi로 만들어 라인 검출    하기 좋은 형태로 전처리 하는 코드이다.

<img width="615" height="140" alt="image" src="https://github.com/user-attachments/assets/1d307dd5-1ccd-4f43-8af9-87073a0043cf" />

2. 레이블링
   connectedComponentsWithStats함수를 사용해여 roi 내에서 연결된 흰색 픽셀 덩어리(차선 후보)를 찾습    니다. serach_radius:첫 실행 시에는 ROI폭 전체를 반경으로 설정하여 광범위하게 탐색하고, 추적 중에     는 이전 위치(tmp_pt_)주변 60픽셀 반경 내에서 탐색합니다.
   Search_center: 첫 실행 시에는 초기 중심좌표(320,60)을 사용하고, 추적 중에는 이전 프레임에서 찾은     차선 중심(tmp_pt_)을 탐색의 기준으로 사용합니다.

   <img width="711" height="366" alt="image" src="https://github.com/user-attachments/assets/17e3711c-08c1-49f8-8c46-1e9b55d70301" />

3. 목표 차선 선택
   이진화된 ROI 내에서 가장 적ㅈ절한 차선 후보를 선택하는 과정입니다.
   배경을 제외하고 모든 연결요소에서 100픽셀 보다 큰 영역을 유효한 차선 후보로 간주합니다.
   cv::norm을 사용해 탐색 기준점과의 거리를 계산해 dist에 저장합니다.
   가장 가까운 차선 후보(min_idx != -1)가 발견되면, 해당 후보의 중심 좌표를 tmp_pt_에 저장하여 다음     프레임의 추적 기준점으로 갱신합니다.
   target_idx에 선택된 요소의 인덱스를 저장합니다.
   first_run_ = false로 설정하여 다음 프레임부터는 위의 코드를 실행하지 않도록 합니다.

   <img width="600" height="305" alt="image" src="https://github.com/user-attachments/assets/267fc88a-1385-49f4-ae12-a116f7e6121d" />

4. 시각화 및 결과 출력
   처리 결과를 사용자에게 보여주고 핵심 정보를 출력합니다. 로봇이 가는 라인은 빨간색, 나머지는 파란
   색 rectangle을 그립니다. 그리고 중심점이 되는곳에 circle을 이용해 점을 그립니다.

   <img width="288" height="47" alt="image" src="https://github.com/user-attachments/assets/825958b2-5836-44ba-ac02-213f481aea63" />

5. 이미지 출력
   imshow를 이용해 원본 영상과 이진화후 검출되는 영상을 출력합니다.

<img width="288" height="47" alt="image" src="https://github.com/user-attachments/assets/fee41f96-2d16-4574-a6a2-7288cfadbab1" />

