# Milk V Duo シリーズ RTOS開発用環境の構築

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
1. イメージを焼いたSDカードを用意する。
    1. 使用するMilk-V Duoデバイスに応じたイメージファイルをダウンロードする
        - https://github.com/milkv-duo/duo-buildroot-sdk/releases/
            からDL可能
        - 旧verのイメージはページ下部にある
        - Arduinoを使って開発したい場合はファイル名の先頭にArduinoと
            ついたものを選ぶ<br>
            (最新版ではArduino対応版のリリースがない場合がある)
    1. RufusやbalenaEtcherを使用してmicro SDカードにイメージを焼く
    - (参考) https://milkv.io/docs/duo/getting-started/boot

1. Milk-V Duoデバイスにmicro SDカードを差し込んだ後、
    USB Type Cケーブルを用いてPCと接続する

1. RNDIS経由でSSH接続し、Linuxへアクセスする。
    - 接続しただけではRNDISドライバが設定されないため、
        手動で設定する必要がある
        - (参考) https://milkv.io/docs/duo/getting-started/setup
    - M2,M3チップのMacはRNDISドライバが対応していない

1. SSH接続が確認できたらセットアップは完了


## RTOS用のソフトウェア開発方法の概要
- (参考) https://github.com/milkv-duo/duo-buildroot-sdk
1. 手順
    1. build-root-sdkを入手
    1. RTOSのファイルを適宜書き換え
    1. sdkをビルドしてイメージファイルを作成
    1. Milk-V Duo に差すmicro SDに作成したイメージを焼く

1. 注意事項
    - 参考URLに書いてある通り、Ubuntu22.04環境でのビルドが推奨されている。
    - 筆者環境において、
        WSLやDockerなどの仮想環境ではうまく動かないことを確認しているため、
        下記の通りVirtualBoxなどで仮想マシンの環境で行うことを推奨する。


## Ubuntu22.04仮想マシンのセットアップ
1. VirtualBoxをインストールし、Linux仮想マシンを構築する。
    - (参考) https://qiita.com/pez/items/a3ef1855f7e1e0ed3dfd
    - OSイメージファイル(.iso)は適当なものを選択する。
      - MilkV の推奨している環境は Ubuntu22.04
      - 筆者個人的には日本語Remixではなく通常のものを推奨

- 仮想マシンの設定時に発生する問題に対する対処は `Appx_Setup_VBox.md`を参照


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
  
1. `duo-buildroot-sdk`のビルドが通ることを確認する
    1. `$ cd [buildroot-dir]/duo-build`
    1. `$ ./build.sh`
    1. 使い方(対象milk-v-duoデバイスの選択方法)が表示される
    1. `$ ./build.sh [target_device_name]`
    1. `Create SD image successfull: ~`が表示されるまで待つ
    - 始めてビルドをするときにダウンロードされるビルドツールのファイルサイズが
    大きいため固定回線推奨
    - ビルド自体に結構時間がかかるので注意(30分~1時間程度)