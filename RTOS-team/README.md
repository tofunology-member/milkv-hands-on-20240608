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
    1. 使用するMilk-V Duoデバイスに応じたイメージファイルをダウンロードする。
        - https://github.com/milkv-duo/duo-buildroot-sdk/releases/
            からDL可能である。
        - 旧verのイメージに関してはページ下部にある。
        - Arduinoを使って開発したい場合はファイル名の先頭に`Arduino`と
            ついたものを選ぶ。<br>
            (最新版ではArduino対応版のリリースがない場合があるので旧verのイメージを確認すること。)
    1. RufusやbalenaEtcherを使用してmicro SDカードにイメージを焼く。
    - (参考) https://milkv.io/docs/duo/getting-started/boot

1. Milk-V Duoデバイスにmicro SDカードを差し込んだ後、
    USB Type Cケーブルを用いてPCと接続する。

1. RNDIS経由でSSH接続し、Linuxへアクセスする。
    - 接続しただけではRNDISドライバが設定されないため、
        手動で設定する必要がある。
        - (参考) https://milkv.io/docs/duo/getting-started/setup
    - M2,M3チップのMacはRNDISドライバが対応していないため注意。

1. SSH接続が確立したことを確認する。


## RTOS用のソフトウェア開発方法の概要
- (参考) https://github.com/milkv-duo/duo-buildroot-sdk
1. 手順
    1. SDK（duo-buildroot-sdk）を入手する。
    1. FreeRTOSのファイルを書き換える。
    1. SDKをビルドしてイメージファイルを作成する。
    1. Milk-V Duo に差し込む micro SDに作成したイメージを焼く。

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

- 仮想マシンの設定時に発生する問題に対する対処は `Appx_Setup_VBox.md`を参照すること。


## SDKのビルド
- 本章の作業は仮想マシン上で実行すること。
1. `duo-buildroot-sdk`のビルドに必要なパッケージを入れる。
    1. aptのアップデートリストを更新する。
        ```bash
        $ sudo apt update
        ```
    1. `duo-buildroot-sdk`の開発依存パッケージをすべてインストールする。
        ```bash
        $ sudo apt install -y pkg-config build-essential ninja-build automake autoconf libtool wget curl git gcc libssl-dev bc slib squashfs-tools android-sdk-libsparse-utils jq python3-distutils scons parallel tree python3-dev python3-pip device-tree-compiler ssh cpio fakeroot libncurses5 flex bison libncurses5-dev genext2fs rsync unzip dosfstools mtools tcl openssh-client cmake expect
        ```
  
1. `duo-buildroot-sdk`の最新バージョンをgitからクローンする
    1. 適当なディレクトリ `<dist_dir_fullpath>` を作成する。
        ```bash
        $ mkdir <dist_dir_fullpath>
        ```
    1. 作成したディレクトリで移動する。
        ```bash
        $ cd <dist_dir_fullpath>
        ```
    1. GitHubから最新版の`duo-buildroot-sdk`をクローンする。
        ```bash
        $ git clone https://github.com/milkv-duo/duo-buildroot-sdk.git --depth=1
        ```
        - オプションの`--depth=1`は最新コミットのみをクローンするオプションであり、省略してもよいがダウンロードサイズが大きくなるためおすすめしない。
        - ダウンロードサイズが大きいため、モバイル回線ではなく自宅の固定回線などで作業することを推奨する。
  
1. `duo-buildroot-sdk`のビルドが通ることを確認する。
    1. SDKの保存されたディレクトリへ移動する。
        ```bash
        $ cd <dist_dir_fullpath>/duo-buildroot-sdk
        ```
    1. 移動先のディレクトリ内にあるシェルスクリプト`build.sh`を実行する。
        ```bash
        $ ./build.sh
        ```
    1. 使い方(milk-v-duoデバイスタイプの選択方法)が表示される。
        - `./build.sh <target_device_name>`
        - `./build.sh launch`
        - 上記2通りのパターンがあるが、本稿では上の形式のものを指定する。
    1. デバイスタイプを選択してビルドを実行する。
        ```bash
        $ ./build.sh <target_device_name>
        ```
    1. 以下のメッセージが表示されるまで待機する。
        ```
        Create SD image successfull: ~
        ```

