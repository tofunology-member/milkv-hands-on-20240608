# VirtualBox + Ubuntu Desktop 22.04 LTSセットアップ時の問題への対処

## VirtualBoxでLinuxのインストールウィンドウが見切れる
- 筆者環境では問題なかった
- 発生する場合があるらしいのでURLのみ添付 <br>
    -> https://www.hanatare-papa.jp/entry/technology-os-centos-1

## Terminalが起動しない & sudoできない
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

## クリップボード共有を使えるようにする (対応中)<br>
1. VirtualBoxで仮想マシンを開く。
1. 仮想マシンのウィンドウのメニューバーから、
    `[デバイス] -> [クリップボードの共有]-> [双方向]`
    と選択する。
1. 仮想マシンのウィンドウのメニューバーから、
    `[デバイス] -> [Guest Additions CDイメージの挿入]`
    と選択する。
1. 仮想マシンのUbuntuのファイルマネージャーから
    `VBox_GAs_x.xx`
    がマウントされていることを確認する。
1. `$ cd /media/$USER/VBox_GAs_*` を実行し、
    CDイメージのディレクトリへ移動する。
1. `$ sudo ./VBoxLinuxAdditions.run` で
    Guest Additionsのインストールを行う。
1. `$ reboot`で再起動する。
1. 再起動後、ホストOS <-> ゲストOS間で
    クリップボードが共有されていることを確認する。