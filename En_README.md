# QtMultiChat

Go back to [Korean 한글 설명서](https://github.com/m3nukim/QtMultiChat/blob/master/README.md)

Randomly matched chatting program for user who has [Dongguk Univ. E-mail](https://mail.dongguk.edu) account.

Built & tested on Ubuntu 16.04 LTS and Qt 5.8.0 .

We recommend you to build with [Qt Creator & Qt 5.8.0](http://download.qt.io/official_releases/qt/5.8/5.8.0/qt-opensource-linux-x64-5.8.0.run)


Forked from https://github.com/kolya-t/Chat [ Thanks to [Nikolay Toporkov](https://github.com/kolya-t) ]

SMTP library from https://github.com/xcoder123/SimpleSmtp_SSL_QT5

Encyrption library from [SimpleCrypt](https://wiki.qt.io/Simple_encryption_with_SimpleCrypt)

# Build yourself

Every procedure will be conducted in terminal. (Ctrl + Alt + T on Ubuntu 16.04 LTS)

## 1. Install Qt5

<pre><code>sudo apt install qt5-default qtmultimedia5-dev libqt5multimedia5-plugins</code></pre>

or you can build with [Qt Creator & Qt 5.8.0](http://download.qt.io/official_releases/qt/5.8/5.8.0/qt-opensource-linux-x64-5.8.0.run)

## 2. Install dependencies

<pre><code>sudo apt install build-essential mesa-common-dev libglu1-mesa-dev libgtk2.0-dev libpulse-dev </code></pre>

## 3. Clone

<pre><code>git clone https://github.com/m3nukim/QtMultiChat.git</pre></code>

then, go to cloned directory

<pre><code>cd QtMultiChat</pre></code>

## 4. Build

Client and Server have the exact same procedure.

### Client

Go to "Client" directory from "QtMultiChat" directory

<pre><code>cd Client</pre></code>

Create makefile then build

<pre><code>qmake
make</pre></code>

finally execute!

<pre><code>./Client</pre></code>

### Server

Go to "Server" directory from "QtMultiChat" directory

<pre><code>cd Server</pre></code>

Create makefile then build

<pre><code>qmake
make</pre></code>

finally execute!

<pre><code>./Server</pre></code>