- 注意事項
    - 始めてビルドをするときにダウンロードされるビルドツールのファイルサイズが
    非常に大きいため、この作業時は固定回線などを使用することを推奨する。
    - ビルド自体にそれなりの時間(30分~1時間程度)がかかるため注意すること。

## オンボード青色LEDの制御(Lチカ)
- 本章についてはDuo 256Mを使用した場合のサンプルとなっている。
    他のデバイスの場合、操作するレジスタなどが異なるため注意すること。
1. FreeRTOSのタスクを記述したのソースファイルを編集する。
    `/duo-buildroot-sdk/freertos/cvitek/`
    - エディタは自由である。仮想マシンのGUI操作が面倒であればVScodeからSSH経由で編集してもよい。
    - 当該ファイルは下記の様に書き換える。
        ```c
        /* Standard includes. */
        #include <stdio.h>

        /* Kernel includes. */
        #include "FreeRTOS.h"
        #include "task.h"
        #include "semphr.h"
        #include "mmio.h"
        #include "delay.h"

        /* cvitek includes. */
        #include "printf.h"
        #include "rtos_cmdqu.h"
        #include "cvi_mailbox.h"
        #include "intr_conf.h"
        #include "top_reg.h"
        #include "memmap.h"
        #include "comm.h"
        #include "cvi_spinlock.h"

        /* Milk-V Duo */
        #include "milkv_duo_io.h"

        // #define __DEBUG__
        #ifdef __DEBUG__
        #define debug_printf printf
        #else
        #define debug_printf(...)
        #endif

        #define BLUE_LED_PIN 0x05021000

        void blinkLEDs (void* param) {
            * (volatile int *) (BLUE_LED_PIN + 0x0004) |= (1 << 2);
            while (1) {
                * (volatile int *) (BLUE_LED_PIN + 0x0000) |= (1 << 2);    // on
                vTaskDelay(100 / portTICK_PERIOD_MS);   // delay 100ms
                * (volatile int *) (BLUE_LED_PIN + 0x0000) &= ~(1 << 2);    //off
                vTaskDelay(100 / portTICK_PERIOD_MS);   // delay 100ms
                * (volatile int *) (BLUE_LED_PIN + 0x0000) |= (1 << 2);    // on
                vTaskDelay(100 / portTICK_PERIOD_MS);   // delay 100ms
                * (volatile int *) (BLUE_LED_PIN + 0x0000) &= ~(1 << 2);    // off
                vTaskDelay(700 / portTICK_PERIOD_MS);   // delay 700ms
            }
        }

        void main_cvirtos(void)
        {
            printf("create cvi task\n");

            /* Start the tasks and timer running. */
            xTaskCreate(blinkLEDs, "blinkLEDs", 1024, NULL, 1, NULL);
            /* Start the tasks and timer running. */
            vTaskStartScheduler();

            printf("cvi task end\n");
            
            for (;;)
                ;
        }

        ```

1. `duo-buildroot-sdk`のディレクトリへ移動する
    ```bash
    $ cd <dist_dir_fullpath>/duo-buildroot-sdk
    ```

1. デバイスタイプを選択してビルドを実行する。
    ```bash
    $ ./build.sh milkv-duo256m
    ```

1. 生成されたイメージファイルを micro SDに焼く。

1. micro SDをMilkV Duo 256Mに差し込み電源を入れる。

1. 青色LEDが2回点滅を1秒間隔で繰り返されることを確認する。
    - Linux側でLEDを制御するシェルスクリプトがバックグラウンドで稼働しているため
    点滅パターンが不規則に見える。
    - 気になる場合はMilkVに対してSSH接続してターミナルからシェルスクリプトを停止したのち再起動すること。コマンドは以下の通り。
        ```bash
        $ mv /mnt/system/blink.sh /mnt/system/blink.sh_backup && sync
        $ reboot
        ```
