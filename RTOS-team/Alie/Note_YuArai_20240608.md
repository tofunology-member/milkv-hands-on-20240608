# Milk V Duo シリーズ RTOS開発フロー

## 執筆時の環境
- 執筆日時: 2024/06/09

- Duo関連の環境
    - Duo Device: Milk-V Duo 256M
    - Duo-Image: ver 1.1.1
        - used : `"milkv-duo256m-v1.1.1-2024-0528.img"`
    - Duo-Buildroot-SDK : Commit ver. bc680ca (2024/6/7)

- 筆者のPC環境
    - VirtualBox: ver 7.0.18
    - PC (Host OS) : Windows 11 Home (23H2)
    - Virtual Machine: Ubuntu Desktop 22.04.4

## Milk Vのセットアップ(RTOS, Arduinoで共通)
1. イメージを焼いたSDカードを用意して起動する。
    - (参考) https://milkv.io/docs/duo/getting-started/boot
    - Arduinoを使って開発したい場合はファイル名の先頭にArduinoとついたものを選ぶ。
    - RTOSで開発したい場合はArduinoがついてないものを選ぶ。

1. PCとUSB接続、SSH経由でLinuxへアクセスする。
    - (参考) https://milkv.io/docs/duo/getting-started/setup
    - M2,M3チップのMacの場合、必要なRNDISドライバが対応していないためこの方法では
        できないかもしれない。

1. SSH接続が確認できたらセットアップは完了


## RTOS用のソフトウェア開発方法の概要
- (参考) https://github.com/milkv-duo/duo-buildroot-sdk
1. 手順
    1. build-root-sdkを入手
    1. 必要なファイルを書き換え
    1. sdkをビルドしてイメージ作成
    1. milk v 起動イメージを作成したイメージに書き換え

1. 注意事項
    - 参考URLに書いてある通り、Ubuntu22.04環境でのビルドが推奨されている。
    - 筆者環境において、WSLやDockerなどの仮想環境ではうまく動かないことを確認している
        ため、下記の通りVirtualBoxなどで仮想マシンの環境で行うことを推奨する。


## Ubuntu22.04仮想マシンのセットアップ
1. VirtualBoxをインストールし、Linux仮想マシンを構築する。
    - (参考) https://qiita.com/pez/items/a3ef1855f7e1e0ed3dfd
    - OSイメージファイル(.iso)は適当なものを選択する。
      - milk V推奨はUbuntu22.04
      - 個人的には日本語Remixではなく通常のものを推奨
    - VirtualBoxでLinuxのインストールウィンドウが見切れる場合
      - 筆者環境では問題なかった
      - 発生する場合があるらしい -> https://www.hanatare-papa.jp/entry/technology-os-centos-1

1. インストール後の初期状態で発生が想定される問題への対処
    - 想定される問題:  Terminalが起動しない, sudoできない
    1. sudoできるようにする
        1. [Ctrl + Alt + F3] で仮想コンソールに入る<br>
        1. 普段使うユーザー(デフォルト設定だと`vboxuser`)でログイン
        1. `$ su`を実行してsuperuserにログインする。
            パスワードは`vboxuser`のものでログインできる。
        1. `$ usermod -G sudo vboxuser` を実行してsudoの権限を付与する。
            `vboxuser`の箇所は適宜自分が使用しているユーザー名に変更すること。
        1. `$ reboot`で再起動する
        1. 再起動後、sudoが使えるようになる
        - (参考) https://qiita.com/kohei-takao/items/3beb89fb95d9cc472125
  
    1. terminalを使えるようにする<br>
        1. [Ctrl + Alt + F3]で仮想コンソールに入る<br>
        1. `$ vi /etc/default/locale`でLocaleを編集する。
          エディタはvim以外でもよい。
        1. `LANG="en_US"` を `LANG="en_US.UTF-8"` に変更する。
          類似している項目名が多いのでよく確認すること
        1. `$ reboot`で再起動する。
        1. 再起動後、Terminalが起動できる。
        -  (参考) https://qiita.com/towamz/items/2052f08e9e1af4068a56
        -  日本語環境でいいのであれば、設定から言語と地域をJapanに設定すると解決する。

1. クリップボード共有を使えるようにする (対応中)<br>
    1. VirtualBoxで仮想マシンを開く。
    1. 仮想マシンのウィンドウのメニューバーから、[デバイス] -> [クリップボードの共有]
        -> [双方向] と選択する。
    1. 仮想マシンのウィンドウのメニューバーから、
        [デバイス] -> [Guest Additions CDイメージの挿入] と選択する。
    1. 仮想マシンのUbuntuのファイルマネージャーから`VBox_GAs_x.xx`が
        マウントされていることを確認する。
    1. `$ cd /media/$USER/VBox_GAs_*` を実行し、CDイメージのディレクトリへ
        移動する。
    1. `$ sudo ./VBoxLinuxAdditions.run` でGuest Additionsのインストールを行う。
    1. `$ reboot`で再起動する。
    1. 再起動後、ホストOS <-> ゲストOS間でクリップボードが共有されていることを
        確認する。

## 仮想マシン上でのSDKのビルド
1. `duo-buildroot-sdk`のビルドに必要パッケージを入れる
    1. `$ sudo apt update`
    1. `$ sudo apt install -y pkg-config build-essential ninja-build automake autoconf libtool wget curl git gcc libssl-dev bc slib squashfs-tools android-sdk-libsparse-utils jq python3-distutils scons parallel tree python3-dev python3-pip device-tree-compiler ssh cpio fakeroot libncurses5 flex bison libncurses5-dev genext2fs rsync unzip dosfstools mtools tcl openssh-client cmake expect`

1. gitを入れる
    1. `$ sudo apt install git`
  
1. `duo-buildroot-sdk`の最新バージョンをgitからクローンする
    1. 適当なフォルダ`[buildroot-dir]`を作成し、`$ cd [buildroot-dir]`で移動する。
    1. `$ git clone https://github.com/milkv-duo/duo-buildroot-sdk.git --depth=1`
        - オプションの`--depth=1`は抜いてもいいがダウンロードファイルサイズが大きくなるのでおすすめしない。
        - ダウンロードするファイルサイズが大きいので、モバイル回線ではなく、自宅などの固定回線でやること。
  
1. `duo-buildroot-sdk`のビルド
    1. `$ cd [buildroot-dir]/duo-build`
    1. `$ ./build.sh`
    1. 使い方(対象milk-v-duoデバイスの選択方法)が表示される
    1. `$ ./build.sh [target_device_name]`
    1. `Create SD image successfull: ~`が表示されるまで待つ
    - 始めてビルドをするときにダウンロードされるビルドツールのファイルサイズが
    大きいため固定回線推奨
    - ビルド自体に結構時間がかかるので注意(30分~1時間程度)