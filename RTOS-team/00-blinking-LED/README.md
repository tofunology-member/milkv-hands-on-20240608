# 事前調査
## RTOS部分の問題点
チュートリアルはあくまでFreeRTOS部分に既存で組まれているプログラムをmailboxで同期どうするかのみ．
つまり，FreeRTOS部分に新しいタスクを作りたいと思っても，その方法が載っていないのだ．

### 解決方法

#### まずはコンパイルする
[https://www.youtube.com/watch?v=bx6BNTwsdZA](https://www.youtube.com/watch?v=bx6BNTwsdZA)



[https://community.milkv.io/t/freertos/1378/12](https://community.milkv.io/t/freertos/1378/12)

```sh
# copy firmware to /lib/firmware dir:
scp -O duo-buildroot-sdk/freertos/cvitek/install/bin/cvirtos.elf root@<duo ip>:/lib/firmware

# stop existing firmware
echo stop > /sys/class/remoteproc/remoteproc0/state
# change firmware name
echo cvirtos.elf > /sys/class/remoteproc/remoteproc0/firmware
# start
echo start > /sys/class/remoteproc/remoteproc0/state
```

# 現状
コンパイルに必要な環境すら整っていない(duo-buildroot-sdk)