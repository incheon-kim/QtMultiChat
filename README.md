# QtMultiChat

Go to [README in english](https://github.com/m3nukim/QtMultiChat/blob/master/En_README.md)

[동국대학교 웹메일](https://mail.dongguk.edu) 계정을 소유한 사용자들을 위한 랜덤채팅 프로그램입니다.

Ubuntu 16.04 LTS , Qt5.8 환경에서 빌드하고 테스트 했습니다.

[Qt Creator & Qt 5.8.0](http://download.qt.io/official_releases/qt/5.8/5.8.0/qt-opensource-linux-x64-5.8.0.run)
을 설치하고 그 환경에서 빌드하는 것을 권장합니다.


https://github.com/kolya-t/Chat 에서 포크된 프로젝트 입니다. 
// Thanks to [Nikolay Toporkov](https://github.com/kolya-t)

SMTP통신을 위한 라이브러리는 [SimpleSmtp_SSL_QT5](https://github.com/xcoder123/SimpleSmtp_SSL_QT5) 를 사용했습니다.

# 빌드하기

모든 과정은 터미널에서 진행됩니다.

우분투 16.04 LTS 환경에서 Ctrl + Alt + T로 터미널을 실행할 수 있습니다.



## 1. Qt5 설치하기

터미널에서 해당 명령어를 입력하세요.

<pre><code>sudo apt install qt5-default qtmultimedia5-dev libqt5multimedia5-plugins</code></pre>

또는 [Qt Creator](http://download.qt.io/official_releases/qt/5.8/5.8.0/qt-opensource-linux-x64-5.8.0.run)
를 설치, 사용하여 빌드 할 수 있습니다. 

## 2. 필수패키지 설치

터미널에서 해당 명령어를 입력하세요.

<pre><code>sudo apt install build-essential mesa-common-dev libglu1-mesa-dev libgtk2.0-dev libpulse-dev </code></pre>

## 3. 소스파일받기

터미널에서 해당 명령어를 입력하여 github로부터 소스를 클론합니다. 또는 github 페이지에서 직접 소스를 받을 수 있습니다.

<pre><code>git clone https://github.com/m3nukim/QtMultiChat.git</pre></code>

github로부터 클론한 소스파일이 포함된 폴더로 이동합니다.

<pre><code>cd QtMultiChat</pre></code>

## 4. 빌드하기

클라이언트와 서버프로그램의 빌드과정은 거의 동일합니다.

### 클라이언트

QtMultiChat 디렉토리로부터 Client 디렉토리로 이동합니다.

<pre><code>cd Client</pre></code>

Makefile 생성 및 컴파일을 진행합니다

<pre><code>qmake
make</pre></code>

그리고 실행합니다! 클라이언트는 서버를 먼저 실행한 상태에서 실행해야 정상작동 합니다.

<pre><code>./Client</pre></code>

### 서버

QtMultiChat 디렉토리로부터 Server 디렉토리로 이동합니다.

<pre><code>cd Server</pre></code>

Makefile 생성 및 컴파일을 진행합니다

<pre><code>qmake
make</pre></code>

그리고 실행합니다!

<pre><code>./Server</pre></code>
