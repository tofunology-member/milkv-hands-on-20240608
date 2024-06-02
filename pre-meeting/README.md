## まずはtofunologyとフカウミさんで足並みをそろえたい
[団体紹介.pdf](https://1drv.ms/b/s!Aik5Z1FR2UJchIYYttUAW_XDsu4SJw?e=GaevKF)

## 1\. みんなが気になること

* **Arduino部分:よく使うパーツに対して， ライブラリはあるのかorないなら作れるのか**
    * サーボモータ(5V PWM)・NeoPixel・ステッピングモータ(A4988)
    * ステッピングモータの制御
        * 今の所使えると予想(volatileやasmがないので) [https://github.com/waspinator/AccelStepper/tree/master](https://github.com/waspinator/AccelStepper/tree/master)
    * サーボモータの制御
        * Arduinoのサーボモータのライブラリに[RISC-V](https://milkv.io/docs/duo/getting-started/arduino)対応がないので，PWMで自作
        * Linux側でwiringxを使っている(Arduino側ではない) [https://milkv.io/ja/docs/duo/application-development/sensor-demo#df9gms-180](https://milkv.io/ja/docs/duo/application-development/sensor-demo#df9gms-180)
        * analogWriteを使える→Servoライブラリは自分で作る感じ？[https://milkv.io/ja/docs/duo/getting-started/arduino#pwm-usage-example](https://milkv.io/ja/docs/duo/getting-started/arduino#pwm-usage-example)
    * NeoPixelの制御
        * Adafruit\_NeoPixelを使いたいがまだ厳しいかも．
        * 仕様書を見て，以下を参考に改造すればもしかしたらって感じ
            * [https://github.com/adafruit/Adafruit_NeoPixel/pull/376](https://github.com/adafruit/Adafruit_NeoPixel/pull/376)

* **Linuxで受け取ったデータをどうやってArduinoへ流すか(逆も然り)** 
    * まだ検証できていないのでハンズオンでやりたいが、ハードウェアの制御のほう優先で
    * (RTOSなら)Mailboxでやりとり：[https://milkv.io/docs/duo/getting-started/rtoscore](https://milkv.io/docs/duo/getting-started/rtoscore)
    * RTOSでやる方法しかないって嘆き：[https://community.milkv.io/t/best-way-to-communicate-between-linux-and-arduino-program-on-milk-v-duo/1463](https://community.milkv.io/t/best-way-to-communicate-between-linux-and-arduino-program-on-milk-v-duo/1463)
    * Big CoreとLittle Coreでピンを物理的に接続して接続：[https://milkv.io/docs/duo/getting-started/arduino](https://milkv.io/docs/duo/getting-started/arduino)
* **LinuxからArduinoのプログラムを書き込めるのか**
    * FreeRTOSのELFファイルの置き換え方法(Arduinoではない)：[https://community.milkv.io/t/it-is-possible-to-burn-arduino-remotely/1655/2](https://community.milkv.io/t/it-is-possible-to-burn-arduino-remotely/1655/2)
    * 議論はある：[https://community.milkv.io/t/arduino-image-and-freertos/1537](https://community.milkv.io/t/arduino-image-and-freertos/1537)
* **ラズパイみたいに急に電源落としても大丈夫なのか**
    * ぎてふ「多分だめ」
* **Linuxどんなの動かす？**
    * かわそん「Python3でHTTP通信(restAPI)」
    * Ubuntu22.04：[https://milkv.io/docs/duo/getting-started/buildroot-sdk](https://milkv.io/docs/duo/getting-started/buildroot-sdk)

## 2\. ハンズオンの目的/内容の確認

### 概要
```md
**日付**：06/08
**時間**：
* 10:00-13:00 (3h)：オープンスペースで活動
* 13:00-14:00 (1h)：休憩
* 14:00-17:00 (3h)：会議室で活動
* 17:00- 飲み
**場所**：アップス [https://ups-s.com/](https://ups-s.com/)
**メンバー**：Milk-V 深海さん+tofunology数名
```

### 目的

* まずは使いかたを一気にみんなで学んでみよう
* githubにupしちゃって記事の1つを作ってしまおう

### 流れ

1. Milk-V お渡し会
2. チームで開発+githubに適宜up
3. githubのREADME作成してup

### チームについて
#### A. Linux+FreeRTOS + Uさん(ARAI Yu ),ぎてふ

* **目標(最低)** Linux+FreeRTOSのFreeRTOS側でLチカ
* **目標(望ましい)** Linuxを介して，FreeRTOSのLEDをホストコンピュータで制御
* **目標(最大)** Linuxを介してFreeRTOSのLEDをホストコンピュータで制御+センサー情報を取得

#### B. Linux+Arduino + かわそん，たかちゃん，総長(Sho Onoguchi)

* **目標(最低)** Linux+ArduinoのArduino側でLチカ
* **目標(望ましい1)** Linuxを介して，ArduinoのLEDをホストコンピュータで制御
* **目標(最大1)** Linux+ArduinoのArduino側でサーボ制御(たかちゃん)
* **目標(最大2)** ホスト-Linux-Arduinoの通信調査(かわそん)
* **目標(最大3)** Linuxを介してArduinoのLEDをホストコンピュータで制御+センサー情報を取得(総長(オノグチ))

## 3\. 広報/公開資料

### github

githubに基本的にup．顔が写ってたりする写真とか機密情報的なのはoneDriveに．

* **github：[https://github.com/tofunology-member/milkv-hands-on-20240608](https://github.com/tofunology-member/milkv-hands-on-20240608)**
    * 深海さんは招待済み( [https://github.com/MilkVNaoki](https://github.com/MilkVNaoki) )
* **oneDrive : [https://1drv.ms/f/s!Aik5Z1FR2UJchIYJ5DWgPjOyYn3E0A](https://1drv.ms/f/s!Aik5Z1FR2UJchIYJ5DWgPjOyYn3E0A)**

### xに関して

> 「https://x.com/MilkV_Official 何かつぶやくときはここに@してもらえると嬉しいです」

## 4\. 必要な事前準備

### 当日持ち物
#### 全員必須のもの
* ノートパソコン（かわそんは持っていない→ぎてふ準備）
* microSD-SD変換(あれば)
* SDカード変換(あれば)
* microSD：2,3個以上(みんな持ってくる)
* typeCのデータ通信できるケーブル：(全員，無いひとは相談)
* ブレッドボード：全員，持ってない人は他のメンバーに相談
* ジャンパワイヤ：たくさん持ってくる

#### 持ってる人は持ってくるモノ
* LEDと抵抗：持ってきてほしい(3.3Vで光るか要チェック)
* スイッチ・10kΩ抵抗(4.7kΩでも可)：持ってこれる人

#### たかちゃん
* MG996R：2つ(たかちゃん) [https://akizukidenshi.com/catalog/g/g112534/](https://akizukidenshi.com/catalog/g/g112534/)
* 撮影道具(たかちゃん)
* L3ルータ，L2スイッチ：(たかちゃん)
* シリアル-USB変換：(たかちゃん，深海さん)
    * たかちゃんが持っているやつ [https://amz.run/9EN6](https://amz.run/9EN6)
* ピンヘッダ80pin以上：(たかちゃん)
* DCジャックDIP化4つ：（たかちゃん）
* 3.3-5レベル変換(4ビット双方向ロジックレベル変換モジュール BSS138)x4：(たかちゃん)

#### 深海さん
* シリアル-USB変換：(たかちゃん，深海さん)
    * [こんな感じのやつhttps://milkv.io/docs/duo/arduino/duo-arduino-08.jpg](https://milkv.io/docs/duo/arduino/duo-arduino-08.jpg)
* 届いたmilkvたち

#### ぎてふ
* LANケーブル：7本以上(ぎてふ)
* スルーホール用テストワイヤ：40本(ぎてふ)
* 5V電源1.5Aくらい？DCプラグのやつx2：(ぎてふ)
* 12V電源1.5Aくらい？DCプラグのやつx1：(ぎてふ)

#### かわそん
* ステッピングモータx1：(かわそん)
    * [持ってるやつ](https://akizukidenshi.com/catalog/g/g106578/)
* ハンダセット(当日ピンの半田づけ)：(かわそん)
* テスター(かわそん，Uさん)

#### Uさん
* テスター(かわそん，Uさん)

#### 未定・要相談リスト
* A4988(ステッピングモータドライバ)x1：(要相談，discord)


### 事前準備
1. microSDカード事前にビルド(全員)
    - 以下のリンクを参考に作成
    - [https://milkv.io/ja/docs/duo/getting-started/boot](https://milkv.io/ja/docs/duo/getting-started/boot)
2. githubの整備(ぎてふ)
3. macにiPad繋げて作業できるかチェック(ぎてふ，優先度低め)
4. 荷物の相談をdiscordで，(全員)
5. 各チームで調査(6時間で終わらせられるように)，(全員)
6. 持ってくるノートPCにgithubの色々設定やっておいてください(SSH)，(全員)

---

## 参考資料

* https://x.com/od_1969/status/1783162340407746847
