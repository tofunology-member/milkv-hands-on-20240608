# 新井の作業メモ

## 最初のセットアップ
1. イメージを焼いたSDカードを用意して起動
<参考> https://milkv.io/docs/duo/getting-started/boot

1. PCとUSB接続、SSHでLinuxへアクセス
<参考> https://milkv.io/docs/duo/getting-started/setup

1. 以上で初期設定はとりあえずOK


## RTOS用のソフトウェア開発方法
<参考> https://github.com/milkv-duo/duo-buildroot-sdk
1. build-root-sdkを入手
1. 必要なファイルを書き換え
1. sdkをビルドしてイメージ作成
1. milk v 起動イメージを作成したイメージに書き換え


### ~~WSL2でのイメージ作成~~ <- これだとできない
1. WSL2でLinux仮想環境を入れる(milk V推奨はUbuntu 22.04)
1. 依存関係のあるパッケージを入れる <br>
  ```$ sudo apt install -y pkg-config build-essential ninja-build automake autoconf libtool wget curl git gcc libssl-dev bc slib squashfs-tools android-sdk-libsparse-utils jq python3-distutils scons parallel tree python3-dev python3-pip device-tree-compiler ssh cpio fakeroot libncurses5 flex bison libncurses5-dev genext2fs rsync unzip dosfstools mtools tcl openssh-client cmake expect ``` <br> <br>
  上記コマンド実行時にエラーが出ることがある <br>
  ``` E: Failed to fetch http://archive.ubuntu.com/ubuntu/pool/main/o/openssh/openssh-sftp-server_8.9p1-3ubuntu0.4_amd64.deb  404  Not Found [IP: 91.189.91.83 80] ``` <br><br>
  この場合は `$ sudo apt update` を実行してから再度`$ sudo apt install (以下略)` を実行

### 仮想マシン上のUbuntuでコンパイル
  1. 仮想マシンのセットアップ  (VirtualBox + Ubuntu) <br>
    参考(windows11) : https://qiita.com/pez/items/a3ef1855f7e1e0ed3dfd <br><br>
    OSイメージファイル(.iso)は適当なものを選択する。<br>
    milk V推奨はUbuntu22.04系、desktopは重いのでserverでもOK
    参考(VirtualBoxでインストールウィンドウが見切れる時の対策 ) : https://www.hanatare-papa.jp/entry/technology-os-centos-1

- Terminalが起動しないとき
  https://www.demandosigno.study/entry/2023/07/02/000000

- sudoできないとき
  https://qiita.com/kohei-takao/items/3beb89fb95d9cc472125