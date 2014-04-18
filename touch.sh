adb shell "sendevent /dev/input/event0 0003 0057 00000000 && sendevent /dev/input/event0 0003 0048 00000010 && sendevent /dev/input/event0 0003 0058 00000070 && sendevent /dev/input/event0 0003 0053 $1 && sendevent /dev/input/event0 0003 0054 $2 && sendevent /dev/input/event0 0000 0000 00000000 && sendevent /dev/input/event0 0003 0057 4294967295 && sendevent /dev/input/event0 0000 0000 00000000"

#adb shell sendevent /dev/input/event0 0003 0057 00000000
#adb shell sendevent /dev/input/event0 0003 0048 00000010
#adb shell sendevent /dev/input/event0 0003 0058 00000070
#adb shell sendevent /dev/input/event0 0003 0053 $1
#adb shell sendevent /dev/input/event0 0003 0054 $2
#adb shell sendevent /dev/input/event0 0000 0000 00000000
#adb shell sendevent /dev/input/event0 0003 0057 4294967295
#adb shell sendevent /dev/input/event0 0000 0000 00000000
