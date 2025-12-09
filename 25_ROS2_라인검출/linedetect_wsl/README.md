2101034 고찬영

Jetson키트가 없어서 WSL 환경에서 진행했습니다.
wsl에서 Publish한 메시지를 Mat으로 변환하여 영상 프레임에서 라인을 찾고 오류(error)를 계산하는 노드입니다. 

1. 전처리
   입력 영상을 받아서 압축을 풀고, Gray변환 + 밝기 보정 + 이진화 + 하단 1/4를 roi로 만들어 라인 검출    하기 좋은 형태로 전처리 하는 코드이다.

 cv::Mat frame_color = cv::imdecode(cv::Mat(msg->data), cv::IMREAD_COLOR);//이미지 디코딩
    if(frame_color.empty()) return;
    cv::Mat frame_gray;
    cv::cvtColor(frame_color, frame_gray, cv::COLOR_BGR2GRAY);//bgr->gray
    cv::Scalar bright_avg = cv::mean(frame_gray); //밝기값 평균
    frame_gray = frame_gray + (100 - bright_avg[0]);//밝기값 평균
    cv::Mat frame_binary;
    cv::threshold(frame_gray, frame_binary, 130, 255, cv::THRESH_BINARY);//이진화
    cv::Mat roi = frame_binary(cv::Rect(0, 240, 640, 120));//1/4값 저장
